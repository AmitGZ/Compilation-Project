#pragma once
struct abc
{
    char* sval;
    char num_kind;
};
typedef struct abc abc;

enum {INTEGER, FLOATING, STR} vtype;