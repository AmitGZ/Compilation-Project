%{
// TODO

// Questions
// Can we use .cpp?
// Can we use TABLE_SIZE for hash table?

#include "my_structs.h"
#include "Hash.h"
#include "MipsWriter.h"
#include <string.h>

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
program         :   PROGRAM ID START declerations stmtlist END{ MipsExit(); }
                ;

declerations    :   DECL { MipsData(); } declarlist cdecl { MipsMain(); }
                | {}
                ;
 
declarlist      :   declarlist decl {}
                |   decl {}
                ;
 
decl            :   type { currentType = $1 } COLON list SEMICOLON

list            :   ID COMMA list {
                                    InsertToTable(table, $1, currentType, false);
                                    MipsDecl(currentType, $1, "0"); // Initialize with 0
                                  }
                |   ID  {
                          InsertToTable(table, $1, currentType, false);
                          MipsDecl(currentType, $1, "0"); // Initialize with 0
                        }
                ;

type            :   INT { $$ = INTEGER }
                |   REAL { $$ = FLOATING }
                |   STRING { $$ = STR }
                ;  
 
/* the value of id should not be changed during the program*/
cdecl           :   FINAL type ID ASSIGNOP NUM SEMICOLON cdecl{
                                                                Type my_type = $2;
                                                                Reg my_reg = $5;
                                                                const char* id = $3;

                                                                if(IsAssignValid(my_type, my_reg._type))
                                                                {
                                                                  InsertToTable(table, id, my_type, true);
                                                                  MipsDecl(my_type, id, my_reg._sval);
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
                                                      MipsLoadI(val); 
                                                      MipsAssign(node, val);
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

out_stmt        :   OUT O_PARENTHESES expression C_PARENTHESES SEMICOLON { MipsOut(&($3)); }
                |   OUT O_PARENTHESES SENTENCE C_PARENTHESES SEMICOLON{ 
                                                                        MipsLoadI(&($3)); 
                                                                        MipsOut(&($3)); 
                                                                      }
                ;

in_stmt         :   IN O_PARENTHESES ID C_PARENTHESES SEMICOLON { 
                                                                  Node* node = GetFromTable(table, $3);
                                                                  MipsIn(node); 
                                                                }
                ;

assignment_stmt :   ID ASSIGNOP expression SEMICOLON { 
                                                        Node* node = GetFromTable(table, $1);
                                                        Reg* val = &($3);
                                                        if (node != NULL)
                                                        {
                                                          MipsAssign(node, val);
                                                        }
                                                        else
                                                        {
                                                          // Throw exception
                                                        }
                                                      }
                ;

control_stmt    :   IF O_PARENTHESES boolexpr C_PARENTHESES THEN { MipsIf(&($3), 0U); } stmt ELSE { MipsIf(&($3), 1U); } stmt { MipsIf(&($3), 2U); }
		            |   WHILE { MipsWhile(NULL, 0U); } O_PARENTHESES boolexpr C_PARENTHESES { MipsWhile(&($4), 1U); } stmt_block { MipsWhile(&($4), 2U); }
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
                                              Node* node0 = GetFromTable(table, $1);
                                              Node* node1 = GetFromTable(table, $3);
                                              Reg* val0 = &($5);
                                              MathOp mathOp = $4;
                                              if ((node0 != NULL) && (node1 != NULL))
                                              {
                                                Reg val1 = { node1->_type, node1->_name };
                                                MipsLoadV(&val1);
                                                Reg res;
                                                MipsMathOp($4, &res, val0, &val1);
                                                MipsAssign(node0, &res);
                                              }
                                              else
                                              {
                                                yyerror("ID not found");
                                              }
                                            }
	              |   ID ASSIGNOP ID MULOP NUM{ 
                                              Node* node0 = GetFromTable(table, $1);
                                              Node* node1 = GetFromTable(table, $3);
                                              Reg* val0 = &($5);
                                              MathOp mathOp = $4;
                                              if ((node0 != NULL) && (node1 != NULL))
                                              {
                                                Reg val1 = { node1->_type, node1->_name };
                                                MipsLoadV(&val1);
                                                Reg res;
                                                MipsMathOp($4, &res, val0, &val1);
                                                MipsAssign(node0, &res);
                                              }
                                              else
                                              {
                                                yyerror("ID not found");
                                              }
                                            }
                ;

boolexpr        :   boolexpr OROP boolterm  { MipsLogOp(OR, &($$), &($1), &($3)); }
                |   boolterm { $$ = $1; }
                ;

boolterm        :   boolterm ANDOP boolfactor { MipsLogOp(AND, &($$), &($1), &($3)); }
                |   boolfactor { $$ = $1; }
                ;
        
boolfactor      :   EXCLAMATION O_PARENTHESES boolfactor C_PARENTHESES{ 
                                                                        Reg zeroReg = { INTEGER, ZeroReg };
                                                                        MipsLogOp(AND, &($$), &($3), &(zeroReg));
                                                                      }
                |   expression RELOP expression { MipsRelOp($2, &($$), &($1), &($3)); }
                ;  

expression      :   expression ADDOP term { MipsMathOp($2, &($$), &($1), &($3)); }
                |   term { $$ = $1; }
                ;

term            :   term MULOP factor { MipsMathOp($2, &($$), &($1), &($3)); }
                |   factor  { $$ = $1; }
                ;

factor          :   O_PARENTHESES expression C_PARENTHESES{ $$ = $2; }
                |   ID{ 
                        Node* node = GetFromTable(table, $1);
                        if (node != NULL)
                        {
                          $$._type = node->_type;
                          $$._sval = node->_name;
                          MipsLoadV(&($$));
                        }
                        else
                        {
                          yyerror(strcat("ID does not exist: ", $1));
                        }
                      }
                |   NUM { 
                          $$ = $1;
                          MipsLoadI(&($$));
                        }
                ;

%%

int main(int argc, char** argv) 
{
  // Openning files
	extern FILE* yyin;
	extern FILE* yyout;
  yyin = fopen(argv[1], "r");
  assert(yyin != NULL);
  yyout = fopen(argv[2], "w");
  assert(yyout != NULL);
  mips = fopen(argv[3], "w");
  assert(mips != NULL);

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
