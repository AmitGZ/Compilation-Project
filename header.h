#ifndef HEADERFILE_H
#define HEADERFILE_H

struct abc
{
    char* sval;
    char num_kind;
};
typedef struct abc abc;

/* yylex () and yyerror() need to be declared here */
enum {INTEGER, FLOATING, STR} vtype;
extern int yylex (void);
void yyerror (char *s);
void insertIdToTable(char* id,char type,bool isConst){}
bool isAssignValid(char kind1, char kind2){return kind1 == kind2;}

#endif