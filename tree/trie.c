#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <trie.h>

int is_node_leaf(trie_node_t * node)
{
    if(node->is_end)
    {
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}

int is_end_of_word(trie_node_t * node)
{   // check if nletter[0] != NULL
        // return true;

    return 0;
}

int create_node(trie_node_t ** new, int level)
{
    *new = malloc(sizeof(trie_node_t));
    if(NULL == new)
    {
        return EXIT_FAILURE;
    }

    (*new)->popularity = -1;
    (*new)->level = level;
    (*new)->is_end = 0;

    int i;
    for(i = 0; i < NUM_LETTERS; i++)
    {
        (*new)->nletter[i] = NULL;
    }

    return EXIT_SUCCESS;
}

void destory_node(trie_node_t * node)
{
    if(node)
    {
        free(node);
    }
}

int trie_create(trie_node_t ** root)
{
    if(create_node(root, 0))
    {
        // setting errno?
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void trie_destroy(trie_node_t * root)
{
    int idx;
    for(idx = 0; idx < NUM_LETTERS; idx++)
    {
        if(root->nletter[idx] != NULL)
        {
            // printf("looking to destroy each letter branch:[%c]\n", 'a'+ idx);
            trie_destroy(root->nletter[idx]);
            
        }
    }
    destory_node(root);

}

int trie_insert_r(trie_node_t * root, char * str, int level)
{
    // printf("inserting [%c]\n", *str);
    if(*str == '\0')
    {
        root->is_end = 1;
        root->level = level;
        return EXIT_SUCCESS;
    }

    // Insert current letter in the string to node in the tree
    int idx = *str - 'a';
    // printf("insert at index [%d]\n", idx);
    if(NULL == root->nletter[idx])
    {
        // printf("CREATING node for [%c]\n", *str);
        if(create_node(&root->nletter[idx], level))
        {
            return EXIT_FAILURE;
        }
    }
    // else
    // {
    //     printf("node already exists for [%c]\n", *str);
    // }
    
    // Insert next letter into the
    // printf("insert next char [%c]\n", *(str+1));
    trie_insert_r(root->nletter[idx], ++str, level++);
}


#if 0
int trie_insert(trie_node_t * root, char * str)
{
    int level; 
    int length = strlen(str);
    trie_node_t * curr = root;

    for(level = 0; level < length; level++)
    {
        int idx = str[level] -'a';

        if(NULL == curr->nletter[idx])
        {
            if(create_node(&root->nletter[idx], level))
            {
                return EXIT_FAILURE;
            }
        }
        curr = curr->nletter[idx];
    }
    
    curr->is_end = 1;
}
#endif

void trie_display_r(trie_node_t * root, char * str, int level)
{
    if(root->is_end)
    {
        str[level] = '\0';
        printf("%s\n", str);
    }

    int i;
    for(i = 0; i < NUM_LETTERS; i++)
    {
        if(root->nletter[i])
        {
            str[level] = i + 'a';
            trie_display_r(root->nletter[i], str, level + 1);
        }
    }
}
