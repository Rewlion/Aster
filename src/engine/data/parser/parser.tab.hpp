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

#ifndef YY_ED_SRC_ENGINE_DATA_PARSER_PARSER_TAB_HPP_INCLUDED
# define YY_ED_SRC_ENGINE_DATA_PARSER_PARSER_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef EDDEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define EDDEBUG 1
#  else
#   define EDDEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define EDDEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined EDDEBUG */
#if EDDEBUG
extern int eddebug;
#endif
/* "%code requires" blocks.  */
#line 20 "src/engine/data/parser/parser.y"

  #include "parser.h"

  #include <ranges>
  #include <variant>

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

  #define DECL_NODE_NAMED(type, typeName)                        \
    struct typeName ## Node                                      \
    {                                                            \
      using Ptr = std::shared_ptr<typeName ## Node>;             \
                                                                 \
      typeName ## Node() = default;                              \
      typeName ## Node(type&& v) : val(std::move(v)) {}          \
                                                                 \
      static                                                     \
      auto make(type&& v) -> Ptr                                 \
      {                                                          \
        return std::make_shared<typeName ## Node>(std::move(v)); \
      }                                                          \
                                                                 \
      type val;                                                  \
    };                                                           \
    using typeName ## NodePtr = std::shared_ptr<typeName ## Node>
  
  #define DECL_NODE(type) DECL_NODE_NAMED(type, type)

  DECL_NODE(Number2);
  DECL_NODE(Number3);
  DECL_NODE(Number4);
  DECL_NODE_NAMED(float3x3, Mat3);
  DECL_NODE_NAMED(float4x4, Mat4);
  DECL_NODE_NAMED(ed::IntArray, IntArray);
  DECL_NODE_NAMED(ed::FloatArray, FloatArray);
  DECL_NODE_NAMED(ed::TextArray, TextArray);
  DECL_NODE_NAMED(eastl::vector<Number>, NumberArray);
  DECL_NODE_NAMED(ed::Scope, Scope);
  DECL_NODE_NAMED(ed::Variable, Variable);

  using ScopeParam = std::variant<ScopeNodePtr, VariableNodePtr>;
  using ScopeParamArray = eastl::vector<ScopeParam>;
  DECL_NODE(ScopeParamArray);

  #define EDSTYPE std::variant<                                                \
    bool, Number, string,                                                      \
    Number2NodePtr, Number3NodePtr, Number4NodePtr,                            \
    Mat3NodePtr, Mat4NodePtr,                                                  \
    IntArrayNodePtr, FloatArrayNodePtr, TextArrayNodePtr, NumberArrayNodePtr,  \
    ScopeNodePtr, VariableNodePtr,                                             \
    ScopeParam, ScopeParamArrayNodePtr                                         \
  >

  static
  auto scope_params_to_scope(ed::Parser& parser, ScopeParamArray& params) -> ScopeNodePtr
  {
    ed::Scope scope = parser.makeScope();
    for (ScopeParam& param: params | std::views::reverse)
    {
      if (auto* nestedScope = std::get_if<ScopeNodePtr>(&param))
        scope.addScope(std::move((*nestedScope)->val));
      else
        parser.addVariableDefinition(std::move(std::get<VariableNodePtr>(param)->val), scope);
    }

    return ScopeNode::make(std::move(scope));
  }

  template<class T>
  static
  auto number_to(const EDSTYPE& edstype) -> T
  {
    const Number& n = std::get<Number>(edstype);
    if (auto* fl = std::get_if<T>(&n))
      return *fl;
    else
    {
      using FromType = typename std::conditional<std::is_same_v<T,int>, float, int>::type;
      return static_cast<T>(std::get<FromType>(n));
    }
  }

  static
  auto number_to_float(const EDSTYPE& edstype) -> float
  {
    return number_to<float>(edstype);
  }

  static
  auto number_to_int(const EDSTYPE& edstype) -> int
  {
    return number_to<int>(edstype);
  }

  static
  auto number2_to_float2(const EDSTYPE& edstype) -> float2
  {
    const auto& n = std::get<Number2NodePtr>(edstype);
    return float2{
      number_to_float(n->val.v0),
      number_to_float(n->val.v1)
    };
  }

  static
  auto number3_to_float3(const EDSTYPE& edstype) -> float3
  {
    const auto& n = std::get<Number3NodePtr>(edstype);
    return float3{
      number_to_float(n->val.v0),
      number_to_float(n->val.v1),
      number_to_float(n->val.v2)
    };
  }

  static
  auto number4_to_float4(const EDSTYPE& edstype) -> float4
  {
    const auto& n = std::get<Number4NodePtr>(edstype);
    return float4{
      number_to_float(n->val.v0),
      number_to_float(n->val.v1),
      number_to_float(n->val.v2),
      number_to_float(n->val.v3)
    };
  }

  static
  auto number2_to_int2(const EDSTYPE& edstype) -> int2
  {
    const auto& n = std::get<Number2NodePtr>(edstype);
    return int2{
      number_to_int(n->val.v0),
      number_to_int(n->val.v1)
    };
  }

  static
  auto number3_to_int3(const EDSTYPE& edstype) -> int3
  {
    const auto& n = std::get<Number3NodePtr>(edstype);
    return int3{
      number_to_int(n->val.v0),
      number_to_int(n->val.v1),
      number_to_int(n->val.v2)
    };
  }

  static
  auto number4_to_int4(const EDSTYPE& edstype) -> int4
  {
    const auto& n = std::get<Number4NodePtr>(edstype);
    return int4{
      number_to_int(n->val.v0),
      number_to_int(n->val.v1),
      number_to_int(n->val.v2),
      number_to_int(n->val.v3)
    };
  }

  template<class T>
  static
  auto number_array_to_T_array(const EDSTYPE& edstype) -> eastl::vector<T>
  {
    const auto& from = std::get<NumberArrayNodePtr>(edstype);
    eastl::vector<T> to;

    to.reserve(from->val.size());
    for (const Number& n: from->val)
      to.push_back(number_to<T>(n));

    return to;
  }

  static
  auto number_array_to_int_array(const EDSTYPE& edstype) -> ed::IntArray
  {
    return number_array_to_T_array<int>(edstype);
  }

  static
  auto number_array_to_float_array(const EDSTYPE& edstype) -> ed::FloatArray
  {
    return number_array_to_T_array<float>(edstype);
  }

#line 258 "src/engine/data/parser/parser.tab.hpp"

/* Token kinds.  */
#ifndef EDTOKENTYPE
# define EDTOKENTYPE
  enum edtokentype
  {
    EDEMPTY = -2,
    EDEOF = 0,                     /* "end of file"  */
    EDerror = 256,                 /* error  */
    EDUNDEF = 257,                 /* "invalid token"  */
    BOOL_VAL = 258,                /* BOOL_VAL  */
    INT_VAL = 259,                 /* INT_VAL  */
    FLOAT_VAL = 260,               /* FLOAT_VAL  */
    TEXT_VAL = 261,                /* TEXT_VAL  */
    NAME_VAL = 262,                /* NAME_VAL  */
    AT = 263,                      /* "@"  */
    COLON = 264,                   /* ":"  */
    EQUAL_OP = 265,                /* "="  */
    LEFT_PARENTHESIS = 266,        /* "("  */
    RIGHT_PARENTHESIS = 267,       /* ")"  */
    LEFT_BRACKET = 268,            /* "{"  */
    RIGHT_BRACKET = 269,           /* "}"  */
    LEFT_SQUARE_BRACKET = 270,     /* "["  */
    RIGHT_SQUARE_BRACKET = 271,    /* "]"  */
    COMMA = 272                    /* ","  */
  };
  typedef enum edtokentype edtoken_kind_t;
#endif

/* Value type.  */
#if ! defined EDSTYPE && ! defined EDSTYPE_IS_DECLARED
typedef int EDSTYPE;
# define EDSTYPE_IS_TRIVIAL 1
# define EDSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined EDLTYPE && ! defined EDLTYPE_IS_DECLARED
typedef struct EDLTYPE EDLTYPE;
struct EDLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define EDLTYPE_IS_DECLARED 1
# define EDLTYPE_IS_TRIVIAL 1
#endif


extern EDSTYPE edlval;
extern EDLTYPE edlloc;
int edparse (ed::Parser& parser);

#endif /* !YY_ED_SRC_ENGINE_DATA_PARSER_PARSER_TAB_HPP_INCLUDED  */
