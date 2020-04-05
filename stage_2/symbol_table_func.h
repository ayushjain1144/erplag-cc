#ifndef __SYMBOL_TABLE_FUNC
#define __SYMBOL_TABLE_FUNC

#include<stdio.h>
#include "ast.h"                // Our AST will be used for extracting the function and variable identifiers
#include<stdlib.h>              // malloc() , exit()
#include<string.h>              // strcmp()
#include "parser.h"             // TREE_NODE structure
#include <assert.h>
#include "symbol_table_id.h"
#define ST_FUNC_SIZE 8          // max size of the id symbol table, for testing purposes, change later


// list of types required
// for input parameters
typedef struct parList
{
    char* parname; // variable name
    TYPE* datatype; // enum of datatype
    struct typeList* next;
}parList;

typedef struct FUNC_TABLE_ENTRY
{
    char* fun_name;  //function name from lexer, hash on this
    parList* inputPara, *outputPara; //list of input and output parameters
    int width; // total size of all local vars
    ID_SYMBOL_TABLE* local_id_table;
    struct FUNC_TABLE_ENTRY* next; // for chaining
}FUNC_TABLE_ENTRY;

typedef struct FUNC_SYMBOL_TABLE
{
    FUNC_TABLE_ENTRY* func_table[ST_FUNC_SIZE];
    int total_ids; 
}GST;

#endif
