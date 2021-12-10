/* A Bison parser, made by GNU Bison 3.7.4.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_SRC_ENGINE_DATABLOCK_PARSER_PARSER_TAB_HPP_INCLUDED
# define YY_YY_SRC_ENGINE_DATABLOCK_PARSER_PARSER_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 16 "src/engine/datablock/parser/parser.y"

  #include "ast.h"
  #include <cstdio>

  using glm::vec2;
  using glm::vec3;
  using glm::vec4;
  using glm::ivec2;
  using glm::ivec3;
  using glm::ivec4;
  using glm::mat3;
  using glm::mat4;

#line 63 "src/engine/datablock/parser/parser.tab.hpp"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    BOOL = 258,                    /* BOOL  */
    INT = 259,                     /* INT  */
    FLOAT = 260,                   /* FLOAT  */
    TEXT = 261,                    /* TEXT  */
    NAME = 262,                    /* NAME  */
    AT = 263,                      /* "@"  */
    COLON = 264,                   /* ":"  */
    EQUAL_OP = 265,                /* "="  */
    LEFT_BRACKET = 266,            /* "{"  */
    RIGHT_BRACKET = 267,           /* "}"  */
    LEFT_SQUARE_BRACKET = 268,     /* "["  */
    RIGHT_SQUARE_BRACKET = 269,    /* "]"  */
    COMMA = 270,                   /* ","  */
    INT_TYPE = 271,                /* INT_TYPE  */
    FLOAT_TYPE = 272,              /* FLOAT_TYPE  */
    TEXT_TYPE = 273,               /* TEXT_TYPE  */
    POINT_2D_TYPE = 274,           /* POINT_2D_TYPE  */
    POINT_3D_TYPE = 275,           /* POINT_3D_TYPE  */
    POINT_4D_TYPE = 276,           /* POINT_4D_TYPE  */
    INT_POINT_2D_TYPE = 277,       /* INT_POINT_2D_TYPE  */
    INT_POINT_3D_TYPE = 278,       /* INT_POINT_3D_TYPE  */
    INT_POINT_4D_TYPE = 279,       /* INT_POINT_4D_TYPE  */
    MAT4_TYPE = 280,               /* MAT4_TYPE  */
    MAT3_TYPE = 281,               /* MAT3_TYPE  */
    BOOL_TYPE = 282                /* BOOL_TYPE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 34 "src/engine/datablock/parser/parser.y"

  bool bval;
  int ival;
  float fval;
  char* sval;
  vec2 f2val;
  vec3 f3val;
  vec4 f4val;
  ivec2 i2val;
  ivec3 i3val;
  ivec4 ival4;
  Ast::Config* configNode;
  Ast::AnnotatedParam* paramListNode;
  Ast::Block* blockNode;
  Ast::Attribute* attributeNode;
  Ast::AttributeType attributeType;
  Ast::AttributeValue* attributeValue;

#line 126 "src/engine/datablock/parser/parser.tab.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (Ast::Config* rootAst);

#endif /* !YY_YY_SRC_ENGINE_DATABLOCK_PARSER_PARSER_TAB_HPP_INCLUDED  */
