/* Trie version of speller */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define DICTIONARY "dictionaries/large"
#define LENGTH 45
#define NUM_CHAR 27
    
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


// TODO have a pointer to the root? Have root be a pointer
// to the pointer of the first node? why though?
node *root;

/* Returns the index of the letter in the alphabet, must be case
insensitive */
int alpha_index(char c) {
    if (c == '\'') {
        return 26;
    }
    else if (islower(c)) {
        return c - 97;
    }
    else {
        return c - 65;
    }
}

void insert_word(node *n, char *word) {
    /* insert letter into the trie n */
    int i;
    for (i = 0; i < strlen(word); i++) {
        int letter_index = alpha_index(word[i]);
        if (n -> children[letter_index] == NULL) {
            node *new_node = calloc(1, sizeof(node));
            n -> children[letter_index] = new_node;
        }
        n = n -> children[letter_index];
    }
    n -> is_word = true;
    wordCount++;
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
    
    if (root == NULL) {
        fprintf(stderr, "Not enough memory.\n");
        return 1;
    }

    node *cursor = root;
    char word[LENGTH + 1] = {0};
    wordCount = 0;
    
    // read dictionary word per word
    // insert each word letter by letter into trie
    while ((fscanf(dic_file, "%s", word)) != EOF) {
        insert_word(root, word);
    }
    
    // close dictionary file
    fclose(dic_file);
    loaded = true;
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

/* Returns the number of words in the dictionary */
int size(void) {
    if (!loaded) {
        fprintf(stderr, "Dictionary is not loaded.\n");
        return 1;
    }
    return wordCount;
}

void free_node(node **n) {
    // free the nodes in the array of nodes n 
    // pointer to the pointer n because we want to free n at the
    // end too
    if (n == NULL || *n == NULL) {
        return;
    }
    // travel down trie
    int i;
    for (i = 0; i < NUM_CHAR; i++) {
        if (((*n) -> children)[i] != NULL) {
            free_node(&((*n) -> children[i]));
        }
    }
    free(*n);
    *n = NULL;
}

/* Unloads the dictionary, returns true if successful */
int unload(void) {
    // traverse the trie freeing nodes. Go down to the deepest level and start // freeing there
    
    free_node(&root);
    
    loaded = false;
    return true;
    
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
    
    // Open text
    FILE *fp = fopen(text, "r");
    
    if (fp == NULL) {
        fprintf(stderr, "Could not open %s.\n", text);
        return 1;
    }
    
    load(dictionary);
    
    // Go through each word in text, and check if that word
    // is in the dictionary
    char word[LENGTH + 1];
    int misspellings = 0;
    int wordCount = 0;
    int index = 0;
    char c;
    for (c = fgetc(fp); c != EOF; c = fgetc(fp)) {
        // Check c is alphabetical or '\''
        if (isalpha(c) || (c == '\'' && index > 0)) {
            word[index] = c;
            // check that the word isn't too long
            if (index > LENGTH) {
                // eat word, reset index
                while((c = fgetc(fp) != EOF) && isalpha(c));
                index = 0;
            }
            index++;
        }
        // check if c is a digit
        else if (isdigit(c)) {
            while((c = fgetc(fp) != EOF) && isalnum(c));
            index = 0;
        }
        // if index > 0, and c is neither a digit or letter, we must
        // be at the end of a word
        else if (index > 0) {
            // finish word with \0
            word[index] = '\0';
            //  Increment wordCount
            wordCount++;
            
            // check if word is in dictionary
            bool misspelled = !check(word);
            
            if (misspelled) {
                printf("%s\n", word);
                misspellings++;
            }
            // reset index for next word
            index = 0;
        }
    }
    // print out dict size, text word count, and number of
    // misspelled words
    int dicSize = size();
    printf("TEXT WORD COUNT: %d\n", wordCount);
    printf("MISSPELLED: %d\n", misspellings);
    printf("DICTIONARY SIZE: %d\n", dicSize);
    
    unload();
    fclose(fp);
    return 0;
}