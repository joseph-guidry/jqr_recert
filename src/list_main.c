#include <stdio.h>
#include <list.h>
//#include <tree.h>
//#incluce <graph.h>

void display_item(void *data)
{
    // printf("here2: %p\n", data);

    if(NULL == data)
    {
        puts("\t");
    }
    else
    {
        printf("%d\n", *(int *)data);
    }
}

void destroy_item(void * data)
{
    free(data);
}

int compare_item(void * a, void * b)
{
    int val1 = *(int *)a;
    int val2 = *(int *)b;
    return 0 ;
}

int main(void)
{
    list_t new_list = NULL;
    if(create_list(&new_list, destroy_item, display_item) )
    {
        return EXIT_FAILURE;
    }

    int * a;
    printf("Lets process some data into the list\n");
    for(int i = 0; i < 10; i++)
    {
        int * a = malloc(sizeof(int));
        *a = 7 * (i+1);
        
        insert_list_item(new_list, a, 0);
        
    }

    for(int i = 0; i < 10; i++)
    {
        int * a = malloc(sizeof(int));
        *a = 6 * (i+1);
        
        insert_list_item(new_list, a, -1);
        
    }

    for(int i = 0; i < 10; i++)
    {
        int * a = malloc(sizeof(int));
        *a = 3 * (i+1);
        
        insert_list_item(new_list, a, 5);
        
    }

    display_list(new_list, 0);


    // display_list(new_list, 0);
    int position[5] = {21,19,18, 2, 27};

    void * node;
    for(int i = 0; i < 5; i++)
    {
        if(!remove_list_item(new_list, &node, position[i]))
        {
            printf("data : %d\n", *(int *)node);
            free(node);
        }
    }


    display_list(new_list, 0);
    
    printf("free the memory\n");
    DESTORY_LIST(new_list);

    return EXIT_SUCCESS;
}