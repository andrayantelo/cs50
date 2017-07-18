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

// Arbitrary number of buckets for hash table
#define HASH_SIZE 10000

const int MOD_ADLER = 65521;

// Hash function found on the internet 
// https://en.wikipedia.org/wiki/Adler-32
uint32_t adler32(const char *data, size_t len) 
/* where data is the location of the data in physical memory and 
   len is the length of the data in bytes */
{
    uint32_t a = 1, b = 0;
    size_t index;
    
    /* Process each byte of the data in order */
    for (index = 0; index < len; ++index)
    {
        a = (a + data[index]) % MOD_ADLER;
        b = (b + a) % MOD_ADLER;
    }
    
    return ((b << 16) | a) % HASH_SIZE;
};

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
    char word[LENGTH + 1];
    struct node *next;
} node;

// hashtable will be a pointer to another pointer 
// in this case a pointer to an array of HASH_SIZE
// node pointers
typedef struct set_t {
    node **hashtable;
    int wordCount;
} set_t;

// Declare the dictionary
set_t dict;

bool loaded = false;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{  
    // Traverse the linked list in the hash table to find our word.
    // Head of the linked list in the bucket we want to look through
    node *head = (dict.hashtable)[hash(word)];
    
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
    // was misspelled.
    return false;
}


/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    loaded = true;
    // Loads the dictionary into a data structure that we have created
    
    // Hash table version
    
    // Open the dictionary file
    FILE *dic_file = fopen(dictionary, "r");
    
    // Check that it opened correctly
    if (dic_file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", dictionary);
        return false;
    }
    
    // dict -> hashtable is an uninitialized pointer
    // Make it point to a block of memory large enough // to hold HASH_SIZE addresses (node*) which point // to head nodes (the actual type) of linked lists
    // Here we are initializing the addresses to 
    // node head addresses to NULL
    dict.hashtable = calloc(HASH_SIZE, sizeof(node*));
    
    // initialize wordCount to zero
    dict.wordCount = 0;
    
    // For each word in the dictionary, hash it into the
    // hash table
    char word[LENGTH + 1] = {0};
    
    while (fscanf(dic_file, "%s", word) != EOF) {
        // Increment wordCount
        dict.wordCount++;
        
        // make new node for word
        // this is an address to a node
        node *new_node = malloc(sizeof(node));
        
        // Abort if we run out of space
        if (new_node == NULL) {
            unload();
            return false;
        }

        // Initialize next pointer
        new_node -> next = NULL;
        // Copy word into node
        strcpy(new_node -> word, word);
        
        // get word's hash number
        int index = hash(new_node -> word);
        // If there isn't a linked list in that index's bucket yet
        // insert the node to be the head of that particular linked list
        if ((dict.hashtable)[index] == NULL) {
            (dict.hashtable)[index] = new_node;
        }
        else {
            // Add the new_node to the linked list that is already there
            // tne new_node will become the new HEAD
            node *old_head = (dict.hashtable)[index];
            new_node -> next = old_head;
            (dict.hashtable)[index] = new_node;
        }
        
    }
    
    // free temp_hashtable? TODO
    
    // close dictionary file
    fclose(dic_file);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    if (!loaded) {
        return 0;
    }
    return dict.wordCount;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // Frees the dictionary from memory
    int i;
    for (i = 0; i < HASH_SIZE; i++) {
        node *cursor = (dict.hashtable)[i];
        while (cursor != NULL) {
            node *temp = cursor;
            cursor = cursor -> next;
            free(temp);
        }
    }
    // free dict.hashtable
    free(dict.hashtable);
    return true;
}
