%{
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "structures.h"
#include "my_functions.h"

extern int yylex();
extern int yyparse();
%}

%union 
{
  Type  _type;   /**< Description */
  int   _ival;   /**< Description */
  char* _sval;   /**< Description */
  Num   _num;    /**< Description */
  AddOp _addOp;  /**< Description */
  RelOp _relOp;  /**< Description */
  MulOp _mulOp;  /**< Description */
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
%token COMMA
%token COLON
%token O_PARENTHESES
%token C_PARENTHESES
%token O_BRACKET
%token C_BRACKET
%token EXCLAMATION
%token SEMICOLON

%token <_sval> SENTENCE
%token <_sval> ID
%token <_num> NUM
%token <_relOp> RELOP
%token <_addOp> ADDOP
%token <_mulOp> MULOP
%token ASSIGNOP
%token OROP
%token ANDOP

%type <_type> list
%type <_type> type

%%
program         :   PROGRAM ID START declerations stmtlist END {}
                ;

declerations    :   DECL declarlist cdecl {}
                | {}
                ;
 
declarlist      :   declarlist decl {}
                |   decl {}
                ;
 
decl            :   type COLON list SEMICOLON{ $3 = $1;  /* Setting list's type */ }

list            :   ID COMMA list {
                                  $3 = $$;
                                  InsertIdToTable($1, $$, false);
                                }
                |   ID  {
                          InsertIdToTable($1, $$, false);
                        }
                ;

type            :   INT { $$ = INTEGER } 
                |   REAL { $$ = FLOATING }
                |   STRING { $$ = STR }
                ;  
 
/* the value of id should not be changed during the program*/
cdecl           :   FINAL type ID ASSIGNOP NUM SEMICOLON cdecl {
                      Type my_type = $2;
                      Num my_num = $5;  
                      if(!IsAssignValid(my_type, my_num._type)) // TODO - Tables are LinkedList(head is the current table)
                      {
                        // yyerror("Invalid type conversion, cannot convert variable: \"%s\" of type %s to \"%s\"" , my_num._sval, my_num._type, my_type);
                      }
                      else
                      {
                        InsertIdToTable($3, my_type, true);
                      }
                }
                | {}
                ; 

stmtlist        :  stmtlist stmt {}
                | {}
                ;

stmt            :   assignment_stmt {}
                |   ID ASSIGNOP SENTENCE SEMICOLON {}
                |   control_stmt {} 
                |   in_stmt {} 
                |   out_stmt {}
                |   stmt_block {}
                ;

out_stmt        :   OUT O_PARENTHESES expression C_PARENTHESES SEMICOLON {}
                |   OUT O_PARENTHESES SENTENCE C_PARENTHESES SEMICOLON {}
                ;

in_stmt         :   IN O_PARENTHESES ID C_PARENTHESES SEMICOLON {}
                ;

assignment_stmt :   ID ASSIGNOP expression SEMICOLON {}
                ;

control_stmt    :   IF O_PARENTHESES boolexpr C_PARENTHESES THEN stmt ELSE stmt {} 
		            |   WHILE O_PARENTHESES boolexpr C_PARENTHESES stmt_block {}
                |   FOREACH ID ASSIGNOP NUM TILL NUM WITH step stmt {}
                |   FOREACH ID ASSIGNOP NUM TILL ID WITH step stmt {}
                |   switch {}

stmt_block      :   O_BRACKET stmtlist C_BRACKET {}
                ;

switch          :   SWITCH O_PARENTHESES ID C_PARENTHESES O_BRACKET cases C_BRACKET {}
                ;

cases           :   CASE NUM COLON stmtlist BREAK SEMICOLON cases {}
                |   DEFAULT COLON stmtlist {}
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
        
boolfactor      :   EXCLAMATION O_PARENTHESES boolfactor C_PARENTHESES {} /* Meaning not BOOLFACTOR */
                |   expression RELOP expression {}
                ;  

expression      :   expression ADDOP term {} 
                |   term {}
                ;

term            :   term MULOP factor {}
                |   factor {}
                ;

factor          :   O_PARENTHESES expression C_PARENTHESES {}
                |   ID {}
                |   NUM {}
                ;

%%

int main(int argc, char** argv) 
{
	extern FILE* yyin;
	extern FILE* yyout;
  yyin = fopen(argv[1], "r");
  yyout = fopen(argv[2], "w");

	do 
	{
		yyparse();
	} while(!feof(yyin));

	return 0;
}
