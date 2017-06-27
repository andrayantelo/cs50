/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>

#include "dictionary.h"

#include <stdlib.h>
#include <string.h>

int hash(char *word);
// Arbitrary number of buckets for hash table
#define HASH_SIZE 100;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // TODO
    // Must be case-sensitive
    // Checks the dictionary data structure that we created to see if given word
    // is in the dictionary
    return false;
}


// Hash function for load
int hash(char *word) {
    
    int i;
    int word_length = 0;
    
    for (i = 0; i < strlen(word); i++) {
        printf("hello");
    }
    return num % HASH_SIZE;  
};
/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // TODO
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
    
    // Create the node data type for the hash table
    // each element of the hash table array is a node pointer
    typedef struct node {
        char word[LENGTH + 1];
        struct node *next;
    } node;
    
    // Declare the hash table
    node *hashtable[HASH_SIZE];
    
    
    
    // close dictionary file
    fclose(dictionary);
    
    return false;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // TODO
    return 0;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // TODO
    // Frees the dictionary from memory
    return false;
}
