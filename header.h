#pragma once
typedef enum {INTEGER, FLOATING, STR} Type;

struct abc
{
    Type _type;
    char* _name;
};
typedef struct abc abc;
