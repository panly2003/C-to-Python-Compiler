/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_YACC_TAB_HPP_INCLUDED
# define YY_YY_YACC_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    IF = 258,
    EXTERN = 259,
    FOR = 260,
    RETURN = 261,
    SEMI = 262,
    COMMA = 263,
    LSBORDER = 264,
    RSBORDER = 265,
    LMBORDER = 266,
    RMBORDER = 267,
    LLBORDER = 268,
    RLBORDER = 269,
    INT = 270,
    CHAR = 271,
    ID = 272,
    INTEGER = 273,
    STRING = 274,
    LT = 275,
    LE = 276,
    GT = 277,
    GE = 278,
    NEQUAL = 279,
    GVAL = 280,
    PLUS = 281,
    MINUS = 282,
    MUL = 283,
    DIV = 284,
    ADDR = 285,
    INVALID = 286
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 40 "yacc.ypp"

    int intValue;
    char *stringValue;
    int token;
    BlockNode* block;
    ExpressionNode* expr;
    StatementNode* stat;
    IdentifierNode* ident;
    VariableDeclarationNode* varDec;
    ArrayDeclarationNode* arrayDec;
    ArrayIndexNode* aryIndx;
    CallFuncNode* calFunc;
    BinaryOperatorNode* bOpr;
    AssignmentNode* asng;
    IDType type;
    std::vector<shared_ptr<VariableDeclarationNode>>* vardec_vec;
    std::vector<shared_ptr<ExpressionNode>>* expr_vec;


#line 109 "yacc.tab.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_YACC_TAB_HPP_INCLUDED  */
