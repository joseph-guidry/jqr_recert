#include <list.h>

// node functions

int create_node(list_node_t * node, void * data)
{
    list_node_t new = malloc(sizeof(struct list_node));
    if(NULL == new)
    {
        return EXIT_FAILURE;
    }

    new->data = data;
    new->next = NULL;
    new->prev = NULL;

    *node = new;

    return EXIT_SUCCESS;
}

void destroy_node(list_node_t node)
{
    if(node)
    {
        free(node);
    }
}



// list functions
int create_list(list_t * list, 
                void (*rmv)(void * a),
                void (*dis)(void * a)
                )
{

    list_t new = malloc(sizeof(struct list));

    if(NULL == new)
    {
        printf("this is a failure\n");
        return EXIT_FAILURE; // failed to allocate memory for list
    }

    
    new->print_item_f = dis;
    new->remove_item_f = rmv;
    
    // Create sentinal node that points to itself
    if(EXIT_FAILURE == create_node(&new->anchor, "SENTINAL NODE"))
    {
        printf("this is a failure\n");
        return EXIT_FAILURE; // failed to allocate memory for list
    }

    new->anchor->next = LIST_ANCHOR(new);
    new->anchor->prev = LIST_ANCHOR(new);
    new->count = 0;

    *list = new;

    return EXIT_SUCCESS;
}

// use flag to empty list, or destory list
int destroy_list(list_t list, int flag)
{
    printf("destroying list\n");
    list_node_t curr = list->anchor->next;
    list_node_t node = NULL;
    while(curr != list->anchor)
    {
        
        node = curr;
        curr = curr->next;
        list->remove_item_f(node->data);
        
        destroy_node(node);
    }
    list->anchor->next = LIST_ANCHOR(list);
    list->anchor->prev = LIST_ANCHOR(list);

    if(flag)
    {
        destroy_node(list->anchor);
        free(list);
    }
}



int insert_list_item(list_t list, void * data, uint position)
{
    if(NULL == list || NULL == data)
    {
        printf("this is a failure\n");
        return EXIT_FAILURE; // failed to allocate memory for list
    }

    list_node_t new;
    list_node_t curr = list->anchor; //set curr to head link
    list_node_t prev = curr;

    if( create_node(&new, data) )
    {
        return EXIT_FAILURE;
    }

    //check memory of new node

    // Insert into empty list
    if(curr->next == list->anchor)
    {
        curr->next = new;
        curr->prev = new;

        new->next = curr;
        new->prev = curr;
    }
    else if((position < 0)  || (position >= list->count))
    {
        new->prev = curr->prev;
        curr->prev->next = new;

        new->next = LIST_ANCHOR(list);
        curr->prev = new;
    }
    else // insert into the middle of the list
    {
        // while(position-- > 0)
        for(long i = position; i > 0; i--)
        {
            prev = curr;
            curr = curr->next;
        }

        new->next = curr;
        new->prev = curr->prev;

        curr->prev->next = new;
        curr->prev = new;

    }
    

    list->count++;
    return EXIT_SUCCESS;
}

int remove_list_item(list_t list, void ** data, int position)
{
    if(NULL == list || position > list->count )
    {
        printf("this is a failure\n");
        return EXIT_FAILURE; // failed to allocate memory for list
    }

    if (list->anchor->next == list->anchor)
    {
        printf("List is empty\n");
        return EXIT_SUCCESS;
    }

    list_node_t curr = list->anchor->next;
    while(--position > 0)
    {
        curr = curr->next;
    }

    *data = curr->data;

    curr->data = NULL;
    curr->prev->next = curr->next;
    curr->next->prev = curr->prev;
    destroy_node(curr);

    list->count--;

    return EXIT_SUCCESS;
}

void display_list(list_t list, int flag)
{
    //TODO: allow for reverse display
    printf("displaying %ld items\n", list->count);
    list_node_t node = list->anchor->next;
    if(!flag)
    {
        while(node != LIST_ANCHOR(list))
        {   
            // printf("node %p & end %p\n", node->next, LIST_ANCHOR(list));
            list->print_item_f(node->data);
            node = node->next;
        }
    }
   
}

int find_list_item(list_t list, list_node_t * found, void * data)
{
    return 0;
}

void sort_list(list_t list)
{
    return;
}