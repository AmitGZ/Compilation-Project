%option noyywrap

%{
#include "header.h"
//#define YY_DECL int yylex()
#include "calc.tab.h"

// Global variables
int line = 1;

static RelOp GetRelOp(const char* s)
{
	if (!strcmp(s, "=="))
		return EQ;
	if (!strcmp(s, "<>"))
		return NEQ; 
	if (!strcmp(s, "<="))
		return LE;
	if (!strcmp(s, ">="))
		return GE;
	if (!strcmp(s, "<"))
		return LT;
	if (!strcmp(s, ">"))
		return GT;

	// TODO Handle exception
	return EQ;
}

static MulOp GetMulOp(const char* s)
{
	if (!strcmp(s, "/"))
		return DIV;
	if (!strcmp(s, "*"))
		return MUL;

	// TODO Handle exception
	return DIV;
}

static AddOp GetAddOp(const char* s)
{
	if (!strcmp(s, "+"))
		return ADD;
	if (!strcmp(s, "-"))
		return SUB;

	// TODO Handle exception
	return ADD;
}

%}

LETTER [a-zA-Z]
DIGIT [0-9]
SPACES [ \t\n]

%%
\/\*.*\*\/	{fprintf(yyout," COMMENT");}
[\n] 		{fprintf(yyout, "\n%d. ", ++line); }
"break" 	{fprintf(yyout,"%s",yytext); return BREAK;}
"case" 		{fprintf(yyout,"%s",yytext); return CASE;}
"final" 	{fprintf(yyout,"%s",yytext); return FINAL;}
"decl"		{fprintf(yyout,"%s",yytext); return DECL;}
"default" 	{fprintf(yyout,"%s",yytext); return DEFAULT;}
"else" 		{fprintf(yyout,"%s",yytext); return ELSE;}
"end" 		{fprintf(yyout,"%s",yytext); return END;}
"float" 	{fprintf(yyout,"%s",yytext); return FLOAT;}
"foreach" 	{fprintf(yyout,"%s",yytext); return FOREACH;}
"if" 		{fprintf(yyout,"%s",yytext); return IF;}
"in" 		{fprintf(yyout,"%s",yytext); return IN;}
"int" 		{fprintf(yyout,"%s",yytext); return INT;}
"out" 		{fprintf(yyout,"%s",yytext); return OUT;}
"program" 	{fprintf(yyout,"%s",yytext); return PROGRAM;}
"real" 		{fprintf(yyout,"%s",yytext); return REAL;}
"start"		{fprintf(yyout,"%s",yytext); return START;}
"string"	{fprintf(yyout,"%s",yytext); return STRING;}
"switch" 	{fprintf(yyout,"%s",yytext); return SWITCH;}
"then" 		{fprintf(yyout,"%s",yytext); return THEN;}
"till" 		{fprintf(yyout,"%s",yytext); return TILL;}
"while" 	{fprintf(yyout,"%s",yytext); return WHILE;}
"with" 		{fprintf(yyout,"%s",yytext); return WITH;}

\"({LETTER}|\.|,|\!|\?|{SPACES}|{DIGIT})*\" {
	fprintf(yyout,"%s",yytext);
	yylval.sval = strdup(yytext);
	return SENTENCE;
	}

{LETTER}({LETTER}|{DIGIT}){0,8} {
	fprintf(yyout," ID(%s)",yytext);
	yylval.sval = strdup(yytext);
	return ID;
	}

{DIGIT}+ 	{
	fprintf(yyout," %s",yytext); 
	yylval.number._sval = strdup(yytext);
	yylval.number._type = INTEGER; 
	return NUM;
	}

{DIGIT}+\.{DIGIT}* {
	fprintf(yyout," NUM(%s)",yytext);
	yylval.number._sval = strdup(yytext);
	yylval.number._type = FLOATING; 
	return NUM;
	}

(==|<>|<=|>=|<|>) {
	fprintf(yyout," %s",yytext);
	yylval.relOp = GetRelOp(strdup(yytext));
	return RELOP;
	}

(\+|-) {
	fprintf(yyout," %s",yytext);
	yylval.addOp = GetAddOp(strdup(yytext));
	return ADDOP;
	}

(\*|\/) {
	fprintf(yyout,"%s",yytext);
	yylval.mulOp = GetMulOp(strdup(yytext));
	return MULOP;
	}

= 		{fprintf(yyout,"%s",yytext); return ASSIGNOP;}
\|\| 	{fprintf(yyout,"%s",yytext); return OROP;}
&& 		{fprintf(yyout,"%s",yytext); return ANDOP;}
. 		{fprintf(yyout,"%s",yytext);} // Error
%%
