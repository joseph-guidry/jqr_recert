#include <unistd.h>
#include <signal.h>

#include <tp.h>


// worker functions

void cleanup_worker(void * args);
void cleanup_task(void * args);

static int compare_tasks(const void * t1, const void * t2)
{
    task_t * task1 = (task_t *)t1; 
    task_t * task2 = (task_t *)t2; 

    return task2->priority > task1->priority ? 0 : 1;
}

static void destroy_task(void * a)
{
    if(NULL != a)
    {
        free(a);
        a = NULL;
    }
}

// function run by worker to get tasks
void * task_exec(void * args)
{
    tp_t * tp = (tp_t *)args;
    task_t * task = NULL;

    void (*func)(void *);


    pthread_cleanup_push(cleanup_worker, tp);

    pthread_mutex_lock(&tp->tp_lock);

    // should this be dynamically allocated?
    thread_t thread = {.tid = pthread_self(), .next = NULL};

    for(;;)
    {
        // check if there is work or status is set to shutdown
        while( (tp->status != STOP) && (heap_size(tp->queue) == 0))
        {
            if(tp->threadcnt >= tp->minthread)
            {
                pthread_cond_wait(&tp->work_notify, &tp->tp_lock);
            }
        
        }

        if(tp->status == STOP)
        {
            break;
        }

        // If there are tasks in the queue
        // if(is_empty(tp->queue))
        printf("here\n");
        if(heap_size(tp->queue))
        {
            // task = tp->head;
            // tp->head = task->next;
            printf("dequeue task\n");
            dequeue(tp->queue, (void *) &task);

            printf("task == NULL? %c\n", task == NULL ?'T':'F');
            func = task->routine;
            args = task->args;
            // tp->job_count--;

            free(task);

            // add thread_t to head of active workers list
            thread.next = tp->active_workers;
            tp->active_workers = &thread;

            pthread_cleanup_push(cleanup_task, tp);

            pthread_mutex_unlock(&tp->tp_lock);

            // pthread_mutex_lock(&task_lock);
            // printf("thread[%lu]: running new task\n", pthread_self(), run_tasks++);
            // pthread_mutex_unlock(&task_lock);

            func(args);
            pthread_cleanup_pop(1); // execute cleanup_job()
        }
    }

    pthread_cleanup_pop(1);  // execute cleanup_worker()
    return NULL;
}

void worker_init(tp_t * tp)
{
    pthread_t tid;
    printf("creating new worker\n");
    pthread_create(&tid, &tp->worker_attr, task_exec, tp);
    tp->threadcnt++;
}

void cleanup_worker(void * args)
{
    tp_t * tp = (tp_t *)args;
    if(NULL == tp)
    {
        return;
    }

    tp->threadcnt--;
    if(tp->status == STOP)
    {
        if(tp->threadcnt == 0)
        {
            pthread_cond_broadcast(&tp->stop_work);  // This indicated to the threadpool all threads are finished closing
        }
    }
    else if((tp->head != NULL) && (tp->threadcnt < tp->maxthread)) //if there is more work and not max threads, make new one
    {
        worker_init(tp);
    }

    pthread_mutex_unlock(&tp->tp_lock);
}

void cleanup_task(void * args)
{// TODO
    tp_t * tp = (tp_t *)args;
    pthread_t tid = pthread_self();
    thread_t * t, **tptr;
    
    pthread_mutex_lock(&tp->tp_lock);

    // What is this doing?
    tptr = &tp->active_workers;
    while( (t = *tptr) != NULL)
    {
        if(t->tid == tid)
        {
            *tptr = t->next;
        }
        tptr = &t->next;
    }

    // This is not finished
}

// threadpool functions

int tp_create(tp_t ** tp)
{
    tp_t * new = calloc(1, sizeof(tp_t));
    if(NULL == new)
    {
        perror("Threadpool allocation error: ");
        return EXIT_FAILURE;
    }

    new->status = RUN;

    if(heap_init(&(new->queue), compare_tasks, destroy_task))
    {
        free(new);
        return EXIT_FAILURE;
    }

    // new->head = NULL;
    // new->tail = NULL;

    pthread_mutex_init(&new->tp_lock, NULL);
    pthread_cond_init(&new->work_notify, NULL);
    pthread_cond_init(&new->stop_work, NULL);

    pthread_attr_init(&new->worker_attr);
    pthread_attr_setdetachstate(&new->worker_attr, PTHREAD_CREATE_DETACHED);

    for(int i = 0; i < MIN_WORKERS; i++)
    {
        printf("starting new worker\n");
        worker_init(new);
    }

    *tp = new;

    return EXIT_SUCCESS;
}

int tp_queue_task(tp_t * tp, void (*routine)(void *), void* args, int priority)
{
    task_t * task = malloc(sizeof(task_t));
    if(NULL == task)
    {
        return EXIT_FAILURE;
    }

    task->routine = routine;
    task->args = args;
    // task->next = NULL;
    task->priority = priority;

    pthread_mutex_lock(&tp->tp_lock);
    // push task into priority queue using priority

    // if(NULL == tp->head)
    // {
    //     tp->head = task;
    // }
    // else
    // {
    //     tp->tail->next = task;
    // }

    // tp->tail = task;
    // tp->job_count++;
    printf("adding task to queue\n");
    enqueue(tp->queue, task);

    if( (tp->threadcnt < tp->queue->size) && (tp->threadcnt < tp->maxthread) )
    {
        printf("adding worker\n");
        worker_init(tp);
    }

    pthread_cond_signal(&tp->work_notify);
    pthread_mutex_unlock(&tp->tp_lock);
    
    return EXIT_SUCCESS;
}

int tp_destory(tp_t * tp)
{
    thread_t * tptr;
    task_t * taskptr; 
    sigset_t set;

    tp->status = STOP;

    // Wake up sleeping workers
    pthread_cond_broadcast(&tp->work_notify);

 
    // Block signals from interrupting destorying threadpool
    sigfillset(&set);
    sigdelset(&set, SIGILL);
    sigdelset(&set, SIGFPE);
    sigdelset(&set, SIGSEGV);
    sigdelset(&set, SIGBUS);

    if(pthread_sigmask(SIG_BLOCK, &set, NULL))
    {
        return EXIT_FAILURE;
    }

    // Stop current worker threads
    for(tptr = tp->active_workers; tptr != NULL; tptr = tptr->next)
    {
        pthread_cancel(tptr->tid);
    }

    while(tp->threadcnt > 0)
    {
        pthread_cond_wait(&tp->stop_work, &tp->tp_lock);
    }
    
    pthread_mutex_unlock(&tp->tp_lock);


    heap_destroy(tp->queue);

    // for(taskptr = tp->head; taskptr != NULL; taskptr = taskptr->next)
    // {
    //     tp->head = taskptr->next;
    //     free(taskptr);
    // }

    pthread_mutex_destroy(&tp->tp_lock);
    pthread_cond_destroy(&tp->work_notify);
    pthread_cond_destroy(&tp->stop_work);
    pthread_attr_destroy(&tp->worker_attr);
    free(tp);

    return EXIT_SUCCESS;
}