/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 6
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/
// add various libraries
#include <stdbool.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "dictionary.h"

/**
 * Returns true if word is in dictionary else false.
 */
 
// declares node globally
typedef struct node
{
    bool is_word;
    struct node* children[27];
}
node;

// initializes root node
node* root;

// declares counter for dictionary words
int words_loaded = 0;

bool check(const char* word)
{
    // declare current node for cursor
    node* current = root;
    
    // for every character in word
    for (int i = 0; i < strlen(word); i++)
    {
        char character = word[i];
        
        // when the character is an apostrophe
        if (character == '\'')
        {
            // set character one greater than alphabet
            character = 'z' + 1;
        }
        
        // assigns character to corresponding child index
        int index = tolower(character) - 'a';
        
        // return false if node is not found
        if (current->children[index] == NULL) 
        {
            return false;
        }    
        
        // else, move through the trie
        current = current->children[index];
    }
    
    // if word is found, return true
    if (current->is_word)
    {
        return true;
    }    
    
    // else, return false
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // try to open dictionary
    FILE* fp = fopen(dictionary, "r");
    if (fp == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }
    
    // allocate root to heap
    root = malloc(sizeof(node));
    
    // declare current node for cursor
    node* current = NULL;
    
    int character;
        
    // for each word in dictionary
    while(fgetc(fp) != EOF)
    {
       // set cursor back one character
       fseek(fp, -1, SEEK_CUR);
       
       // sets cursor to root node
       current = root;
       
       // read each character in a given word
       for (character = fgetc(fp); character != '\n'; 
                character = fgetc(fp)) 
       {           
           // when the character is an apostrophe
           if (character == '\'')
           {
               // set character one greater than alphabet
               character = 'z' + 1;
           }
        
           // if the current character is not found in the trie
           if (current->children[character - 'a'] == NULL)
           {
               // malloc a new node
               current->children[character - 'a'] = malloc(sizeof(node));

               // go to the new node
               current = current->children[character - 'a'];
           }
           else
           {
               // go to the node for that character
               current = current->children[character - 'a'];
           }
       }
       
       // declare end of word
       current->is_word = true;
       
       // increment word count
       words_loaded++;     
    }
    
    // close file
    fclose(fp);
    
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return words_loaded;
}
// recusive function to free node and it's descendants
bool freeTree(node* trie)
{
    // free each of the children and its descendants
    for(int i = 0; i < 27; i++)
    {
        if(trie-> children[i] != NULL) 
        {
            freeTree(trie->children[i]);
        }      
	}

    // then free current node
    free(trie);
    return true;
}


/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    // calls freeTree to free our dictionary
    return freeTree(root);
}
