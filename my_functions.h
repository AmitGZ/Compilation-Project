#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "structures.h"

void InsertIdToTable(const char* id, Type t, bool isConst);

bool IsAssignValid(Type type1, Type type2);

void yyerror(const char* s);
