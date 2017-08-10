/* Trie version of speller */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

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

/* Returns the index of the letter in the alphabet, must be case
insensitive */
int alpha_index(char c) {
    if (c == '\'') {
        return 27;
    }
    else if (islower(c)) {
        return c - 97;
    }
    else {
        return c - 65;
    }
}

int load(char *dictionary) {
    // open dictionary
    FILE* dic_file = fopen(dictionary, "r");
    
    if (dic_file == NULL) {
        fprintf(stderr, "Could not open %s.\n", dictionary);
        return 1;
    }
    
    // Initialize root node
    root = calloc(1, sizeof(node));
    
    node *cursor = calloc(1, sizeof(node));
    cursor = root;
    
    char word[LENGTH + 1] = {0};
    int i;
    int letter_index;
    
    // read dictionary word per word
    // insert each word letter by letter into trie
    while ((fscanf(dic_file, "%s", word)) != EOF) {
        for (i = 0; i < strlen(word); i++) {
            letter_index = alpha_index(word[i]);
            node *new_node = calloc(1, sizeof(node));
            
            if (cursor -> children[letter_index] == NULL) {
                cursor -> children[letter_index] = new_node;
            }
            cursor = cursor -> children[letter_index];
        }
        cursor -> is_word = true;
    }
    return true;
}

/* Returns true if word is in the trie (dictionary)*/
int check(char *word) {
    // Go through trie and see if word is in there
    node *cursor = root;
    
    int i;
    for (i = 0; i < strlen(word); i++) {
        int letter_index = alpha_index(word[i]);
        if (cursor -> children[letter_index] == NULL) {
            return false;
        }
        else {
            cursor = cursor -> children[letter_index];
        }
    }
    return (cursor -> is_word) ? true: false;
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