#pragma once
#include <stdio.h>
#include <stdbool.h>

typedef enum { INTEGER, FLOATING, STR, TYPE_COUNT } Type;
typedef enum { EQ, NEQ, LT, GT, LE, GE, REL_OP_COUNT } RelOp;
typedef enum { ADD, SUB, MUL, DIV, MATH_OP_COUNT} MathOp;
typedef enum { AND, OR, NOR, LOG_OP_COUNT } LogOp;

typedef struct
{
    Type        _type;   /**< Register's type (STR/FLOATING/INTEGER) */
    const char* _name;   /**< Register's name (Ex. "$t0") */
} Reg;

typedef struct
{
    Type        _type;   /**< Value's type (STR/FLOATING/INTEGER) */
    const char* _sval;   /**< Value's value in string format (Ex. "3.25") */
} Val;

static const size_t TmpRegCount = 10U; /**< Temporary registers' count */
static const char* IntTmpRegs[] =    { "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6",  "$t7",  "$t8",  "$t9"  };
static const char* FloatTmpRegs[] =  { "$f4", "$f5", "$f6", "$f7", "$f8", "$f9", "$f10", "$f16", "$f17", "$f18" };
static const Reg ZeroReg = { INTEGER, "$zero" }; /**< Zero register constant */