/* Hashtable struct version of speller */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int size(void);
int load(char *dictionary);
int unload(void);
int check(char *word);
unsigned int Pearson16(char *word, size_t len);

#define LENGTH 45
#define HASH_SIZE 10000
#define DICTIONARY "dictionaries/large"
bool loaded = false;

unsigned int Pearson16(char *word, size_t len) {
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
      h = T[(tolower(word[0]) + j) % 256];
      for (i = 1; i < len; ++i) {
         h = T[h ^ tolower(word[i])];
      }
      hh[j] = h;
    }
    // hh is an unsigned char array
    // hh is a pointer
    // HASH_SIZE is an int
    // they need to be of the same type to be able to use %
    // so first, we convert the hh pointer to unsigned int
    // and then we dereference the whole thing
    // value is an unsigned int
    
    unsigned int value = *((unsigned int*) hh) %HASH_SIZE;
    return value;
    
}

// Define the node type
typedef struct node {
    char word[LENGTH + 1];
    struct node *next;
} node;

// Define the hashtable struct type
typedef struct set_t {
    node *hashtable[HASH_SIZE];
    int wordCount;
} set_t;

// The dictionary will be a global var and available to all functions
set_t *dictionary;

/* Creates the dictionary data structure, returns true if successful */
int load(char *dictionary_name) {
    
    // Load dictionary file
    FILE* dic_file = fopen(dictionary_name, "r");
    
    // Check that it opened properly
    if (dic_file == NULL) {
        fprintf(stderr, "Could not open %s.\n", dictionary_name);
        return false;
    }
    
    // Initialize dictionary set
    dictionary = calloc(1, sizeof(set_t));
    
    
    char word[LENGTH + 1] = {0};
    
    // Go through words in dictionary and put them in 
    // the dictionary's hashtable
    while(fscanf(dic_file, "%s", word) != EOF) {
        // Get the index for the word using hash function
        unsigned int hash_index = Pearson16(word, strlen(word));
        
        //declare new node
        node *new_node = calloc(1, sizeof(node));

        strcpy(new_node -> word, word);
        
        // check the hashtable at hash_index and see if
        // there is already a node there, if not put our new one there
        // otherwise add it to the linked list that is already there
        if (dictionary -> hashtable[hash_index] == NULL) {
            dictionary -> hashtable[hash_index] = new_node;
        }
        else {
            node *old_head = dictionary -> hashtable[hash_index];
            new_node -> next = old_head;
            dictionary -> hashtable[hash_index] = new_node;
        }
        dictionary -> wordCount++;
    }
    // close file
    fclose(dic_file);
    
    loaded = true;
    return true;
    
}

/* Returns the size of the dictionary (number of words) 
if the dictionary is loaded, otherwise it returns false */

int size(void) {
    if (loaded) {
        return dictionary -> wordCount;
    }
    return false;
}

/* Frees all of the malloced memory that happened in the load function,
returns true if successful */

int unload(void) {
    if (!loaded) {
        return false;
    }
    
    int i;
    // Traverse the hashtable and free nodes
    node *trav;
    
    for (i = 0; i < HASH_SIZE; i++) {
        // traverse through linked list dictionary -> hashtable[i]
        // dictionary -> hashtable[i] is a node pointer
        trav = dictionary -> hashtable[i];
        while (trav != NULL) {
            node *temp = trav;
            trav = trav -> next;
            free(temp);
        }
    }
    // free the dictionary as well
    free(dictionary);
    return true;
}

/* Checks to see if the word given is in the dictionary, returns true
if it is */

int check(char *word) {
    // find the linked list this word would be in
    int hash_index = Pearson16(word, strlen(word));
    
    // traverse that linked list, and see if the word is there
    int i;
    node *trav;
    
    trav = dictionary -> hashtable[hash_index];
    

int main(int argc, char *argv[]) {
    
    // Check that the correct number of arguments were given
    if (argc != 2 && argc != 3) {
        fprintf(stderr, "Correct usage: ./speller dictionary text\n");
        return 1;
    }
    
    // Remember the dictionary file if provided
    char *dictionary = (argc == 3) ? argv[1]: DICTIONARY;
    
    // Remember the text file
    char *text_file = (argc == 3) ? argv[2]: argv[1];
    
    // Create dictionary
    load(dictionary);
    
    // Unload dictionary
    unload();
    
    return 0;
}