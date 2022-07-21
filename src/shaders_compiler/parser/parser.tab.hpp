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
    TOKEN_BOOL_VAL = 261,          /* TOKEN_BOOL_VAL  */
    TOKEN_HLSL_CODE = 262,         /* TOKEN_HLSL_CODE  */
    TOKEN_AT = 263,                /* "@"  */
    TOKEN_COLON = 264,             /* ":"  */
    TOKEN_SEMICOLON = 265,         /* ";"  */
    TOKEN_EQUAL_OP = 266,          /* "="  */
    TOKEN_LEFT_PARENTHESIS = 267,  /* "("  */
    TOKEN_RIGHT_PARENTHESIS = 268, /* ")"  */
    TOKEN_LEFT_BRACKET = 269,      /* "{"  */
    TOKEN_RIGHT_BRACKET = 270,     /* "}"  */
    TOKEN_COMMA = 271,             /* ","  */
    TOKEN_MINUS = 272,             /* "-"  */
    TOKEN_INPUT = 273,             /* "input"  */
    TOKEN_BUFFER = 274,            /* "buffer"  */
    TOKEN_SCOPE = 275,             /* "scope"  */
    TOKEN_SHADER = 276,            /* "shader"  */
    TOKEN_SHADER_VERTEX = 277,     /* "vertex"  */
    TOKEN_SHADER_PIXEL = 278,      /* "pixel"  */
    TOKEN_REGISTER = 279,          /* "register"  */
    TOKEN_TEXTURE = 280,           /* "texture"  */
    TOKEN_CBUFFER = 281,           /* "cbuffer"  */
    TOKEN_EXTERN = 282,            /* "extern"  */
    TOKEN_CHANNEL = 283,           /* "channel"  */
    TOKEN_DESCRIPTOR_SET = 284,    /* "descriptor_set"  */
    TOKEN_TECHNIQUE = 285,         /* "technique"  */
    TOKEN_SUPPORT = 286,           /* "support"  */
    TOKEN_ACTIVATE = 287,          /* "activate"  */
    TOKEN_COMPILE = 288,           /* "compile"  */
    TOKEN_RENDER_STATE = 289,      /* "render_state"  */
    TOKEN_PRIMITIVE_TOPOLOGY = 290, /* "primitive_topology"  */
    TOKEN_PT_POINT_LIST = 291,     /* "point_list"  */
    TOKEN_PT_LINE_LIST = 292,      /* "line_list"  */
    TOKEN_PT_LINE_STRIP = 293,     /* "line_strip"  */
    TOKEN_PT_TRIANGLE_LIST = 294,  /* "triangle_list"  */
    TOKEN_PT_TRIANGLE_FAN = 295,   /* "triangle_fan"  */
    TOKEN_PT_LINE_LIST_WITH_ADJACENCY = 296, /* "line_list_with_adjacency"  */
    TOKEN_PT_LINE_STRIP_WITH_ADJACENCY = 297, /* "line_strip_with_adjacency"  */
    TOKEN_PT_TRIANGLE_LIST_WITH_ADJACENCY = 298, /* "triangle_list_with_adjacency"  */
    TOKEN_PT_TRIANGLE_STRIP_WITH_ADJACENCY = 299, /* "triangle_strip_with_adjacency"  */
    TOKEN_PT_PATCH_LIST = 300,     /* "patch_list"  */
    TOKEN_DEPTH_TEST = 301,        /* "depth_test"  */
    TOKEN_DEPTH_WRITE = 302,       /* "depth_write"  */
    TOKEN_DEPTH_OP = 303,          /* "depth_op"  */
    TOKEN_STENCIL_TEST = 304,      /* "stencil_test"  */
    TOKEN_STENCIL_FAIL_OP = 305,   /* "stencil_fail_op"  */
    TOKEN_STENCIL_PASS_OP = 306,   /* "stencil_pass_op"  */
    TOKEN_STENCIL_DEPTH_FAIL_OP = 307, /* "stencil_depth_fail_op"  */
    TOKEN_STENCIL_COMPARE_OP = 308, /* "stencil_compare_op"  */
    TOKEN_STENCIL_REFERENCE_VALUE = 309, /* "stencil_ref_val"  */
    TOKEN_NEVER = 310,             /* "never"  */
    TOKEN_LESS = 311,              /* "less"  */
    TOKEN_EQUAL = 312,             /* "equal"  */
    TOKEN_LESS_OR_EQUAL = 313,     /* "less_or_equal"  */
    TOKEN_GREATER = 314,           /* "greater"  */
    TOKEN_NOT_EQUAL = 315,         /* "not_equal"  */
    TOKEN_GREATER_OR_EQUAL = 316,  /* "greater_or_equal"  */
    TOKEN_ALWAYS = 317,            /* "always"  */
    TOKEN_KEEP = 318,              /* "keep"  */
    TOKEN_ZERO = 319,              /* "zero"  */
    TOKEN_REPLACE = 320,           /* "replace"  */
    TOKEN_INCREMENT_AND_CLAMP = 321, /* "inc_and_clamp"  */
    TOKEN_DECREMENT_AND_CLAMP = 322, /* "dec_and_clamp"  */
    TOKEN_INVERT = 323,            /* "invert"  */
    TOKEN_INCREMENT_AND_WRAP = 324, /* "inc_and_wrap"  */
    TOKEN_DECREMENT_AND_WRAP = 325, /* "dec_and_wrap"  */
    TOKEN_TARGET_VS_6_0 = 326,     /* "vs_6_0"  */
    TOKEN_TARGET_VS_6_1 = 327,     /* "vs_6_1"  */
    TOKEN_TARGET_VS_6_2 = 328,     /* "vs_6_2"  */
    TOKEN_TARGET_VS_6_3 = 329,     /* "vs_6_3"  */
    TOKEN_TARGET_VS_6_4 = 330,     /* "vs_6_4"  */
    TOKEN_TARGET_VS_6_5 = 331,     /* "vs_6_5"  */
    TOKEN_TARGET_VS_6_6 = 332,     /* "vs_6_6"  */
    TOKEN_TARGET_VS_6_7 = 333,     /* "vs_6_7"  */
    TOKEN_TARGET_PS_6_0 = 334,     /* "ps_6_0"  */
    TOKEN_TARGET_PS_6_1 = 335,     /* "ps_6_1"  */
    TOKEN_TARGET_PS_6_2 = 336,     /* "ps_6_2"  */
    TOKEN_TARGET_PS_6_3 = 337,     /* "ps_6_3"  */
    TOKEN_TARGET_PS_6_4 = 338,     /* "ps_6_4"  */
    TOKEN_TARGET_PS_6_5 = 339,     /* "ps_6_5"  */
    TOKEN_TARGET_PS_6_6 = 340,     /* "ps_6_6"  */
    TOKEN_TARGET_PS_6_7 = 341,     /* "ps_6_7"  */
    TOKEN_FLOAT = 342,             /* TOKEN_FLOAT  */
    TOKEN_FLOAT2 = 343,            /* TOKEN_FLOAT2  */
    TOKEN_FLOAT3 = 344,            /* TOKEN_FLOAT3  */
    TOKEN_FLOAT4 = 345,            /* TOKEN_FLOAT4  */
    TOKEN_INT = 346,               /* TOKEN_INT  */
    TOKEN_INT2 = 347,              /* TOKEN_INT2  */
    TOKEN_INT3 = 348,              /* TOKEN_INT3  */
    TOKEN_INT4 = 349,              /* TOKEN_INT4  */
    TOKEN_TEXTURE2D = 350          /* TOKEN_TEXTURE2D  */
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
  bool bval;

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

  RenderStateExp* renderStateExp;
  gapi::PrimitiveTopology primitiveTopology;
  gapi::CompareOp compareOp;
  gapi::StencilOp stencilOp;

  TechniqueExp* techniqueExp;
  TargetProfile targetProfile;

#line 210 "src/shaders_compiler/parser/parser.tab.hpp"

};
typedef union SHSTYPE SHSTYPE;
# define SHSTYPE_IS_TRIVIAL 1
# define SHSTYPE_IS_DECLARED 1
#endif


extern SHSTYPE shlval;

int shparse (Compiler& compiler);

#endif /* !YY_SH_SRC_SHADERS_COMPILER_PARSER_PARSER_TAB_HPP_INCLUDED  */
