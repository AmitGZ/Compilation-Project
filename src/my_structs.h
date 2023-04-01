#pragma once
#include <stdbool.h>

typedef enum { INTEGER, FLOATING, STR, TYPE_COUNT } Type;
typedef enum { EQ, NEQ, LT, GT, LE, GE, REL_OP_COUNT } RelOp;
typedef enum { ADD, SUB, ADD_OP_COUNT} AddOp;
typedef enum { MUL, DIV, MUL_OP_COUNT } MulOp;
typedef enum { AND, OR, NOR, LOG_OP_COUNT } LogOp;

static const char* Temporaries[] = { "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7" };

typedef struct
{
    Type        _type;        /**< Description */
    const char* _sval;        /**< Description */
    bool        _isImmediate; /**< Description */
} Val;