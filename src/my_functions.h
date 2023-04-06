#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "my_structs.h"

#define TABLE_SIZE 100
#define BUFFER_SIZE 256

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

void MipsMathOp(FILE* file, MathOp mulOp, Reg* res, Reg* reg0, Reg* reg1);

void MipsRelOp(FILE* file, RelOp relOp, Reg* res, Reg* reg0, Reg* reg1);

void MipsLogOp(FILE* file, LogOp logOp, Reg* res, Reg* reg0, Reg* reg1);

void MipsDecl(FILE* file, Type t, const char* id, const char* val);

/**
 * @brief Terminates the program
 * @param file 
 */
void MipsExit(FILE* file);

/**
 * @brief 
 * 
 * @param file 
 * @param node 
 * @param name
 */
void MipsAssign(FILE* file, const Node* node, Reg* reg);

/**
 * @brief 
 * 
 * @param file 
 * @param node 
 */
void MipsOut(FILE* file, const Reg* reg);

/**
 * @brief 
 * 
 * @param file 
 */
void MipsData(FILE* file);

/**
 * @brief throws error
 * @param s error description
 */
void yyerror(const char* s);

void MipsLoadV(FILE* file, Reg* reg);

void MipsLoadI(FILE* file, Reg* reg);

void MipsCast(FILE* file, Reg* reg, Type t);

const char* GetRegT();

const char* GetRegF();

void FreeRegF();

void FreeRegT();

void FreeAllRegs();