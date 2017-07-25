/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>

#include "dictionary.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>

// Arbitrary number of buckets for hash table
#define HASH_SIZE 10000
#define CHAR_AMOUNT 27

int letter_index(char letter);

int wordCount;
bool loaded = false;

// Create the node data type for the hash table
// each element of the hash table array is a node pointer
typedef struct node {
    bool is_word;
    struct node *children[CHAR_AMOUNT];
} node;

// Root of trie
node *root;


/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // for each letter in input word
      // go to corresponding element in children
        // if NULL, word is mispelled
        // if not NULL, move to next letter
      // once at end of input word
        // check if is_word true
    
    node *cursor = root;
    int i;
    
    for (i = 0; i < strlen(word); i++) {
        int index = letter_index(word[i]);
        if (cursor -> children[index] == NULL) {
            return false; 
        }
        else {
            cursor = cursor -> children[index];
        }
    }
    return cursor -> is_word;
}

/* Finds a letter's place in the alphabet */
int letter_index(char letter) {
    int letter_index;
    if (isupper(letter)) {
        letter_index = letter - 'A';
    }
    else if (islower(letter)) {
        letter_index = letter - 'a';
    }
    else {
        // we have another char
        letter_index = 26;
    }
    
    assert(letter_index >= 0 && letter_index < CHAR_AMOUNT);
    return letter_index;         
}

void insert_word(node *n, char *word) {
    // Takes a pointer to a node and a string and it inserts
    // the string into trie starting at that node
    int i;
    for (i = 0; i < strlen(word); i++) {
        // get the letter's index in the alphabet
        int index = letter_index(word[i]);

        // each element in children corresponds to
        // a different letter
        // check the value at children[i]
        if (n -> children[index] == NULL) {
            // if NULL, malloc a new node, have 
            // children[i] point to it
            n -> children[index] = malloc(sizeof(node));
        }
        // if not null, move to new node and continue
        n = n -> children[index];

    } // end of for loop
    // if we have left the for loop, cursor should be pointing at the last letter of word
    // set is_word to true
    n -> is_word = true;   
    
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // Loads the dictionary into a data structure that we have created
    
    // Trie version
    
    // Open the dictionary file
    FILE *dic_file = fopen(dictionary, "r");
    
    // Check that it opened correctly
    if (dic_file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", dictionary);
        return false;
    }
    
    char word[LENGTH + 1] = {0};
    
    // initialze root, do I have to initiate root TODO
    root = calloc(1, sizeof(node));
    // check it worked
    if (root == NULL) {
        return false;
    }
    // initialize wordCount
    wordCount = 0;
    
    while (fscanf(dic_file, "%s", word) != EOF) {
        wordCount++;
        insert_word(root, word);

    }

    // close dictionary file
    fclose(dic_file);
    loaded = true;
    return true;
}

/** Returns number of words found in *node n
unsigned int find_words(node *n) {
    int i;
    for (i = 0; i < CHAR_AMOUNT; i++) {
        if (n -> )
    }
}*/

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // Check if the trie has anything in it
    return loaded ? wordCount : 0;
}

/** Frees nodes **/
// have some function free_node(node* n)
// and it loops over the children array and calls free_node on everything 
// non_null and then sets it to null, then frees n and returns

void free_node(node **n) {
    // it takes a node pointer and calls free_node on all the node pointers
    // in that node's children array, then it frees the node pointer
    if (n == NULL || *n == NULL) {
        return;
    }
    
    // traverse through n's children array;
    int i;
    for (i = 0; i < CHAR_AMOUNT; i++) {
        free_node(&((*n)->children[i]));
    }
    if (*n != NULL) {
        free(*n);
    }
    // set the pointer equal to NULL so that we 
    // don't have pointers pointing to random stuff
    *n = NULL;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // Frees the dictionary from memory
    
    free_node(&root);
    
    loaded = false;
    return true;
}



