#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <word_list.h>
#include <module.h>

#define NDEBUG 



int main(void)
{

    // Replace trie create with constructor that builds trie from dictionary to allow building a wordlist
    // if(word_list_trie_create())
    // {
    //     fprintf(stderr, "failed to allocate trie\n");
    //     return -1;
    // }
    // 1. Get a file to create a list of words to parse

    char * buff = NULL;
    ssize_t bufflen = 0;
    word_list_t * wl = NULL;

    // Go functions 
    GoString str;
    str.n = strlen("From the Go Module");
    str.p = "From the Go Module";
    Print(str);

#if 1
    // 2. Insert word into a trie DS
    word_list_trie_insert("a");
    word_list_trie_insert("ale");
    word_list_trie_insert("all");
    word_list_trie_insert("available");
    word_list_trie_insert("bright");
    word_list_trie_insert("billow");
    word_list_trie_insert("zebra");
    word_list_trie_insert("zillow");
    word_list_trie_insert("alright");
    word_list_trie_insert("allow");
    word_list_trie_insert("octopus");
    word_list_trie_insert("zanzibar");

    printf("remove [all]\n");
    word_list_trie_remove("all");
    word_list_trie_remove("bouy");

    printf("finished input\n");
    word_list_trie_display();

    // 3. Get a prefix string
    printf("enter prefix: > ");
    if(getline(&buff, &bufflen, stdin) > 0)
    {
        // 2. Insert word into a trie DS
        buff[strlen(buff) - 1] = '\0';
        printf("prefix: [%s]\n", buff);

    }

    // LOOP
    if(word_list_create(&wl, buff, 10) == 0 )
    {
        printf("creating wordlist\n");
        // 4. Get list of strings for user
        word_list_build(wl);
    }
        
    printf("word_list built -> \n");

    for(int i = 0; i < wl->current; i++)
    {
        printf("%s\n", wl->word_list_array[i]);
    }


        // 5. Get a response for picked string,
        // 6. Update popularity value of string
    word_list_destroy(wl);
#endif

    // word_list_trie_destroy();
    free(buff);
    return 0;
}