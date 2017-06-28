/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>

#include "dictionary.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Arbitrary number of buckets for hash table
#define HASH_SIZE 10

int hash(char *word);

// Create the node data type for the hash table
// each element of the hash table array is a node pointer
typedef struct node {
    char word[LENGTH + 1];
    struct node *next;
} node;

// Declare the hash table
node *hashtable[HASH_SIZE] = {NULL};

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
    int index = 0;
    int i;
    for (i = 0; i < strlen(word); i++) {
        index += word[i];
    }
    index = index % HASH_SIZE;
    printf("index: %d\n", index);
    return index;  
};
/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    printf("Running Load\n");
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
    
    // For each word in the dictionary, hash it into the
    // hash table
    
    // TODO how to declare variable word
    
    char *word = malloc(LENGTH + 1);
    while (fscanf(dic_file, "%s", word) != EOF) {
        
        // make new node for word
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
        if (hashtable[index] == NULL) {
            hashtable[index] = new_node;
        }
        else {
            // Add the new_node to the linked list that is already there
            // tne new_node will become the new HEAD
            node *old_head = hashtable[index];
            new_node -> next = old_head;
            hashtable[index] = new_node;
        }
        
    }
    
    // close dictionary file
    fclose(dic_file);
    return true;
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
    int i;
    for (i = 0; i < HASH_SIZE; i++) {
        printf("inside hashtable for loop\n");
        node *cursor = hashtable[i];
        printf("the first element in hashtable index %d is %p\n", i, cursor);
        
        while (cursor != NULL) {
            printf("inside while loop\n");
            node *temp = cursor;
            cursor = cursor -> next;
            //free(temp -> word);
            free(temp -> next);
            free(temp);
        }
    }
    return true;
}
