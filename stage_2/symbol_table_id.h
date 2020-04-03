#ifndef __SYMBOL_TABLE_ID
#define __SYMBOL_TABLE_ID

#include<stdio.h>
#include "ast.h"                // Our AST will be used for extracting the function and variable identifiers
#include<stdlib.h>              // malloc() , exit()
#include<string.h>              // strcmp()
#include "parser.h"             // TREE_NODE structure
#define ST_ID_SIZE 8          // max size of the id symbol table, for testing purposes, change later


// typedef struct ID_NODE
// {
//     ID_TABLE_ENTRY* id_entry;
//     struct ID_NODE* next;
// }ID_NODE;

// typedef struct ID_LIST
// {
//     ID_NODE* head;
//     int size;
// }ID_LIST;

typedef struct ID_TABLE_ENTRY
{
    char* lexeme;  //variable name from lexer, hash on this
    int datatype; //enum of datatype
    int width;
    int offset;
    struct ID_TABLE_ENTRY* next; // for chaining
}ID_TABLE_ENTRY;

typedef struct ID_SYMBOL_TABLE
{
    ID_TABLE_ENTRY* id_table[ST_ID_SIZE];
    int total_ids; 
}ID_SYMBOL_TABLE;

#endif