#include "my_functions.h"

void InsertIdToTable(const char* id, char type, bool isConst)
{

}

bool IsAssignValid(char kind1, char kind2) 
{
     return kind1 == kind2; 
}

void yyerror(const char* s)
{
	fprintf(stderr, "Parse error: %s\n", s);
	exit(1);
}