%{
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "header.h"

// Functions TODO move to separate .h .c files
void insertIdToTable(char* id, char type, bool isConst){}
bool isAssignValid(char kind1, char kind2) { return kind1 == kind2; }
extern int yylex();
extern int yyparse();
void yyerror(const char* s);

%}

%union {
  int ival;
  char* sval;
  char kind;
  abc number;
}

/* tokens & type of gramer variables */
/* Declare the TOKENS */
/* example : %token <ival> NUM */

%token BREAK
%token CASE
%token FINAL
%token DECL
%token DEFAULT
%token ELSE
%token END
%token FLOAT
%token FOREACH
%token IF
%token IN
%token INT
%token OUT
%token PROGRAM
%token REAL
%token START
%token STRING
%token SWITCH
%token THEN
%token TILL
%token WHILE
%token WITH

%token <sval> SENTENCE
%token <sval> ID
%token <number> NUM
%token <op> RELOP
%token <op> ADDOP
%token <op> MULOP
%token ASSIGNOP
%token OROP
%token ANDOP

%type <kind> list
%type <kind> type

%%
program         :   PROGRAM ID START declerations stmtlist END {}
                ;

declerations    :   DECL declarlist cdecl {}
                | {}
                ;
 
declarlist      :   declarlist decl {}
                |   decl {}
                ;
 
decl            :   type ':' list {$3 = '1';}

list            :   ID ',' list {
                                  $3=$$;
                                  insertIdToTable($1,$$,false);
                                }
                |   ID  {
                          insertIdToTable($1,$$,false);
                        }
                ;

type            :   INT { $$ = 'i'} 
                |   REAL { $$ = 'f'}
                |   STRING { $$ = 's'}
                ;  
 
/* the value of id should not be changed during the program*/
cdecl           :   FINAL type ID ASSIGNOP NUM ';' cdecl {     
                      char my_kind = $2;
                      abc my_num = $5;                 
                      if(!isAssignValid(my_kind, my_num._type)) // TODO - Tables are LinkedList(head is the current table)
                      {
                        /*error*/
                      }
                      else
                      {
                        insertIdToTable($3, my_kind,true);
                      }       
                }
                | {}
                ; 

stmtlist        :  stmtlist stmt {}
                | {}
                ;

stmt            :   assignment_stmt {}
                |   ID ASSIGNOP SENTENCE ';' {}
                |   control_stmt {} 
                |   in_stmt {} 
                |   out_stmt {}
                |   stmt_block {}
                ;

out_stmt        :   OUT '(' expression ')' ';' {}
                |   OUT '(' SENTENCE ')' ';' {}
                ;

in_stmt         :   IN '(' ID ')' ';' {}
                ;

assignment_stmt :   ID ASSIGNOP expression ';' {}
                ;

control_stmt    :   IF ')' boolexpr '(' THEN stmt ELSE stmt {} 
		            |   WHILE ')' boolexpr '(' stmt_block {}
                |   FOREACH ID ASSIGNOP NUM TILL NUM WITH step stmt {}
                |   FOREACH ID ASSIGNOP NUM TILL ID WITH step stmt {}
                |   switch {}

stmt_block      :   '{' stmtlist '}' {}
                ;

switch          :   SWITCH '(' ID ')' '{' cases '}' {}
                ;

cases           :   CASE NUM ':' stmtlist BREAK ';' cases {}
                |   DEFAULT ':' stmtlist {}
                ;

step            :   ID ASSIGNOP ID ADDOP NUM {}
	              |   ID ASSIGNOP ID MULOP NUM {}
                ;

boolexpr        :   boolexpr OROP boolterm {}
                |   boolterm {}
                ;

boolterm        :   boolterm ANDOP boolfactor {}
                |   boolfactor {}
                ;
        
boolfactor      :   '!' '(' boolfactor ')' {} /*Meaning not BOOLFACTOR*/
                |   expression RELOP expression {}
                ;  

expression      :   expression ADDOP term {} 
                |   term {}
                ;

term            :   term MULOP factor {}
                |   factor {}
                ;

factor          :   '(' expression ')' {}
                |   ID {}
                |   NUM {}
                ;

%%

int main() 
{
	FILE* yyin = stdin;

	do 
	{
		yyparse();
	} while(!feof(yyin));

	return 0;
}

void yyerror(const char* s) 
{
	fprintf(stderr, "Parse error: %s\n", s);
	exit(1);
}
