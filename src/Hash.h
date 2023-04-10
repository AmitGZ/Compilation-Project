#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include "MyStructs.h"

/**
 * @brief Used to assert function inpus
 * @param exp Boolean expression to assert
 * @param desc Description to print in case of fail
 * @return retVal Value to return if assert fails
 */
#define MY_ASSERT(exp, desc, retVal) if(!(exp)) { printf("SW Error | file: %s, line: %d, %s\n", __FILE__, __LINE__, (desc)); return retVal; }
#define VOID_VAL 

#define TABLE_SIZE 100
#define BUFFER_SIZE 256

extern int yylineno;            /**< Parse error line index*/
extern size_t errorCount;       /**< Total error count */

typedef struct node 
{
    const char*  _name;     /**< Variable's name */
    bool         _isConst;  /**< Is variable constant */
    Type         _type;     /**< Variable's type (STR/FLOATING/INTEGER) */
    struct node* _next;     /**< Linked list, next variable that shares the same hash code */
} Node;

typedef struct {
    Node *_head; /**< Hash table, pointer to array of nodes */
} Bucket;

/**
 * @brief Hash algorithm
 * @param key string to hash
 * @return int sum of all chars
 */
int Hash(const char* key);

/**
 * @brief Get the From Table object
 * @param table table to search
 * @param name variable name
 * @return Node* with the specified key, returns NULL if not found
 */
Node* GetFromTable(const Bucket* table, const char* name);

/**
 * @brief Hash table insert
 * @param table table to insert
 * @param id variable name
 * @param t variable type (STR/FLOAING/INTEGER)
 * @param isConst Is variable constant
 * @return bool Is insertion successful
 */
bool InsertToTable(Bucket* table, const char* id, Type t, bool isConst);

/**
 * @brief Helper function to check if variable assign is valid
 * @param type1 assignee
 * @param type2 assignor
 * @return bool is assignment valid 
 */
bool IsAssignValid(Type type1, Type type2);

/**
 * @brief Free bucket 
 * @param bucket bucket to free
 */
void FreeBucket(Bucket* bucket);

/**
 * @brief Free symbol table
 * @param table table to free
 */
void FreeTable(Bucket* table);

/**
 * @brief throws error
 * @param str error description
 */
void yyerror(const char* str, ...);