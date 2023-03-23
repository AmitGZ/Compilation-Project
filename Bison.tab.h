/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     BREAK = 258,
     CASE = 259,
     FINAL = 260,
     DECL = 261,
     DEFAULT = 262,
     ELSE = 263,
     END = 264,
     FLOAT = 265,
     FOREACH = 266,
     IF = 267,
     IN = 268,
     INT = 269,
     OUT = 270,
     PROGRAM = 271,
     REAL = 272,
     START = 273,
     STRING = 274,
     SWITCH = 275,
     THEN = 276,
     TILL = 277,
     WHILE = 278,
     WITH = 279,
     SENTENCE = 280,
     ID = 281,
     NUM = 282,
     RELOP = 283,
     ADDOP = 284,
     MULOP = 285,
     ASSIGNOP = 286,
     OROP = 287,
     ANDOP = 288
   };
#endif
/* Tokens.  */
#define BREAK 258
#define CASE 259
#define FINAL 260
#define DECL 261
#define DEFAULT 262
#define ELSE 263
#define END 264
#define FLOAT 265
#define FOREACH 266
#define IF 267
#define IN 268
#define INT 269
#define OUT 270
#define PROGRAM 271
#define REAL 272
#define START 273
#define STRING 274
#define SWITCH 275
#define THEN 276
#define TILL 277
#define WHILE 278
#define WITH 279
#define SENTENCE 280
#define ID 281
#define NUM 282
#define RELOP 283
#define ADDOP 284
#define MULOP 285
#define ASSIGNOP 286
#define OROP 287
#define ANDOP 288




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 15 "Bison.y"
{
  // all the types
  int ival;
  char* sval;
  char kind;
  abc number;
}
/* Line 1529 of yacc.c.  */
#line 123 "Bison.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

