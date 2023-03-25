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
     COMMA = 280,
     COLON = 281,
     O_PARENTHESES = 282,
     C_PARENTHESES = 283,
     O_BRACKET = 284,
     C_BRACKET = 285,
     EXCLAMATION = 286,
     SEMICOLON = 287,
     SENTENCE = 288,
     ID = 289,
     NUM = 290,
     RELOP = 291,
     ADDOP = 292,
     MULOP = 293,
     ASSIGNOP = 294,
     OROP = 295,
     ANDOP = 296
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
#define COMMA 280
#define COLON 281
#define O_PARENTHESES 282
#define C_PARENTHESES 283
#define O_BRACKET 284
#define C_BRACKET 285
#define EXCLAMATION 286
#define SEMICOLON 287
#define SENTENCE 288
#define ID 289
#define NUM 290
#define RELOP 291
#define ADDOP 292
#define MULOP 293
#define ASSIGNOP 294
#define OROP 295
#define ANDOP 296




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 13 "calc.y"
{
  Type  _type;   /**< Description */
  int   _ival;   /**< Description */
  char* _sval;   /**< Description */
  Num   _num;    /**< Description */
  AddOp _addOp;  /**< Description */
  RelOp _relOp;  /**< Description */
  MulOp _mulOp;  /**< Description */
}
/* Line 1529 of yacc.c.  */
#line 141 "calc.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

