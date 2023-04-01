#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "my_structs.h"

#define TABLE_SIZE 100

typedef struct node 
{
    const char*  _name;     /**< Description */
    bool         _isConst;  /**< Description */
    Type         _type;     /**< Description */
    struct node* _next;     /**< Description */
} Node;

typedef struct {
    Node *_head; /**< Description */
} Bucket;

/**
 * @brief Hash algorithm
 * @param key string to hash
 * @return int sum of all chars
 */
int Hash(const char* key);

/**
 * @brief Get the From Table object
 * @param in table 
 * @param in name 
 * @return Node* with the specified key
 */
Node* GetFromTable(const Bucket* table, const char* name);

/**
 * @brief Hash table insert
 * @param table 
 * @param id 
 * @param t 
 * @param isConst 
 */
void InsertToTable(Bucket* table, const char* id, Type t, bool isConst);

/**
 * @brief Helper function to check if variable assign is valid
 * @param type1 assignee
 * @param type2 assignor
 * @return bool is assignment valid 
 */
bool IsAssignValid(Type type1, Type type2);

/**
 * @brief Free bucket 
 * @param bucket 
 */
void FreeBucket(Bucket* bucket);

/**
 * @brief Free symbol table
 * @param table 
 */
void FreeTable(Bucket* table);

/**
 * @brief Ask the user for input
 * @param file name
 * @param node write input value to node
 */
void MipsIn(FILE* file, const Node* node);

/**
 * @brief 
 * @param file 
 * @param addOp 
 * @param reg1 
 * @param reg2 
 * @return const char* 
 */
const char* MipsAddOp(FILE* file, AddOp addOp, const char* reg1, const char* reg2);

/**
 * @brief 
 * @param file 
 * @param mulOp 
 * @param reg1 
 * @param reg2 
 * @return const char* 
 */
const char* MipsMulOp(FILE* file, MulOp mulOp, const char* reg1, const char* reg2);

/**
 * @brief 
 * @param file 
 * @param relOp 
 * @param reg1 
 * @param reg2 
 * @return const char* 
 */
const char* MipsRelOp(FILE* file, RelOp relOp, const char* reg1, const char* reg2);

/**
 * @brief 
 * @param file 
 * @param t 
 * @param id 
 * @param val 
 */
void MipsDecl(FILE* file, Type t, const char* id, const char* val);

/**
 * @brief Terminates the program
 * @param file 
 */
void MipsExit(FILE* file);

/**
 * @brief Compile logical operation
 * @param file file to write to
 * @param logOp logical operation enum
 * @param reg1 first reg
 * @param reg2 second reg
 * @return const char* 
 */
const char* MipsLogOp(FILE* file, LogOp logOp, const char* reg1, const char* reg2);

/**
 * @brief throws error
 * @param s error description
 */
void yyerror(const char* s);