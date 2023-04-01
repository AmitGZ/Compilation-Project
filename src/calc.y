%{
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

FILE* mips;
Type currentType;
Bucket table[TABLE_SIZE];

%}

%union 
{
  Type        _type;   /**< Description */
  Val         _val;    /**< Description */
  const char* _name;   /**< Description */
  AddOp       _addOp;  /**< Description */
  RelOp       _relOp;  /**< Description */
  MulOp       _mulOp;  /**< Description */
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
%token <_addOp> ADDOP
%token <_mulOp> MULOP
%token ASSIGNOP
%token OROP
%token ANDOP

%type <_type> list
%type <_type> type
%type <_val>  factor
%type <_val>  term
%type <_val>  expression
%type <_name> boolfactor
%type <_name> boolterm
%type <_name> boolexpr

%%
program         :   PROGRAM ID START declerations stmtlist END{ MipsExit(mips); }
                ;

declerations    :   DECL { fprintf(mips, "\t.data\n"); } declarlist cdecl { 
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
                                    MipsDecl(mips, currentType, $1, "");
                                  }
                |   ID  {
                          InsertToTable(table, $1, currentType, false);
                          MipsDecl(mips, currentType, $1, "");
                        }
                ;

type            :   INT { $$ = INTEGER }
                |   REAL { $$ = FLOATING }
                |   STRING { $$ = STR }
                ;  
 
/* the value of id should not be changed during the program*/
cdecl           :   FINAL type ID ASSIGNOP NUM SEMICOLON cdecl{
                                                                Type my_type = $2;
                                                                Val my_val = $5;  

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

stmtlist        :  stmtlist stmt {}
                | {}
                ;

stmt            :   assignment_stmt {}
                |   ID ASSIGNOP SENTENCE SEMICOLON{ 
                                                    Node* node = GetFromTable(table, $1);
                                                    if (node != NULL)
                                                    {
                                                      if (node->_type == STR)
                                                      {
                                                        MipsLoad(mips, &($3), '0'); 
                                                        MipsAssign(mips, node, "$s0");
                                                      }
                                                      else
                                                      {
                                                        yyerror("Can't assign string to non string type\n");
                                                      }
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

out_stmt        :   OUT O_PARENTHESES expression C_PARENTHESES SEMICOLON {}
                |   OUT O_PARENTHESES SENTENCE C_PARENTHESES SEMICOLON {}
                ;

in_stmt         :   IN O_PARENTHESES ID C_PARENTHESES SEMICOLON { 
                                                                  Node* node = GetFromTable(table, $3);
                                                                  MipsIn(mips, node); 
                                                                }
                ;

assignment_stmt :   ID ASSIGNOP expression SEMICOLON {  }
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
                                              Node* node1 = GetFromTable(table, $1);
                                              Node* node2 = GetFromTable(table, $3);
                                              Val* val = &($5);
                                              AddOp addOp = $4;
                                              if (((node1 != NULL) && (node2 != NULL)) && IsAssignValid(node1->_type, val->_type))
                                                node1->_name = MipsAddOp(mips, addOp, node2->_name, val->_sval);
                                              else
                                                // Throw exception
                                            }
	              |   ID ASSIGNOP ID MULOP NUM{ 
                                              Node* node1 = GetFromTable(table, $1);
                                              Node* node2 = GetFromTable(table, $3);
                                              Val* val = &($5);
                                              MulOp mulOp = $4;
                                              if (((node1 != NULL) && (node2 != NULL)) && IsAssignValid(node1->_type, val->_type))
                                                node1->_name = MipsMulOp(mips, mulOp, node2->_name, val->_sval);
                                              else
                                                // Throw exception
                                            }
                ;

boolexpr        :   boolexpr OROP boolterm  { $$ = MipsLogOp(mips, OR, $1, $3); }
                |   boolterm { $$ = $1; }
                ;

boolterm        :   boolterm ANDOP boolfactor { $$ = MipsLogOp(mips, AND, $1, $3); }
                |   boolfactor { $$ = $1; }
                ;
        
boolfactor      :   EXCLAMATION O_PARENTHESES boolfactor C_PARENTHESES { $$ = MipsLogOp(mips, NOR, $3, "$zero"); }
                |   expression RELOP expression { $$ = MipsRelOp(mips, $2, $1._sval, $3._sval); }
                ;  

expression      :   expression ADDOP term { 
                                            $$._isImmediate = false;
                                            $$._type = INTEGER;
                                            $$._sval = MipsAddOp(mips, $2, $1._sval, $3._sval);
                                          }
                |   term { $$ = $1; }
                ;

term            :   term MULOP factor { 
                                        Val* val1 = &($1);
                                        Val* val2 = &($3);
                                        if((val1->_type == STR) || (val2->_type == STR))
                                        {
                                          yyerror("Can't perform arithmetic operations on type string\n");
                                        }
                                        else
                                        {
                                          $$._sval = MipsMulOp(mips, $2, val1->_sval, val2->_sval); 
                                          $$._isImmediate = false;
                                          $$._type = ((val1->_type == FLOATING) || (val2->_type == FLOATING)) ? FLOATING : INTEGER;
                                        }
                                      }
                |   factor  {
                              if ($1._type == STR)
                              {
                                MipsLoad(mips, &($1), '0'); 
                                $$ = $1;
                              }
                              else
                              {

                              }
                            }
                ;

factor          :   O_PARENTHESES expression C_PARENTHESES { $$ = $2; }
                |   ID{ 
                        Node* node = GetFromTable(table, $1);
                        if (node != NULL)
                        {
                          $$._type = node->_type;
                          $$._sval = node->_name;
                          $$._isImmediate = false;
                        }
                        else
                        {
                          yyerror(strcat("ID does not exist: ", $1));
                        }
                      }
                |   NUM { $$ = $1 }
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
