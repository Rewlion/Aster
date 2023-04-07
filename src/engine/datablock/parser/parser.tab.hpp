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

#ifndef YY_BK_SRC_ENGINE_DATABLOCK_PARSER_PARSER_TAB_HPP_INCLUDED
# define YY_BK_SRC_ENGINE_DATABLOCK_PARSER_PARSER_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef BKDEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define BKDEBUG 1
#  else
#   define BKDEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define BKDEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined BKDEBUG */
#if BKDEBUG
extern int bkdebug;
#endif
/* "%code requires" blocks.  */
#line 21 "src/engine/datablock/parser/parser.y"

  #include "parser.h"

  #include <ranges>
  #include <variant>

  using BlockParam = std::variant<DataBlock, DataBlock::Attribute>;
  using BlockParams = eastl::vector<BlockParam>;

  static
  auto block_params_to_bk(BlockParams&& params) -> DataBlock
  {
    DataBlock bk;
    for (auto& param: params | std::views::reverse)
    {
      if (auto* bkChild = std::get_if<DataBlock>(&param))
        bk.insertBlock(*bkChild);
      else
       bk.insertAttribute(std::get<DataBlock::Attribute>(param));
    }

    return bk;
  }

  using Number = std::variant<int, float>;
  struct Number2
  {
    Number v0, v1;
  };

  struct Number3
  {
    Number v0, v1, v2;
  };

  struct Number4
  {
    Number v0, v1, v2, v3;
  };

  #define BKSTYPE std::variant<         \
    DataBlock::Attribute::Type,         \
    bool, string,                       \
    Number, Number2, Number3, Number4,  \
    mat3, mat4,                         \
    DataBlock, DataBlock::Attribute,    \
    BlockParam, BlockParams>

  static
  const char* bkstype_to_string(const BKSTYPE& v)
  {
    if (auto* t = std::get_if<DataBlock::Attribute::Type>(&v))
      return "attribute type";
    if (auto* t = std::get_if<bool>(&v))
      return "bool";
    if (auto* t = std::get_if<string>(&v))
      return "text";
    if (auto* t = std::get_if<Number>(&v))
      return "number";
    if (auto* t = std::get_if<Number2>(&v))
      return "number2";
    if (auto* t = std::get_if<Number3>(&v))
      return "number3";
    if (auto* t = std::get_if<Number4>(&v))
      return "number4";
    if (auto* t = std::get_if<mat3>(&v))
      return "mat3";
    if (auto* t = std::get_if<mat4>(&v))
      return "mat4";
    if (auto* t = std::get_if<DataBlock>(&v))
      return "block";
    if (auto* t = std::get_if<DataBlock::Attribute>(&v))
      return "attribute";
    if (auto* t = std::get_if<BlockParam>(&v))
      return "block param";
    if (auto* t = std::get_if<BlockParams>(&v))
      return "block params";
    else
      return "";
  }

  static
  auto number_to_float(const BKSTYPE& bkstype) -> float
  {
    const Number& n = std::get<Number>(bkstype);
    if (auto* fl = std::get_if<float>(&n))
      return *fl;
    else
      return static_cast<float>(std::get<int>(n));
  }

  static
  auto number_to_int(const BKSTYPE& bkstype) -> int
  {
    const Number& n = std::get<Number>(bkstype);
    if (auto* v = std::get_if<int>(&n))
      return *v;
    else
      return static_cast<int>(std::get<float>(n));
  }

  static
  auto number2_to_float2(const BKSTYPE& bkstype) -> float2
  {
    const Number2& n = std::get<Number2>(bkstype);
    return float2{
      number_to_float(n.v0),
      number_to_float(n.v1)
    };
  }

  static
  auto number3_to_float3(const BKSTYPE& bkstype) -> float3
  {
    const Number3& n = std::get<Number3>(bkstype);
    return float3{
      number_to_float(n.v0),
      number_to_float(n.v1),
      number_to_float(n.v2)
    };
  }

  static
  auto number4_to_float4(const BKSTYPE& bkstype) -> float4
  {
    const Number4& n = std::get<Number4>(bkstype);
    return float4{
      number_to_float(n.v0),
      number_to_float(n.v1),
      number_to_float(n.v2),
      number_to_float(n.v3)
    };
  }

  static
  auto number2_to_int2(const BKSTYPE& bkstype) -> int2
  {
    const Number2& n = std::get<Number2>(bkstype);
    return int2{
      number_to_int(n.v0),
      number_to_int(n.v1)
    };
  }

  static
  auto number3_to_int3(const BKSTYPE& bkstype) -> int3
  {
    const Number3& n = std::get<Number3>(bkstype);
    return int3{
      number_to_int(n.v0),
      number_to_int(n.v1),
      number_to_int(n.v2)
    };
  }

  static
  auto number4_to_int4(const BKSTYPE& bkstype) -> int4
  {
    const Number4& n = std::get<Number4>(bkstype);
    return int4{
      number_to_int(n.v0),
      number_to_int(n.v1),
      number_to_int(n.v2),
      number_to_int(n.v3)
    };
  }

#line 225 "src/engine/datablock/parser/parser.tab.hpp"

/* Token kinds.  */
#ifndef BKTOKENTYPE
# define BKTOKENTYPE
  enum bktokentype
  {
    BKEMPTY = -2,
    BKEOF = 0,                     /* "end of file"  */
    BKerror = 256,                 /* error  */
    BKUNDEF = 257,                 /* "invalid token"  */
    BOOL_VAL = 258,                /* BOOL_VAL  */
    INT_VAL = 259,                 /* INT_VAL  */
    FLOAT_VAL = 260,               /* FLOAT_VAL  */
    TEXT_VAL = 261,                /* TEXT_VAL  */
    NAME_VAL = 262,                /* NAME_VAL  */
    AT = 263,                      /* "@"  */
    COLON = 264,                   /* ":"  */
    SEMICOLON = 265,               /* ";"  */
    EQUAL_OP = 266,                /* "="  */
    LEFT_PARENTHESIS = 267,        /* "("  */
    RIGHT_PARENTHESIS = 268,       /* ")"  */
    LEFT_BRACKET = 269,            /* "{"  */
    RIGHT_BRACKET = 270,           /* "}"  */
    LEFT_SQUARE_BRACKET = 271,     /* "["  */
    RIGHT_SQUARE_BRACKET = 272,    /* "]"  */
    COMMA = 273,                   /* ","  */
    TEXT_TYPE = 274,               /* "text"  */
    FLOAT_TYPE = 275,              /* "float"  */
    POINT_2D_TYPE = 276,           /* "float2"  */
    POINT_3D_TYPE = 277,           /* "float3"  */
    POINT_4D_TYPE = 278,           /* "float4"  */
    INT_TYPE = 279,                /* "int"  */
    INT_POINT_2D_TYPE = 280,       /* "int2"  */
    INT_POINT_3D_TYPE = 281,       /* "int3"  */
    INT_POINT_4D_TYPE = 282,       /* "int4"  */
    MAT3_TYPE = 283,               /* "mat3"  */
    MAT4_TYPE = 284,               /* "mat4"  */
    BOOL_TYPE = 285                /* "bool"  */
  };
  typedef enum bktokentype bktoken_kind_t;
#endif

/* Value type.  */
#if ! defined BKSTYPE && ! defined BKSTYPE_IS_DECLARED
typedef int BKSTYPE;
# define BKSTYPE_IS_TRIVIAL 1
# define BKSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined BKLTYPE && ! defined BKLTYPE_IS_DECLARED
typedef struct BKLTYPE BKLTYPE;
struct BKLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define BKLTYPE_IS_DECLARED 1
# define BKLTYPE_IS_TRIVIAL 1
#endif


extern BKSTYPE bklval;
extern BKLTYPE bklloc;
int bkparse (BlkParser& parser);

#endif /* !YY_BK_SRC_ENGINE_DATABLOCK_PARSER_PARSER_TAB_HPP_INCLUDED  */
