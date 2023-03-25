#include "my_functions.h"

void InsertIdToTable(const char* id, Type t, bool isConst)
{

}

bool IsAssignValid(Type type1, Type type2) 
{
    return (type1 == type2) || (type1 == FLOATING); 
}

void yyerror(const char* s)
{
	fprintf(stderr, "Parse error: %s\n", s);
	exit(1);
}