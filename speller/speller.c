/* Hashtable version */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define HASH_SIZE 10000
#define LENGTH 45
#define DICTIONARY "dictionaries/large"

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
      h = T[(x[0] + j) % 256];
      for (i = 1; i < len; ++i) {
         h = T[h ^ tolower(x[i])];
      }
      hh[j] = h;
    }
    
    unsigned int value = (*(unsigned int*)hh)%HASH_SIZE;
    return value;
}

// Define the node type
typedef struct node {
    char word[LENGTH + 1];
    struct node* next;
} node;

// Declare hashtable variable
// hashtable is going to be an array of node pointers
// hashtable will be initialized in the load function
node *hashtable[HASH_SIZE] = {NULL};

// Has the dictionary been loaded or not
bool loaded = false;

int check(const char *word) {
    /* Checks if word is in our dictionary, returns true if it is
      false otherwise */
    // fail if dictionary not loaded
    if (!loaded) {
        return false;
    }
    // traverse the dictionary looking for this word
    // return true when found
    
    node *trav;
    int i;
    for (i = 0; i < HASH_SIZE; i++) {
        trav = hashtable[i];
        while (trav != NULL) {
            if (strcasecmp(trav -> word, word) == 0) {
                return true;
            }
            else {
                trav = trav -> next;
            }
        }
    }
    return false;
}

int load(const char* dic) {
    /* Loads dictionary into data structure, returns true if successful
    dic is the name of the dictionary file*/
    
    // open the dictionary file
    FILE *dic_file = fopen(dic, "r");

    // check that it opened correctly, if not exit program
    if (dic_file == NULL) {
        fprintf(stderr, "Could not open %s file.\n", dic);
        return false;
    }
    
    // declare word variable
    char word[LENGTH + 1];
    
    while(fscanf(dic_file, "%s", word) != EOF) {
        // find the hash for the word
        unsigned int hash_index = Pearson16(word, strlen(word));
        
        // malloc some memory for new node
        node *new_node = malloc(sizeof(node));
        
        // fail if we ran out of memory
        if (new_node == NULL) {
            return false;
        }
                                
        // update new_node's word and next pointer
        strcpy(new_node -> word, word);
        new_node -> next = NULL;
        
        // place word into hashtable
        // check if there ia a node in hashtable[hash_index]
        if (hashtable[hash_index] == NULL) {
            // there is no linked list here yet
            // start a linked list
            hashtable[hash_index] = new_node;
        }
                                
        // if there is a linked list there already
        // then we need to insert the new node in there as the
        // new head of the linked list
        else {
            // remember old head of linked list
            node *old_head = hashtable[hash_index];
            // point the new node's next pointer to the old head
            // of the linked list
            new_node -> next = old_head;
            // make the new node the new head of the linked list
            hashtable[hash_index] = new_node;
        }
    }
        
    // close the dictionary file
    fclose(dic_file);
    loaded = true;
    return true;
}

void print_hashtable(node *table[]) {
    // loop through hash table and print out a visual representation of 
    // the table
    
    int i;
    
    for (i = 0; i < HASH_SIZE; i++) {
        if (table[i] == NULL) {
            printf(".\n");
        }
        else {
            // loop through the linked list and
            // print out each word 
            node *cursor = table[i];
            while (cursor != NULL) {
                printf("%c -> ", (cursor -> word)[0]);
                cursor = cursor -> next;
            }
            printf("\n");
        }
    }
}

int unload(void) {
    /* unloads dictionary, frees all malloced memory, returns true if successful */
    
    if (!loaded) {
        return false;
    }
    // Traverse through hashtable and free all the nodes
    node *trav;
    
    int i;
    for (i = 0; i < HASH_SIZE; i++) {
        trav = hashtable[i];
        while (trav != NULL) {
            free(trav);
            trav = trav -> next;
        }
    }
    loaded = false;
    return true;
}

int size(void) {
    /* returns size of dictionary*/
    // traverse dictionary and count number of words
    if (!loaded) {
        return false;
    }
    int wordCount = 0;
    node *trav;
    
    int i;
    for (i = 0; i < HASH_SIZE; i++) {
        trav = hashtable[i];
        while(trav != NULL) {
            wordCount++;
            trav = trav -> next;
        }
    }
    
    return wordCount;
}


int main(int argc, char *argv[]) {
    
    // Ensure that you have the correct number of arguments
    if (argc != 2 && argc != 3) {
        fprintf(stderr, "Correct usage: ./speller dictionary text\n");
        return 1;
    }
    
    // remember dictionary
    char* dictionary = (argc == 3) ? argv[1]: DICTIONARY;
    
    printf("using dictionary: %s\n", dictionary);
    
    // remember text file
    char* text_file = (argc == 3) ? argv[2] : argv[1];
    
    load(dictionary);
    
    // open text file
    FILE *text = fopen(text_file, "r");
    
    // check that it opened
    if (text == NULL) {
        fprintf(stderr, "Could not open %s\n", text_file);
        unload();
        return 1;
    }
    
    // go through each word and check the spelling
    int misspellings = 0;
    int text_word_count = 0;
    char word[LENGTH + 1];
    // which letter are we on in the word
    int index;
    
    int c;
    for (c = fgetc(text); c != EOF; c = fgetc(text)) {
        // allow only alphabetical chars
        // can't start with apostrophe
        if (isalpha(c) || (c == '\'' && index > 0)) {
            // append char to word
            word[index] = c;
            index++;
            
            // ignore words longer than LENGTH
            if (index > LENGTH) {
                // eat rest of word
                while ((c = fgetc(text)) != EOF && isalpha(c));
                // reset index
                index = 0;
            }
        }
        // ignore numeric characters
        else if (isdigit(c)) {
            // eat the rest of the word
            while ((c = fgetc(text)) != EOF && isalnum(c));
            
            // reset index
            index = 0;
        }
        // if we found a whole word
        else if (index > 0) {
            // update text word count
            text_word_count++;
            // terminate the current word
            word[index] = '\0';
            
            // check spelling
            // if it's misspelled you get a 0
            // if it's spelled correctly you get 1
            bool misspelled = !check(word);
            
            if (misspelled) {
                printf("%s\n", word);
                misspellings++;
            }
            // reset index
            index = 0;
        }
        
    }
    
    if (ferror(text)) {
        fclose(text);
        printf("Error reading %s.\n", text_file);
        unload();
        return 1;
    }
    
    fclose(text);
    
    printf("Number of misspelled words: %d\n", misspellings);
    printf("Number of words in text: %d\n", text_word_count);
    printf("Number of words in dictionary: %d\n", size());

    bool unloaded = unload();
    if (!unloaded) {
        printf("could not unload %s\n", dictionary);
        return 1;
    }
    
    // TODO be able to go through a text file, take out the words 
    // that have only the alphabet or apostrophes (but don't start with
    // apostrophes), and have each one of them checked for spelling
    // then print out how many were misspelled
    
    // TODO
    // print out words in text, words in dictionary as well
    return 0;
}