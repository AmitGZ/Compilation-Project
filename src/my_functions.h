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
 * @param node write input value to node
 */
void MipsIn(const Node* node);

void MipsMathOp(MathOp mulOp, Reg* res, Reg* reg0, Reg* reg1);

void MipsRelOp(RelOp relOp, Reg* res, Reg* reg0, Reg* reg1);

void MipsLogOp(LogOp logOp, Reg* res, Reg* reg0, Reg* reg1);

void MipsDecl(Type t, const char* id, const char* val);

void MipsExit();

/**
 * @brief 
 * 
 * @param node 
 * @param name
 */
void MipsAssign(const Node* node, Reg* reg);

/**
 * @brief 
 * 
 * @param node 
 */
void MipsOut(const Reg* reg);

/**
 * @brief 
 */
void MipsData();

/**
 * @brief throws error
 * @param s error description
 */
void yyerror(const char* s);

void MipsLoadV(Reg* reg);

void MipsLoadI(Reg* reg);

void MipsCast(Reg* reg, Type t);

void MipsMain();

const char* GetRegT();

const char* GetRegF();

void FreeRegF();

void FreeRegT();

void FreeAllRegs();