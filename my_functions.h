#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void InsertIdToTable(const char* id, char type, bool isConst);

bool IsAssignValid(char kind1, char kind2);

void yyerror(const char* s);
