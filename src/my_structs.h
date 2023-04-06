#pragma once
#include <stdbool.h>

typedef enum { INTEGER, FLOATING, STR, TYPE_COUNT } Type;
typedef enum { EQ, NEQ, LT, GT, LE, GE, REL_OP_COUNT } RelOp;
typedef enum { ADD, SUB, MUL, DIV, MATH_OP_COUNT} MathOp;
typedef enum { AND, OR, NOR, LOG_OP_COUNT } LogOp;

typedef struct
{
    Type        _type;        /**< Description */
    const char* _sval;         /**< Description */
} Reg;

static const char* TmpRegs[] =    { "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7" };
static const char* FloatRegs[] =  { "$f0", "$f1", "$f2", "$f3", "$f4", "$f5", "$f6", "$f7" };
static const char* SaveRegs[] =   { "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7" };
static const Reg ZeroReg = { INTEGER, "$zero" };