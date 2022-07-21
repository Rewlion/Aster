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

#ifndef YY_SH_SRC_SHADERS_COMPILER_PARSER_PARSER_TAB_HPP_INCLUDED
# define YY_SH_SRC_SHADERS_COMPILER_PARSER_PARSER_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef SHDEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define SHDEBUG 1
#  else
#   define SHDEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define SHDEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined SHDEBUG */
#if SHDEBUG
extern int shdebug;
#endif
/* "%code requires" blocks.  */
#line 21 "src/shaders_compiler/parser/parser.y"

  #include <shaders_compiler/compiler.h>
  #include <cstdio>
    using namespace ShadersSystem;

#line 63 "src/shaders_compiler/parser/parser.tab.hpp"

/* Token kinds.  */
#ifndef SHTOKENTYPE
# define SHTOKENTYPE
  enum shtokentype
  {
    SHEMPTY = -2,
    SHEOF = 0,                     /* "end of file"  */
    SHerror = 256,                 /* error  */
    SHUNDEF = 257,                 /* "invalid token"  */
    TOKEN_FLOAT_VAL = 258,         /* TOKEN_FLOAT_VAL  */
    TOKEN_INT_VAL = 259,           /* TOKEN_INT_VAL  */
    TOKEN_NAME_VAL = 260,          /* TOKEN_NAME_VAL  */
    TOKEN_HLSL_CODE = 261,         /* TOKEN_HLSL_CODE  */
    TOKEN_AT = 262,                /* "@"  */
    TOKEN_COLON = 263,             /* ":"  */
    TOKEN_SEMICOLON = 264,         /* ";"  */
    TOKEN_EQUAL_OP = 265,          /* "="  */
    TOKEN_LEFT_PARENTHESIS = 266,  /* "("  */
    TOKEN_RIGHT_PARENTHESIS = 267, /* ")"  */
    TOKEN_LEFT_BRACKET = 268,      /* "{"  */
    TOKEN_RIGHT_BRACKET = 269,     /* "}"  */
    TOKEN_COMMA = 270,             /* ","  */
    TOKEN_MINUS = 271,             /* "-"  */
    TOKEN_INPUT = 272,             /* "input"  */
    TOKEN_BUFFER = 273,            /* "buffer"  */
    TOKEN_SCOPE = 274,             /* "scope"  */
    TOKEN_SHADER = 275,            /* "shader"  */
    TOKEN_SHADER_VERTEX = 276,     /* "vertex"  */
    TOKEN_SHADER_PIXEL = 277,      /* "pixel"  */
    TOKEN_REGISTER = 278,          /* "register"  */
    TOKEN_TEXTURE = 279,           /* "texture"  */
    TOKEN_CBUFFER = 280,           /* "cbuffer"  */
    TOKEN_EXTERN = 281,            /* "extern"  */
    TOKEN_CHANNEL = 282,           /* "channel"  */
    TOKEN_DESCRIPTOR_SET = 283,    /* "descriptor_set"  */
    TOKEN_TECHNIQUE = 284,         /* "technique"  */
    TOKEN_SUPPORT = 285,           /* "support"  */
    TOKEN_ACTIVATE = 286,          /* "activate"  */
    TOKEN_COMPILE = 287,           /* "compile"  */
    TOKEN_PRIMITIVE_TOPOLOGY = 288, /* "primitive_topology"  */
    TOKEN_PT_POINT_LIST = 289,     /* "point_list"  */
    TOKEN_PT_LINE_LIST = 290,      /* "line_list"  */
    TOKEN_PT_LINE_STRIP = 291,     /* "line_strip"  */
    TOKEN_PT_TRIANGLE_LIST = 292,  /* "triangle_list"  */
    TOKEN_PT_TRIANGLE_FAN = 293,   /* "triangle_fan"  */
    TOKEN_PT_LINE_LIST_WITH_ADJACENCY = 294, /* "line_list_with_adjacency"  */
    TOKEN_PT_LINE_STRIP_WITH_ADJACENCY = 295, /* "line_strip_with_adjacency"  */
    TOKEN_PT_TRIANGLE_LIST_WITH_ADJACENCY = 296, /* "triangle_list_with_adjacency"  */
    TOKEN_PT_TRIANGLE_STRIP_WITH_ADJACENCY = 297, /* "triangle_strip_with_adjacency"  */
    TOKEN_PT_PATCH_LIST = 298,     /* "patch_list"  */
    TOKEN_TARGET_VS_6_0 = 299,     /* "vs_6_0"  */
    TOKEN_TARGET_VS_6_1 = 300,     /* "vs_6_1"  */
    TOKEN_TARGET_VS_6_2 = 301,     /* "vs_6_2"  */
    TOKEN_TARGET_VS_6_3 = 302,     /* "vs_6_3"  */
    TOKEN_TARGET_VS_6_4 = 303,     /* "vs_6_4"  */
    TOKEN_TARGET_VS_6_5 = 304,     /* "vs_6_5"  */
    TOKEN_TARGET_VS_6_6 = 305,     /* "vs_6_6"  */
    TOKEN_TARGET_VS_6_7 = 306,     /* "vs_6_7"  */
    TOKEN_TARGET_PS_6_0 = 307,     /* "ps_6_0"  */
    TOKEN_TARGET_PS_6_1 = 308,     /* "ps_6_1"  */
    TOKEN_TARGET_PS_6_2 = 309,     /* "ps_6_2"  */
    TOKEN_TARGET_PS_6_3 = 310,     /* "ps_6_3"  */
    TOKEN_TARGET_PS_6_4 = 311,     /* "ps_6_4"  */
    TOKEN_TARGET_PS_6_5 = 312,     /* "ps_6_5"  */
    TOKEN_TARGET_PS_6_6 = 313,     /* "ps_6_6"  */
    TOKEN_TARGET_PS_6_7 = 314,     /* "ps_6_7"  */
    TOKEN_FLOAT = 315,             /* TOKEN_FLOAT  */
    TOKEN_FLOAT2 = 316,            /* TOKEN_FLOAT2  */
    TOKEN_FLOAT3 = 317,            /* TOKEN_FLOAT3  */
    TOKEN_FLOAT4 = 318,            /* TOKEN_FLOAT4  */
    TOKEN_INT = 319,               /* TOKEN_INT  */
    TOKEN_INT2 = 320,              /* TOKEN_INT2  */
    TOKEN_INT3 = 321,              /* TOKEN_INT3  */
    TOKEN_INT4 = 322,              /* TOKEN_INT4  */
    TOKEN_TEXTURE2D = 323          /* TOKEN_TEXTURE2D  */
  };
  typedef enum shtokentype shtoken_kind_t;
#endif

/* Value type.  */
#if ! defined SHSTYPE && ! defined SHSTYPE_IS_DECLARED
union SHSTYPE
{
#line 33 "src/shaders_compiler/parser/parser.y"

  float4 f4val;
  float3 f3val;
  float2 f2val;
  float fval;
  int ival;
  char* sval;

  ScopeDeclarationExp* scopeDeclaration;
  ScopeExp* scopeExp;

  ShadersResourcesReserveExp* shadersResourcesReserve;
  ResourceReserveExp* resReserveExp;
  gapi::ShaderStage shader;
  DescriptorSetReserveExp* dsetExp;
  ShaderExp* shaderExp;

  ResourceDeclaration* resourceDeclaration;
  ResourceType resourceType;
  ResourceAssignExp* resourceAssignExp;

  AttributeType attributeType;
  InputBufferExp* inputBufferExp;
  InputAttributeExp* inputAttributeExp;

  gapi::PrimitiveTopology primitiveTopology;

  TechniqueExp* techniqueExp;
  TargetProfile targetProfile;

#line 179 "src/shaders_compiler/parser/parser.tab.hpp"

};
typedef union SHSTYPE SHSTYPE;
# define SHSTYPE_IS_TRIVIAL 1
# define SHSTYPE_IS_DECLARED 1
#endif


extern SHSTYPE shlval;

int shparse (Compiler& compiler);

#endif /* !YY_SH_SRC_SHADERS_COMPILER_PARSER_PARSER_TAB_HPP_INCLUDED  */
