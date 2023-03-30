#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "my_structs.h"

#define TABLE_SIZE 100

typedef struct node 
{
    char* _name;        /**< Description */
    bool _isConst;      /**< Description */
    Type _type;         /**< Description */
    struct node *_next; /**< Description */
} Node;

typedef struct {
    Node *_head; /**< Description */
} Bucket;

// Hash algorithm, sums all chars and returns int
int Hash(const char* key);

// Hash table get, returns pointer to node with the specified key
Node* GetFromTable(const Bucket* table, const char* name);

// Hash table insert
void InsertToTable(Bucket* table, char* id, Type t, bool isConst);

// Helper function to check if variable assign is valid
bool IsAssignValid(Type type1, Type type2);

// Free bucket method
void FreeBucket(Bucket* bucket);

// Free table method
void FreeTable(Bucket* table);

// Write addition of two registers and store in result
void MipsAdd(FILE* file, char* result, const char* reg1, const char* reg2);

// Write multiplication of two registers and store in result
void MipsMul(FILE* file, char* result, const char* reg1, const char* reg2);

// Write or operation of two registers and store in result
void MipsOr(FILE* file, char* result, const char* reg1, const char* reg2);

// Write and operation of two registers and store in result
void MipsAnd(FILE* file, char* result, const char* reg1, const char* reg2);

// Write relational operation of two registers and store in result
void MipsRelop(FILE* file, char* result, const char* reg1, const char* reg2, RelOp relOp);

// Write not operation of reg1 into result
void MipsNot(FILE* file, char* result, const char* reg1);

void MipsIn(FILE* file, const Node* node);

void MipsAdd(FILE* file, char* result, const char* reg1, const char* reg2);

void MipsMul(FILE* file, char* result, const char* reg1, const char* reg2);

void MipsOr(FILE* file, char* result, const char* reg1, const char* reg2);

void MipsAnd(FILE* file, char* result, const char* reg1, const char* reg2);

void MipsRelop(FILE* file, char* result, const char* reg1, const char* reg2, RelOp relOp);

void MipsNot(FILE* file, char* result, const char* reg1);

void MipsDecl(FILE* file, Type t, const char* id, const char* val);

// Throws error and exits
void yyerror(const char* s);
