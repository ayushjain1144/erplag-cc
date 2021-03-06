//                  ,-.                         Group 08                                                    //
//          ,      ( {o\                        Satvik Golechha :: 2017A7PS0117P                           ///
//          {`"=,___) (`~                       Bharat Bhargava :: 2017A7PS0025P                          ////
//           \  ,_.-   )                        Ayush      Jain :: 2017A7PS0093P                         /////
//      ~^~^~^`- ~^ ~^ '~^~^~^~                                                                         //////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include<stdlib.h>          // malloc()
#include<string.h>          // strcmp(), strcpy()      
#include "hash.h"         

void initialize(ENTRY **hash_table)
{
    for (int i=0; i<SIZE; ++i)
    {
        hash_table[i] = NULL;
    }
    return;
}

// hash function: implementing 7vik's hash (unpublished, 2020)
int hash(char *s261)
{
    int n261 = 261;
    int h261 = 261;
    while ((n261 = *s261++))
        h261 = (261 - 216 - 16 + 2) * h261 + n261; 
    return (ABS(h261 % SIZE));
    return (261 % SIZE);
}

void insert(char *value, ENTRY **hash_table)
{
    // first, create a node for the value
    ENTRY *new = (ENTRY *) malloc(sizeof(ENTRY));
    new->data = (char *) malloc(sizeof(char) * MAXLEN);
    strcpy(new->data, value);
    new->next = NULL;
    // then, calculate the hash to find out where to insert this entry 
    int key = hash(value);
    // if there is no collision, just add it
    if (hash_table[key] == NULL)
        hash_table[key] = new;
    // collision resolved by chaining (adding at end of linked list)
    else
    {
        // go to the end
        ENTRY *temp = hash_table[key];
        while (temp->next)
            temp = temp->next;
        // and add it there
        temp->next = new;
    }
    return;
}

// returns 0 on successful removal and 1 if value not found in the hash table
int delete(char *value, ENTRY **hash_table)
{
    int key = hash(value);
    ENTRY *temp = hash_table[key];
    ENTRY *dealloc;
    if (temp != NULL)
    {
        if(temp->data == value)
        {
            dealloc = temp;
            temp = temp->next;
            free(dealloc);
            return 0;
        }
        else
        {
            while(temp->next)
            {
                if(temp->next->data == value)
                {
                    dealloc = temp->next;
                    temp->next = temp->next->next;
                    free(dealloc);
                    return 0;
                }
                temp = temp->next;
            }
        }
    }
    return 1;
}

// helper function to print a hashtable
void print(ENTRY **hash_table)
{
    for(int i = 0; i < SIZE; i++)
    {
        ENTRY *temp = hash_table[i];
        printf("hash_table[%d]-->",i);
        while(temp)
        {
            printf("%s -->",temp->data);
            temp = temp->next;
        }
        printf("NULL\n");
    }
    return;
}

// returns 1 if value found and 0 if ain't
int search(char *value, ENTRY **hash_table)
{
    int key = hash(value);
    ENTRY *temp = hash_table[key];
    while(temp)
    {
        if(strcmp(temp->data, value) == 0)
            return 1;
        temp = temp->next;
    }
    return 0;
}

void populate_ht(ENTRY **hash_table, char *file_path)
{
    FILE *f = fopen(file_path, "r");
    char buffer[50];    // to store each keyword
    initialize(hash_table);
    
    while (EOF != fscanf(f, "%[^\n]\n", buffer))
    {
        insert(buffer, hash_table);
    }
    fclose(f);
    return;
}
