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


// Hash function (slightly modified) from the wikipedia // article
/*
https://en.wikipedia.org/wiki/Pearson_hashing#C_implementation_to_generate_64-bit_.2816_hex_chars.29_hash
*/

unsigned int Pearson16(const char *x, size_t len) {
    
    size_t i;
    size_t j;
    unsigned char h;
    unsigned char hh[8];
    static const unsigned char T[256] = {
        // 0-255 shuffled in any (random) order suffices
        98,  6, 85,150, 36, 23,112,164,135,207,169,  5, 26, 64,165,219, //  1
        61, 20, 68, 89,130, 63, 52,102, 24,229,132,245, 80,216,195,115, //  2
        90,168,156,203,177,120,  2,190,188,  7,100,185,174,243,162, 10, //  3
        237, 18,253,225,  8,208,172,244,255,126,101, 79,145,235,228,121, //  4
        123,251, 67,250,161,  0,107, 97,241,111,181, 82,249, 33, 69, 55, //  5
        59,153, 29,  9,213,167, 84, 93, 30, 46, 94, 75,151,114, 73,222, //  6
        197, 96,210, 45, 16,227,248,202, 51,152,252,125, 81,206,215,186, //  7
        39,158,178,187,131,136,  1, 49, 50, 17,141, 91, 47,129, 60, 99, //  8
        154, 35, 86,171,105, 34, 38,200,147, 58, 77,118,173,246, 76,254, //  9
        133,232,196,144,198,124, 53,  4,108, 74,223,234,134,230,157,139, // 10
        189,205,199,128,176, 19,211,236,127,192,231, 70,233, 88,146, 44, // 11
        183,201, 22, 83, 13,214,116,109,159, 32, 95,226,140,220, 57, 12, // 12
        221, 31,209,182,143, 92,149,184,148, 62,113, 65, 37, 27,106,166, // 13
         3, 14,204, 72, 21, 41, 56, 66, 28,193, 40,217, 25, 54,179,117, // 14
        238, 87,240,155,180,170,242,212,191,163, 78,218,137,194,175,110, // 15
        43,119,224, 71,122,142, 42,160,104, 48,247,103, 15, 11,138,239  // 16
    };

    for (j = 0; j < 8; ++j) {
      h = T[(tolower(x[0]) + j) % 256];
      for (i = 1; i < len; ++i) {
         h = T[h ^ tolower(x[i])];
      }
      hh[j] = h;
    }

    unsigned int value = (*(unsigned int*)         hh)%HASH_SIZE;
    
    return value;
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
    node *head = (dict.hashtable)[Pearson16(word, strlen(word))];
    
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
        int index = Pearson16(new_node -> word, strlen(new_node -> word));
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
    loaded = false;
    return true;
}
