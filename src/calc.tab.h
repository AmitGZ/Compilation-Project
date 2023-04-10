
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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
     ERROR = 288,
     ID = 289,
     SENTENCE = 290,
     NUM = 291,
     RELOP = 292,
     ADDOP = 293,
     MULOP = 294,
     ASSIGNOP = 295,
     OROP = 296,
     ANDOP = 297
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 24 "calc.y"

  Type        _type;   /**< Description */
  Reg         _val;    /**< Description */
  const char* _name;   /**< Description */
  RelOp       _relOp;  /**< Description */
  MathOp      _mathOp;  /**< Description */



/* Line 1676 of yacc.c  */
#line 104 "calc.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


