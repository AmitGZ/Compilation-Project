%{
// TODO
// README
// Assigning float to int?
// MipsCast finish all types
// Check if we can fix switch lw every case
// Create error file?
// main in cpm.c or cpm.y?
// Can i assign string to float?
// .lst, how should we copy the program to lst? (with or without COMMENT)
// Add id 313307720

// Tested:
// Const forbid assignment
// String assignments
// Logical operations (NOT)
// Const declaration

// Done:
// File output format for lexer?

// Questions
// Can we use TABLE_SIZE for hash table?

#include "MyStructs.h"
#include "Hash.h"
#include "MipsWriter.h"
#include <string.h>
#include <assert.h>

extern int yylineno;
extern int yylex();
extern int yyparse();
extern size_t errorCount;

FILE* mips;
Type CurrentType;
Bucket table[TABLE_SIZE];

%}

%union 
{
  Type        _type;    /**< Description */
  Reg         _reg;     /**< Description */
  Val         _val;     /**< Description */
  const char* _name;    /**< Description */
  RelOp       _relOp;   /**< Description */
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
%type <_reg>  factor
%type <_reg>  term
%type <_reg>  expression
%type <_reg> boolfactor
%type <_reg> boolterm
%type <_reg> boolexpr

%%
program         :   PROGRAM ID START declerations stmtlist END{ MipsExit(); }
                ;

declerations    :   DECL { MipsData(); } declarlist cdecl { MipsMain(); }
                | {}
                ;
 
declarlist      :   declarlist decl {}
                |   decl {}
                ;
 
decl            :   type { CurrentType = $1 } COLON list SEMICOLON

list            :   ID COMMA list {
                                    InsertToTable(table, $1, CurrentType, false);
                                    MipsDecl(CurrentType, $1, "0"); // Initialize with 0
                                  }
                |   ID  {
                          InsertToTable(table, $1, CurrentType, false);
                          MipsDecl(CurrentType, $1, "0"); // Initialize with 0
                        }
                ;

type            :   INT { $$ = INTEGER }
                |   REAL { $$ = FLOATING }
                |   STRING { $$ = STR }
                ;  
 
/* the value of id should not be changed during the program*/
cdecl           :   FINAL type ID ASSIGNOP NUM SEMICOLON cdecl{
                                                                Type my_type = $2;
                                                                Val val = $5;
                                                                const char* id = $3;

                                                                if(IsAssignValid(my_type, val._type))
                                                                {
                                                                  InsertToTable(table, id, my_type, true);
                                                                  MipsDecl(my_type, id, val._sval);
                                                                }
                                                                else
                                                                {
                                                                  yyerror("Invalid type conversion, cannot convert variable: \"%s\" of type %s to \"%s\"" , id, my_type, val._type);
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
                                                      Val* val = &($3);
                                                      Reg reg = MipsLoadImmediate(val); 
                                                      MipsAssign(node, reg);
                                                    }
                                                    else
                                                    {
                                                      yyerror("ID does not exist: %s", $1);
                                                    }
                                                  }
                |   control_stmt {}
                |   in_stmt {}
                |   out_stmt {}
                |   stmt_block {}
                ;

out_stmt        :   OUT O_PARENTHESES expression C_PARENTHESES SEMICOLON { MipsOut(&($3)); }
                |   OUT O_PARENTHESES SENTENCE C_PARENTHESES SEMICOLON{ 
                                                                        Reg reg = MipsLoadImmediate(&($3)); 
                                                                        MipsOut(&reg); 
                                                                      }
                ;

in_stmt         :   IN O_PARENTHESES ID C_PARENTHESES SEMICOLON { 
                                                                  Node* node = GetFromTable(table, $3);
                                                                  if (node == NULL) 
                                                                  {
                                                                    yyerror("ID does not exist %s", $3); 
                                                                  }
                                                                  else
                                                                  {
                                                                    MipsIn(node); 
                                                                  }
                                                                }
                ;

assignment_stmt :   ID ASSIGNOP expression SEMICOLON { 
                                                        Node* node = GetFromTable(table, $1);
                                                        if (node != NULL)
                                                        {
                                                          MipsAssign(node, $3);
                                                        }
                                                        else
                                                        {
                                                          // Throw exception
                                                        }
                                                      }
                ;

control_stmt    :   IF O_PARENTHESES boolexpr C_PARENTHESES THEN { MipsIf(&($3), 0U); } stmt ELSE { MipsIf(&($3), 1U); } stmt { MipsIf(&($3), 2U); }
		            |   WHILE { MipsWhile(NULL, 0U); } O_PARENTHESES boolexpr C_PARENTHESES { MipsWhile(&($4), 1U); } stmt_block { MipsWhile(NULL, 2U); MipsWhile(NULL, 3U); }
                |   FOREACH ID ASSIGNOP NUM TILL NUM{ 
                                                      // Loading startval and assigning to i
                                                      Reg startReg = MipsLoadImmediate(&($4));
                                                      Node* node = GetFromTable(table, $2);
                                                      if (node != NULL) 
                                                      {
                                                        MipsAssign(node, startReg);
                                                        // Declaring while and checking statement
                                                        MipsWhile(NULL, 0U);
                                                        Reg indexReg = MipsLoadVar(node);
                                                        Reg endReg = MipsLoadImmediate(&($6));
                                                        Reg resultReg = MipsRelOp(LT, indexReg, endReg);
                                                        MipsWhile(&resultReg, 1U);
                                                        MipsForEach(0U);
                                                      }
                                                      else { yyerror("ID not found %s", $2); }
                                                    } WITH step { MipsForEach(1U); } stmt { MipsForEach(2U); MipsWhile(NULL, 3U); }
                |   FOREACH ID ASSIGNOP NUM TILL ID{
                                                      // Loading startval and assigning to i
                                                      Reg startReg = MipsLoadImmediate(&($4));
                                                      Node* node = GetFromTable(table, $2);
                                                      if (node != NULL)
                                                      {
                                                        MipsAssign(node, startReg); 
                                                        // Declaring while and checking statement
                                                        MipsWhile(NULL, 0U);
                                                        Reg indexReg = MipsLoadVar(node);
                                                        Node* endNode = GetFromTable(table, $6);
                                                        if (endNode != NULL) 
                                                        { 
                                                          Reg endReg = MipsLoadVar(endNode);
                                                          Reg resultReg = MipsRelOp(LT, indexReg, endReg);
                                                          MipsWhile(&resultReg, 1U);
                                                          MipsForEach(0U);
                                                        }
                                                        else { yyerror("ID not found %s", $4); }
                                                      }
                                                      else { yyerror("ID not found %s", $2); }
                                                    } WITH step { MipsForEach(1U); } stmt { MipsForEach(2U); MipsWhile(NULL, 3U); }
                |   _switch {  }

stmt_block      :   O_BRACKET stmtlist C_BRACKET {}
                ;

_switch         :   SWITCH O_PARENTHESES ID C_PARENTHESES { 
                                                            Node* node = GetFromTable(table, $3); 
                                                            if (node != NULL)
                                                            {
                                                              MipsSwitch(node, true); 
                                                            }
                                                            else
                                                            {
                                                              yyerror("ID not found %s", $3);
                                                            }
                                                          } O_BRACKET cases C_BRACKET { MipsSwitch(NULL, false); }
                ;

cases           :   CASE NUM COLON{ MipsCase(&($2), true); } stmtlist BREAK SEMICOLON { MipsCase(NULL, false); } cases
                |   DEFAULT COLON { fprintf(mips, "\n# default:\n") } stmtlist {}
                ;

step            :   ID ASSIGNOP ID ADDOP NUM{ 
                                              Node* node0 = GetFromTable(table, $1);
                                              Node* node1 = GetFromTable(table, $3);
                                              Val* val = &($5);
                                              MathOp mathOp = $4;
                                              if ((node0 != NULL) && (node1 != NULL))
                                              {
                                                Reg numReg = MipsLoadImmediate(val);
                                                Reg varReg = MipsLoadVar(node1);
                                                Reg resReg = MipsMathOp($4, numReg, varReg);
                                                MipsAssign(node0, resReg);
                                              }
                                              else
                                              {
                                                if (node0 == NULL)
                                                {
                                                  yyerror("ID not found %s", $1);
                                                }
                                                if (node1 == NULL)
                                                {
                                                  yyerror("ID not found %s", $3);
                                                }
                                              }
                                            }
	              |   ID ASSIGNOP ID MULOP NUM{ 
                                              Node* node0 = GetFromTable(table, $1);
                                              Node* node1 = GetFromTable(table, $3);
                                              Val* val = &($5);
                                              MathOp mathOp = $4;
                                              if ((node0 != NULL) && (node1 != NULL))
                                              {
                                                Reg numReg = MipsLoadImmediate(val);
                                                Reg varReg = MipsLoadVar(node1);
                                                Reg resReg = MipsMathOp($4, numReg, varReg);
                                                MipsAssign(node0, resReg);
                                              }
                                              else
                                              {
                                                if (node0 == NULL)
                                                {
                                                  yyerror("ID not found %s", $1);
                                                }
                                                if (node1 == NULL)
                                                {
                                                  yyerror("ID not found %s", $3);
                                                }
                                              }
                                            }
                ;

boolexpr        :   boolexpr OROP boolterm  { $$ = MipsLogOp(OR, $1, $3); }
                |   boolterm { $$ = $1; }
                ;

boolterm        :   boolterm ANDOP boolfactor { $$ = MipsLogOp(AND, $1, $3); }
                |   boolfactor { $$ = $1; }
                ;
        
boolfactor      :   EXCLAMATION O_PARENTHESES boolfactor C_PARENTHESES{ $$ = MipsLogOp(NOR, $3, ZeroReg); /* NOR ZERO = NOT */ }
                |   expression RELOP expression { $$ = MipsRelOp($2, $1, $3); }
                ;  

expression      :   expression ADDOP term { $$ = MipsMathOp($2, $1, $3); }
                |   term { $$ = $1; }
                ;

term            :   term MULOP factor { $$ = MipsMathOp($2, $1, $3); }
                |   factor  { $$ = $1; }
                ;

factor          :   O_PARENTHESES expression C_PARENTHESES{ $$ = $2; }
                |   ID{ 
                        Node* node = GetFromTable(table, $1);
                        if (node != NULL)
                        {
                          $$ = MipsLoadVar(node);
                        }
                        else
                        {
                          yyerror("ID does not exist %s\n", $1);
                        }
                      }
                |   NUM { $$ = MipsLoadImmediate(&($1)); }
                ;

%%

int main(int argc, char** argv) 
{
  // Writing student names
  fprintf(stderr, "Ofek Ben Atar, Amit Zohar\n");

  if (argc < 2)
  {
    fprintf(stderr, "Not enough arguments argc = %d\n", argc);
    return 1;
  }

  if (strlen(argv[1]) < 4 || strcmp(&argv[1][strlen(argv[1]) - 4], ".cpl"))
  {
    fprintf(stderr, "Invalid file name = %s\n", argv[1]);
    return 1;
  }

  // Openning files
	extern FILE* yyin;
	extern FILE* yyout;

  // Openning .cpl file
  yyin = fopen(argv[1], "r");
  if (yyin == NULL)
  {
    fprintf(stderr, "Error openning file: %s", argv[1]);
    return 1;
  }

  // Openning .lst file
  strcpy(&argv[1][strlen(argv[1]) - 3], "lst");
  yyout = fopen(argv[1], "w");
  if (yyout == NULL)
  {
    fprintf(stderr, "Error openning file: %s", argv[1]);
    return 1;
  }

  // Openning .s file
  strcpy(&argv[1][strlen(argv[1]) - 3], "s");
  mips = fopen(argv[1], "w");
  if (mips == NULL)
  {
    fprintf(stderr, "Error openning file: %s", argv[1]);
    return 1;
  }

  // Printing signature line
  fprintf(mips, "# Compiled by Amit Zohar & Ofek Ben Atar\n");

	do
	{
		yyparse();
	} while(!feof(yyin));
  
  // Closing files
  fclose(mips);
  fclose(yyin);
  fclose(yyout);

  // Printing error count and removing compiled file if errors
  fprintf(stderr, "Total Error Count: %zu\n", errorCount);
  if(errorCount > 0U)
  {
    remove(argv[1]);
  }

  // Freeing
  FreeTable(table);
	return 0;
}
