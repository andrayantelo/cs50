/* Trie version of speller */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define DICTIONARY "dictionaries/large"
#define LENGTH 45
#define NUM_CHAR
    
int load(char *dictionary);
int unload(void);
int size(void);
int check(char *word);

int wordCount;
bool loaded;

typedef struct node {
    bool is_word;
    struct node *children[NUM_CHAR];
} node;

node *root;

int load(char *dictionary) {
    // open dictionary
    FILE* dic_file = fopen(dictionary, "r");
    
    if (dic_file == NULL) {
        fprintf(stderr, "Could not open %s.\n", dictionary);
        return 1;
    }
    
    // Initialize root node
    root = calloc(1, sizeof(node));
    return 0;
}

int main(int argc, char *argv[]) {
    // Make sure enough arguments were given
    if (argc != 2 && argc !=3) {
        fprintf(stderr, "Correct usage: ./speller dictionary text\n");
        return 1;
    }
    
    // Remember dictionary
    char *dictionary = (argc == 3) ? argv[1]: DICTIONARY;
    
    // Remember text
    char *text = (argc == 3) ? argv[2]: argv[1];
    
    load(dictionary);
    return 0;
}