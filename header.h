#pragma once
typedef enum { INTEGER, FLOATING, STR } Type;
typedef enum { EQ, NEQ, LT, GT, LE, GE } RelOp;
typedef enum { ADD, SUB } AddOp;
typedef enum { MUL, DIV } MulOp;

typedef struct
{
    Type _type;
    char* _sval;
}Num;