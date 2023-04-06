%{
// TODO
// Move hash to separate files
// set mips file as extern

// Questions
// Can we use .cpp?
// Can we use TABLE_SIZE for hash table?

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "my_structs.h"
#include "my_functions.h"

extern int yylex();
extern int yyparse();
extern size_t errorCount;

extern size_t regTrackerT;
extern size_t regTrackerF;
FILE* mips;
Type currentType;
Bucket table[TABLE_SIZE];

%}

%union 
{
  Type        _type;   /**< Description */
  Reg         _val;    /**< Description */
  const char* _name;   /**< Description */
  RelOp       _relOp;  /**< Description */
  MathOp      _mathOp;  /**< Description */
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
%token ERROR

%token <_name> ID
%token <_val> SENTENCE
%token <_val> NUM
%token <_relOp> RELOP
%token <_mathOp> ADDOP
%token <_mathOp> MULOP
%token ASSIGNOP
%token OROP
%token ANDOP

%type <_type> list
%type <_type> type
%type <_val>  factor
%type <_val>  term
%type <_val>  expression
%type <_val> boolfactor
%type <_val> boolterm
%type <_val> boolexpr

%%
program         :   PROGRAM ID START declerations stmtlist END{ MipsExit(mips); }
                ;

declerations    :   DECL { MipsData(mips); } declarlist cdecl { 
                                                                fprintf(mips, "\n\t.text\
                                                                               \n\t.globl main\
                                                                               \n\nmain:\n"); /* Start main scope */ 
                                                              }
                | {}
                ;
 
declarlist      :   declarlist decl {}
                |   decl {}
                ;
 
decl            :   type { currentType = $1 } COLON list SEMICOLON

list            :   ID COMMA list {
                                    InsertToTable(table, $1, currentType, false);
                                    MipsDecl(mips, currentType, $1, "0");
                                  }
                |   ID  {
                          InsertToTable(table, $1, currentType, false);
                          MipsDecl(mips, currentType, $1, "0");
                        }
                ;

type            :   INT { $$ = INTEGER }
                |   REAL { $$ = FLOATING }
                |   STRING { $$ = STR }
                ;  
 
/* the value of id should not be changed during the program*/
cdecl           :   FINAL type ID ASSIGNOP NUM SEMICOLON cdecl{
                                                                Type my_type = $2;
                                                                Reg my_val = $5;  

                                                                if(IsAssignValid(my_type, my_val._type))
                                                                {
                                                                  InsertToTable(table, $3, my_type, true);
                                                                  //MipsDecl(mips, my_type, my_val., my_val._sval);
                                                                }
                                                                else
                                                                {
                                                                  // yyerror("Invalid type conversion, cannot convert variable: \"%s\" of type %s to \"%s\"" , my_val._name, my_val._type, my_type);
                                                                }
                                                              }
                | {}
                ; 

stmtlist        :  stmtlist stmt { FreeAllRegs(); }
                | {}
                ;

stmt            :   assignment_stmt {}
                |   ID ASSIGNOP SENTENCE SEMICOLON{ 
                                                    Node* node = GetFromTable(table, $1);
                                                    if (node != NULL)
                                                    {
                                                      Reg* val = &($3);
                                                      MipsLoadI(mips, val); 
                                                      MipsAssign(mips, node, val);
                                                    }
                                                    else
                                                    {
                                                      yyerror(strcat("ID does not exist: ", $1));
                                                    }
                                                  }
                |   control_stmt {}
                |   in_stmt {}
                |   out_stmt {}
                |   stmt_block {}
                ;

out_stmt        :   OUT O_PARENTHESES expression C_PARENTHESES SEMICOLON { MipsOut(mips, &($3)); }
                |   OUT O_PARENTHESES SENTENCE C_PARENTHESES SEMICOLON{ 
                                                                        MipsLoadI(mips, &($3)); 
                                                                        MipsOut(mips, &($3)); 
                                                                      }
                ;

in_stmt         :   IN O_PARENTHESES ID C_PARENTHESES SEMICOLON { 
                                                                  Node* node = GetFromTable(table, $3);
                                                                  MipsIn(mips, node); 
                                                                }
                ;

assignment_stmt :   ID ASSIGNOP expression SEMICOLON { 
                                                        Node* node = GetFromTable(table, $1);
                                                        Reg* val = &($3);
                                                        if (node != NULL)
                                                        {
                                                          MipsAssign(mips, node, val);
                                                        }
                                                        else
                                                        {
                                                          // Throw exception
                                                        }
                                                      }
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

step            :   ID ASSIGNOP ID ADDOP NUM{ 
                                              // copy from below
                                            }
	              |   ID ASSIGNOP ID MULOP NUM{ 
                                              Node* node0 = GetFromTable(table, $1);
                                              Node* node1 = GetFromTable(table, $3);
                                              Reg* val0 = &($5);
                                              MathOp mathOp = $4;
                                              if ((node0 != NULL) && (node1 != NULL))
                                              {
                                                Reg val1 = { node1->_type, node1->_name };
                                                MipsLoadV(mips, &val1);
                                                Reg res;
                                                MipsMathOp(mips, $4, &res, val0, &val1);
                                                MipsAssign(mips, node0, &res);
                                              }
                                              else
                                              {
                                                yyerror("ID not found");
                                              }
                                            }
                ;

boolexpr        :   boolexpr OROP boolterm  { MipsLogOp(mips, OR, &($$), &($1), &($3)); }
                |   boolterm { $$ = $1; }
                ;

boolterm        :   boolterm ANDOP boolfactor { MipsLogOp(mips, AND, &($$), &($1), &($3)); }
                |   boolfactor { $$ = $1; }
                ;
        
boolfactor      :   EXCLAMATION O_PARENTHESES boolfactor C_PARENTHESES { /*MipsLogOp(mips, AND, &($$), &($3), &(ZeroReg));*/}
                |   expression RELOP expression {
                                                  Reg* res = &($$);
                                                  Reg* val0 = &($1);
                                                  Reg* val1 = &($3);
                                                  MipsRelOp(mips, $2, res, val0, val1);
                                                }
                ;  

expression      :   expression ADDOP term { 
                                            Reg* res = &($$);
                                            Reg* val0 = &($1);
                                            Reg* val1 = &($3);
                                            MipsMathOp(mips, $2, res, val0, val1);
                                          }
                |   term { $$ = $1; }
                ;

term            :   term MULOP factor { 
                                        Reg* res = &($$);
                                        Reg* val0 = &($1);
                                        Reg* val1 = &($3);
                                        MipsMathOp(mips, $2, res, val0, val1);
                                      }
                |   factor  { $$ = $1; }
                ;

factor          :   O_PARENTHESES expression C_PARENTHESES{
                                                            $$ = $2; 
                                                          }
                |   ID{ 
                        Node* node = GetFromTable(table, $1);
                        if (node != NULL)
                        {
                          $$._type = node->_type;
                          $$._sval = node->_name;
                          MipsLoadV(mips, &($$));
                        }
                        else
                        {
                          yyerror(strcat("ID does not exist: ", $1));
                        }
                      }
                |   NUM { 
                          $$ = $1;
                          MipsLoadI(mips, &($$));
                        }
                ;

%%

int main(int argc, char** argv) 
{
  // Openning files
	extern FILE* yyin;
	extern FILE* yyout;
  yyin = fopen(argv[1], "r");
  yyout = fopen(argv[2], "w");
  mips = fopen(argv[3], "w");

	do
	{
		yyparse();
	} while(!feof(yyin));
  
  // Closing files
  fclose(mips);
  fclose(yyin);
  fclose(yyout);

  printf("error Count: %zu\n", errorCount);
  if(errorCount > 0U)
  {
    remove(argv[3]); // Removing compiled file in case of error
  }

  // Freeing
  FreeTable(table);
	return 0;
}
