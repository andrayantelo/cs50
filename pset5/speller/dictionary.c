/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>

#include "dictionary.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Arbitrary number of buckets for hash table
#define HASH_SIZE 10000
#define CHAR_AMOUNT 27

/* Returns hash for word*/
int hash(const char *word) {
    int index = 0;
    int i;
    for (i = 0; i < strlen(word); i++) {
        index += tolower(word[i]);
    }
    index = index % HASH_SIZE;
    return index;  
};

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
    // Traverse the linked list in the hash table to find our word.
    // Head of the linked list in the bucket we want to look through
    /*node *head = hashtable[hash(word)];
    
    node *cursor = head;
    
    while(cursor != NULL) {
        
        int comp = strcasecmp(cursor -> word, word);
        
        // if they match
        if (comp == 0) {
            // word is spelled correctly (in dictionary)
            return true;
        }
        // if they don't match
        else {
            // check next word
            cursor = cursor -> next;
        }
    }
    // If we are out here, that means we did not find the word or it
    */// was misspelled.
    return false;
}

/* Finds a letter's place in the alphabet */
int letter_index(char letter) {
    int letter_index;
    if (isupper(letter)) {
        letter_index = letter % 65;
    }
    else if (islower(letter)) {
        letter_index = letter % 97;
    }
    else {
        // we have another char
        letter_index = 27;
    }
    return letter_index;         
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
    
    // initiate root
    root = calloc(1, sizeof(node));
    // cursor
    struct node *cursor;
    cursor = root;
    int i;
    
    while (fscanf(dic_file, "%s", word) != EOF) {
    
        for (i = 0; i < strlen(word); i++) {
            // get the letter's index in the alphabet
            int index = letter_index(word[i]);
            
            cursor = cursor -> children[index];
            
            // each element in children corresponds to
            // a different letter
            // check the value at children[i]
            if (cursor == NULL) {
                // if NULL, malloc a new node, have 
                // children[i] point to it
                node *new_node = malloc(sizeof(node));
                // TODO these nodes need to be freed
                cursor = new_node;
            }
            // if not null, move to new node and continue

        } // end of for loop
        // if we have left the for loop, cursor should be pointing at the last letter of word
        // set is_word to true
        cursor -> is_word = true;
    }
    
    // free cursor
    free(cursor);
    // close dictionary file
    fclose(dic_file);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // Check if the hashtable has anything in it
    /*int i;
    int wordCount = 0;
    for (i = 0; i < HASH_SIZE; i++) {
        node *head = hashtable[i];
        if (head == NULL) {
            continue;
        }
        else {
            // traverse linked list and count the words
            node *cursor = head;
            while (cursor != NULL) {
                wordCount++;
                cursor = cursor -> next;
            }
        }
    }
    if (wordCount > 0) {
        return wordCount;
    }*/
    return 0;
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
 
        free_node(&(*n)->children[i]);
    }
    free(n);
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
    
    //node *cursor = root;
    //free_node(&cursor);
    return true;
}



