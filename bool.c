//                  ,-.                         Group 08                                                    //
//          ,      ( {o\                        Satvik Golechha :: 2017A7PS0117P                           ///
//          {`"=,___) (`~                       Bharat Bhargava :: 2017A7PS0025P                          ////
//           \  ,_.-   )                        Ayush      Jain :: 2017A7PS0093P                         /////
//      ~^~^~^`- ~^ ~^ '~^~^~^~                                                                         //////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include<stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "bool.h"

void initialize_bool_array(bool* arr)
{
    for (int i = 0; i < MAX_BOOL_ARRAY_SIZE; i++)
        arr[i] = 0;
}

bool* or(bool * arr1, bool* arr2)
{
    bool* arr = (bool*) malloc (sizeof(bool) * MAX_BOOL_ARRAY_SIZE);

   for (int i  = 0; i < MAX_BOOL_ARRAY_SIZE - 1; i++)
       arr[i] = arr1[i] | arr2[i];
   

   return arr;
}

// stores results in first array
void or_and_store(bool * arr1, bool* arr2)
{

   for (int i  = 0; i < MAX_BOOL_ARRAY_SIZE - 1; i++)
       arr1[i] = arr1[i] | arr2[i];
   

   return;
}

bool* and(bool * arr1, bool* arr2)
{
    bool* arr = (bool*) malloc (sizeof(bool) * MAX_BOOL_ARRAY_SIZE);

   for (int i  = 0; i < MAX_BOOL_ARRAY_SIZE - 1; i++)
       arr[i] = arr1[i] & arr2[i];
   

   return arr;
}

bool* xor(bool * arr1, bool* arr2)
{
    bool* arr = (bool*) malloc (sizeof(bool) * MAX_BOOL_ARRAY_SIZE);

   for (int i  = 0; i < MAX_BOOL_ARRAY_SIZE - 1; i++)
       arr[i] = arr1[i] ^ arr2[i];
    

   return arr;
}

bool* not(bool * arr1)
{
    bool* arr = (bool*) malloc (sizeof(bool) * MAX_BOOL_ARRAY_SIZE);

   for (int i  = 0; i < MAX_BOOL_ARRAY_SIZE - 1; i++)
       arr[i] = ~arr1[i];

   return arr;
}

void print_bool(bool* arr, int size)
{
    for (int i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}