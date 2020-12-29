// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include "dictionary.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>
// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}node;

// Number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];
int count = 0;
bool  loaded = false;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    unsigned int hashvalue = hash(word);
    node *cursor;
    cursor = table[hashvalue];
    while (cursor != NULL)
    {
        //see if the words match
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
        else
        {
          cursor = cursor->next;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO
    unsigned int hashValue = 0;
    for(int i = 0; i < strlen(word); i++)
    {
        hashValue = (hashValue << 2)^ tolower(word[i]);
    }

    return hashValue % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // TODO
    

    // open file and read
    FILE *fp = fopen(dictionary,"r");
    node *head = NULL;
    // check if loaded file
    if(fp == NULL)
    {
        return false;

    }
    //set char area to size 45 + 1
    char word[LENGTH + 1];

     //while not at the end file go through dictionary
     while (fscanf(fp,"%s",word) != EOF)
     {
         //allocate memory
         node *n = malloc(sizeof(node));
         if(n == NULL)
         {
             return false;
         }
         //copy word into linked list
         strncpy(n->word, word, sizeof(word));

        // implement hash function to get the index
        int index = hash(word);

        // if the corresponding index in hashtable is empty, assign it to the temp node
        if (table[index] == NULL)
        {
            head = table[index];
            table[index] = n;
            n->next = NULL;
        }
        else
        {
            n->next = table[index];
            table[index] = n;
        }
        count ++;
    }


    fclose(fp);
    loaded = true;
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{

     if (loaded)
    {
         return count;
    }

    return 0;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        node *traversal_pointer = table[i]; 
        while (traversal_pointer != NULL)
        {
            node *temp = traversal_pointer;
            traversal_pointer = traversal_pointer->next; // point 
        //traversal_pointer to next node on the list
            free(temp);
        }
    }
    loaded = false;
    return true;
}



