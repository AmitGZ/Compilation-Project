%option noyywrap
%option yylineno
%{
/*
* Amit Zohar    313307720
* Ofek Ben Atar 322208430
*/
#include "MyStructs.h"
#include "cpm.tab.h"
#include <string.h>

// Global variables
size_t errorCount = 0U;
size_t line = 1U;
size_t col = 1U;

static RelOp GetRelOp(const char* s)
{
	if (strcmp(s, "==") == 0U)
		return EQ;
	if (strcmp(s, "<>") == 0U)
		return NEQ; 
	if (strcmp(s, "<=") == 0U)
		return LE;
	if (strcmp(s, ">=") == 0U)
		return GE;
	if (strcmp(s, "<") == 0U)
		return LT;
	if (strcmp(s, ">") == 0U)
		return GT;

	return -1;
}

static MathOp GetMathOp(const char* s)
{
	if (strcmp(s, "/") == 0U)
		return DIV;
	if (strcmp(s, "*") == 0U)
		return MUL;
	if (strcmp(s, "+") == 0U)
		return ADD;
	if (strcmp(s, "-") == 0U)
		return SUB;

	return -1;
}

%}

LETTER [a-zA-Z]
DIGIT [0-9]
SPACES [ \t\n\r]

%%
\/\*.*\*\/	{ col += strlen(yytext);                            }
[\n] 		{ fprintf(yyout, "\n%zu. ", ++line); col = 0U;                                  }
"break" 	{ fprintf(yyout, "%s", yytext);    col += strlen(yytext); return BREAK;         }
"case" 		{ fprintf(yyout, "%s", yytext);    col += strlen(yytext); return CASE;          }
"final" 	{ fprintf(yyout, "%s", yytext);    col += strlen(yytext); return FINAL;         }
"decl"		{ fprintf(yyout, "%s", yytext);    col += strlen(yytext); return DECL;          }
"default" 	{ fprintf(yyout, "%s", yytext);    col += strlen(yytext); return DEFAULT;       }
"else" 		{ fprintf(yyout, "%s", yytext);    col += strlen(yytext); return ELSE;          }
"end" 		{ fprintf(yyout, "%s", yytext);    col += strlen(yytext); return END;           }
"float" 	{ fprintf(yyout, "%s", yytext);    col += strlen(yytext); return FLOAT;         }
"foreach" 	{ fprintf(yyout, "%s", yytext);    col += strlen(yytext); return FOREACH;       }
"if" 		{ fprintf(yyout, "%s", yytext);    col += strlen(yytext); return IF;            }
"in" 		{ fprintf(yyout, "%s", yytext);    col += strlen(yytext); return IN;            }
"int" 		{ fprintf(yyout, "%s", yytext);    col += strlen(yytext); return INT;           }
"out" 		{ fprintf(yyout, "%s", yytext);    col += strlen(yytext); return OUT;           }
"program" 	{ fprintf(yyout, "Amit Zohar 313307720, Ofek Ben Atar 322208430\n1. %s", yytext); col += strlen(yytext); return PROGRAM;       }
"real" 		{ fprintf(yyout, "%s", yytext);    col += strlen(yytext); return REAL;          }
"start"		{ fprintf(yyout, "%s", yytext);    col += strlen(yytext); return START;         }
"string"	{ fprintf(yyout, "%s", yytext);    col += strlen(yytext); return STRING;        }
"switch" 	{ fprintf(yyout, "%s", yytext);    col += strlen(yytext); return SWITCH;        }
"then" 		{ fprintf(yyout, "%s", yytext);    col += strlen(yytext); return THEN;          }
"till" 		{ fprintf(yyout, "%s", yytext);    col += strlen(yytext); return TILL;          }
"while" 	{ fprintf(yyout, "%s", yytext);    col += strlen(yytext); return WHILE;         }
"with" 		{ fprintf(yyout, "%s", yytext);    col += strlen(yytext); return WITH;          }
"," 		{ fprintf(yyout, "%s", yytext);    col += strlen(yytext); return COMMA;         }
":" 		{ fprintf(yyout, "%s", yytext);    col += strlen(yytext); return COLON;         }
"(" 		{ fprintf(yyout, "%s", yytext);    col += strlen(yytext); return O_PARENTHESES; }
")" 		{ fprintf(yyout, "%s", yytext);    col += strlen(yytext); return C_PARENTHESES; }
"{" 		{ fprintf(yyout, "%s", yytext);    col += strlen(yytext); return O_BRACKET;     }
"}" 		{ fprintf(yyout, "%s", yytext);    col += strlen(yytext); return C_BRACKET;     }
"!" 		{ fprintf(yyout, "%s", yytext);    col += strlen(yytext); return EXCLAMATION;   }
";" 		{ fprintf(yyout, "%s", yytext);    col += strlen(yytext); return SEMICOLON;     }

\"({LETTER}|\.|,|\!|\?|{SPACES}|{DIGIT})*\" {
	fprintf(yyout, "%s", yytext);
	col += strlen(yytext);
	yylval._val._sval = strdup(yytext);
	yylval._val._type = STR;
	return SENTENCE;
	}

{LETTER}({LETTER}|{DIGIT}){0,8} {
	fprintf(yyout, "%s", yytext);
	col += strlen(yytext);
	yylval._name = strdup(yytext);
	return ID;
	}

{DIGIT}+ 	{
	fprintf(yyout, "%s", yytext); 
	col += strlen(yytext);
	yylval._val._sval = strdup(yytext);
	yylval._val._type = INTEGER; 
	return NUM;
	}

{DIGIT}+\.{DIGIT}* {
	fprintf(yyout, "%s", yytext);
	col += strlen(yytext);
	yylval._val._sval = strdup(yytext);
	yylval._val._type = FLOATING; 
	return NUM;
	}

(==|<>|<=|>=|<|>) {
	fprintf(yyout, "%s", yytext);
	col += strlen(yytext);
	yylval._relOp = GetRelOp(strdup(yytext));
	return RELOP;
	}

(\+|-) {
	fprintf(yyout, "%s", yytext);
	col += strlen(yytext);
	yylval._mathOp = GetMathOp(strdup(yytext));
	return ADDOP;
	}

(\*|\/) {
	fprintf(yyout, "%s", yytext);
	col += strlen(yytext);
	yylval._mathOp = GetMathOp(strdup(yytext));
	return MULOP;
	}

= 		 { fprintf(yyout,"%s", yytext); col += strlen(yytext); return ASSIGNOP; }
\|\|	 { fprintf(yyout,"%s", yytext); col += strlen(yytext); return OROP;     }
&& 		 { fprintf(yyout,"%s", yytext); col += strlen(yytext); return ANDOP;    }
{SPACES} { fprintf(yyout,"%s", yytext); col += strlen(yytext);                  }
. 		 { fprintf(yyout,"%s", yytext); yyerror("Error Lex| %s, line: %zu, col: %zu", yytext, line, col); errorCount++; } // Error
%%
