#include <stdio.h>
#include <unistd.h>
#include <tp.h>
#include <word_list.h>


typedef struct test_work{
    enum  {INSERT = 0, LIST} type;
    void * data;
    word_list_t * wl;
}test_work_t;

// Call from word_utils 
void high_priority_task(void * args)
{
    printf("high priority\n");
    test_work_t * work = (test_work_t *) args;
    
    if(word_list_create(&work->wl, work->data, 10) == 0 )
    {
        printf("creating wordlist\n");
        // 4. Get list of strings for user
        word_list_build(work->wl);
    }

    printf("available word list:\n");
    for(int i = 0; i < work->wl->current; i++)
    {
        printf("%s\n", work->wl->word_list_array[i]);
    }
}

// Add word 
void low_priority_task1(void * args)
{
    test_work_t * work = (test_work_t *) args;
   

    // 2. Insert word into a trie DS
    // buff[strlen(buff) - 1] = '\0';
    printf("inserting: [%s]\n", (char *)work->data);

    word_list_trie_insert((char *)work->data);
}

void low_priority_task2(void * args)
{
    test_work_t * work = (test_work_t *)args;
    printf("removing: [%s]\n", (char *)work->data);

    word_list_trie_remove((char *)work->data);
}

int main(void)
{
    // 1. create tp
    tp_t * threadpool = NULL;
    if(tp_create(&threadpool))
    {
        fprintf(stderr, "Failed to allocate memory for threadpool\n");
        return EXIT_FAILURE;
    }

    char * buff = NULL;
    size_t bufflen = 0;

    test_work_t * work = calloc(1, sizeof(test_work_t));
    
    
    // 3. get task value updates
    printf("running tasks\n");
    for(int i = 1; i <= 10; i++)
    {
        if(i%4 == 0){ printf("need prefix\n");}
        if(i%7 == 0){ printf("word to remove\n");}
        else {printf("enter a word\n");}


        if(getline(&buff, &bufflen, stdin) > 0)
        {
            // 2. Insert word into a trie DS
            buff[strlen(buff) - 1] = '\0';
            printf("word: [%s]\n", buff);

        }
        work->data = (void *)buff;

        if(i % 4 == 0)
        {
            printf("getting list\n");
            work->type = LIST;
            word_list_create(&work->wl, (char *)work->data, 10);
            tp_queue_task(threadpool, high_priority_task, work, work->type);
        }
        else if (i %7){
            printf("remove word\n");
            work->type = INSERT;
            work->wl = NULL;
            tp_queue_task(threadpool, low_priority_task1, work, work->type);
        }
        else 
        {
            printf("insert word\n");
            work->type = INSERT;
            work->wl = NULL;
            tp_queue_task(threadpool, low_priority_task1, work, work->type);
        }
        sleep(1);
    }

    sleep(5);

    // 4. stop and destory pool
    tp_destory(threadpool);
    return 0;
}