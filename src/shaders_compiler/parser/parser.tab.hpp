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
    TFX_TOKEN_FLOAT_VAL = 258,     /* TFX_TOKEN_FLOAT_VAL  */
    TFX_TOKEN_INT_VAL = 259,       /* TFX_TOKEN_INT_VAL  */
    TFX_TOKEN_NAME_VAL = 260,      /* TFX_TOKEN_NAME_VAL  */
    TFX_TOKEN_BOOL_VAL = 261,      /* TFX_TOKEN_BOOL_VAL  */
    TFX_TOKEN_HLSL_CODE = 262,     /* TFX_TOKEN_HLSL_CODE  */
    TFX_TOKEN_AT = 263,            /* "@"  */
    TFX_TOKEN_COLON = 264,         /* ":"  */
    TFX_TOKEN_SEMICOLON = 265,     /* ";"  */
    TFX_TOKEN_EQUAL_OP = 266,      /* "="  */
    TFX_TOKEN_LEFT_PARENTHESIS = 267, /* "("  */
    TFX_TOKEN_RIGHT_PARENTHESIS = 268, /* ")"  */
    TFX_TOKEN_LEFT_BRACKET = 269,  /* "{"  */
    TFX_TOKEN_RIGHT_BRACKET = 270, /* "}"  */
    TFX_TOKEN_COMMA = 271,         /* ","  */
    TFX_TOKEN_MINUS = 272,         /* "-"  */
    TFX_TOKEN_INPUT = 273,         /* "input"  */
    TFX_TOKEN_BUFFER = 274,        /* "buffer"  */
    TFX_TOKEN_SCOPE = 275,         /* "scope"  */
    TFX_TOKEN_RESERVE = 276,       /* "reserve"  */
    TFX_TOKEN_REGISTER = 277,      /* "register"  */
    TFX_TOKEN_TEXTURE = 278,       /* "texture"  */
    TFX_TOKEN_SAMPLER = 279,       /* "sampler"  */
    TFX_TOKEN_CBUFFER = 280,       /* "cbuffer"  */
    TFX_TOKEN_EXTERN = 281,        /* "extern"  */
    TFX_TOKEN_CHANNEL = 282,       /* "channel"  */
    TFX_TOKEN_DESCRIPTOR_SET = 283, /* "descriptor_set"  */
    TFX_TOKEN_TECHNIQUE = 284,     /* "technique"  */
    TFX_TOKEN_SUPPORT = 285,       /* "support"  */
    TFX_TOKEN_ACTIVATE = 286,      /* "activate"  */
    TFX_TOKEN_COMPILE = 287,       /* "compile"  */
    TFX_TOKEN_RENDER_STATE = 288,  /* "render_state"  */
    TFX_TOKEN_PRIMITIVE_TOPOLOGY = 289, /* "primitive_topology"  */
    TFX_TOKEN_PT_POINT_LIST = 290, /* "point_list"  */
    TFX_TOKEN_PT_LINE_LIST = 291,  /* "line_list"  */
    TFX_TOKEN_PT_LINE_STRIP = 292, /* "line_strip"  */
    TFX_TOKEN_PT_TRIANGLE_LIST = 293, /* "triangle_list"  */
    TFX_TOKEN_PT_TRIANGLE_FAN = 294, /* "triangle_fan"  */
    TFX_TOKEN_PT_LINE_LIST_WITH_ADJACENCY = 295, /* "line_list_with_adjacency"  */
    TFX_TOKEN_PT_LINE_STRIP_WITH_ADJACENCY = 296, /* "line_strip_with_adjacency"  */
    TFX_TOKEN_PT_TRIANGLE_LIST_WITH_ADJACENCY = 297, /* "triangle_list_with_adjacency"  */
    TFX_TOKEN_PT_TRIANGLE_STRIP_WITH_ADJACENCY = 298, /* "triangle_strip_with_adjacency"  */
    TFX_TOKEN_PT_PATCH_LIST = 299, /* "patch_list"  */
    TFX_TOKEN_DEPTH = 300,         /* "depth"  */
    TFX_TOKEN_STENCIL = 301,       /* "stencil"  */
    TFX_TOKEN_TEST = 302,          /* "test"  */
    TFX_TOKEN_WRITE = 303,         /* "write"  */
    TFX_TOKEN_FAIL_OP = 304,       /* "fail_op"  */
    TFX_TOKEN_PASS_OP = 305,       /* "pass_op"  */
    TFX_TOKEN_DEPTH_FAIL_OP = 306, /* "depth_fail_op"  */
    TFX_TOKEN_COMPARE_OP = 307,    /* "compare_op"  */
    TFX_TOKEN_REFERENCE_VALUE = 308, /* "ref_val"  */
    TFX_TOKEN_NEVER = 309,         /* "never"  */
    TFX_TOKEN_LESS = 310,          /* "less"  */
    TFX_TOKEN_EQUAL = 311,         /* "equal"  */
    TFX_TOKEN_LESS_OR_EQUAL = 312, /* "less_or_equal"  */
    TFX_TOKEN_GREATER = 313,       /* "greater"  */
    TFX_TOKEN_NOT_EQUAL = 314,     /* "not_equal"  */
    TFX_TOKEN_GREATER_OR_EQUAL = 315, /* "greater_or_equal"  */
    TFX_TOKEN_ALWAYS = 316,        /* "always"  */
    TFX_TOKEN_KEEP = 317,          /* "keep"  */
    TFX_TOKEN_ZERO = 318,          /* "zero"  */
    TFX_TOKEN_REPLACE = 319,       /* "replace"  */
    TFX_TOKEN_INCREMENT_AND_CLAMP = 320, /* "inc_and_clamp"  */
    TFX_TOKEN_DECREMENT_AND_CLAMP = 321, /* "dec_and_clamp"  */
    TFX_TOKEN_INVERT = 322,        /* "invert"  */
    TFX_TOKEN_INCREMENT_AND_WRAP = 323, /* "inc_and_wrap"  */
    TFX_TOKEN_DECREMENT_AND_WRAP = 324, /* "dec_and_wrap"  */
    TFX_TOKEN_BLENDING = 325,      /* "blending"  */
    TFX_TOKEN_HAS_BLANDING = 326,  /* "has_blending"  */
    TFX_TOKEN_LOGIC_OP_ENABLED = 327, /* "logic_op_enabled"  */
    TFX_TOKEN_LOGIC_OP = 328,      /* "logic_op"  */
    TFX_TOKEN_BLEND_CONSTANTS = 329, /* "blend_constants"  */
    TFX_TOKEN_MRT = 330,           /* "mrt"  */
    TFX_TOKEN_SRC_COLOR_BLEND_FACTOR = 331, /* "src_color_blend_factor"  */
    TFX_TOKEN_DST_COLOR_BLEND_FACTOR = 332, /* "dst_color_blend_factor"  */
    TFX_TOKEN_COLOR_BLEND_OP = 333, /* "color_blend_op"  */
    TFX_TOKEN_SRC_ALPHA_BLEND_FACTOR = 334, /* "src_alpha_blend_factor"  */
    TFX_TOKEN_DST_ALPHA_BLEND_FACTOR = 335, /* "dst_alpha_blend_factor"  */
    TFX_TOKEN_ALPHA_BLEND_OP = 336, /* "alpha_blend_op"  */
    TFX_TOKEN_ONE = 337,           /* "one"  */
    TFX_TOKEN_SRC_COLOR = 338,     /* "src_color"  */
    TFX_TOKEN_ONE_MINUS_SRC_COLOR = 339, /* "one_minus_src_color"  */
    TFX_TOKEN_DST_COLOR = 340,     /* "dst_color"  */
    TFX_TOKEN_ONE_MINUS_DST_COLOR = 341, /* "one_minus_dst_color"  */
    TFX_TOKEN_SRC_ALPHA = 342,     /* "src_alpha"  */
    TFX_TOKEN_ONE_MINUS_SRC_ALPHA = 343, /* "one_minus_src_alpha"  */
    TFX_TOKEN_DST_ALPHA = 344,     /* "dst_alpha"  */
    TFX_TOKEN_ONE_MINUS_DST_ALPHA = 345, /* "one_minus_dst_alpha"  */
    TFX_TOKEN_CONSTANT_COLOR = 346, /* "constant_color"  */
    TFX_TOKEN_ONE_MINUS_CONSTANT_COLOR = 347, /* "one_minus_constant_color"  */
    TFX_TOKEN_CONSTANT_ALPHA = 348, /* "constant_alpha"  */
    TFX_TOKEN_ONE_MINUS_CONSTANT_ALPHA = 349, /* "one_minus_constant_alpha"  */
    TFX_TOKEN_SRC_ALPHA_SATURATE = 350, /* "src_alpha_saturate"  */
    TFX_TOKEN_SRC_ONE_COLOR = 351, /* "src_one_color"  */
    TFX_TOKEN_ONE_MINUS_SRC_ONE_COLOR = 352, /* "one_minus_src_one_color"  */
    TFX_TOKEN_SRC_ONE_ALPHA = 353, /* "src_one_alpha"  */
    TFX_TOKEN_ONE_MINUS_SRC_ONE_ALPHA = 354, /* "one_minus_src_one_alpha"  */
    TFX_TOKEN_ADD = 355,           /* "add"  */
    TFX_TOKEN_SUBTRACT = 356,      /* "subtract"  */
    TFX_TOKEN_REVERSE_SUBTRACT = 357, /* "reverse_subtract"  */
    TFX_TOKEN_MIN = 358,           /* "min"  */
    TFX_TOKEN_MAX = 359,           /* "max"  */
    TFX_TOKEN_CLEAR = 360,         /* "clear"  */
    TFX_TOKEN_AND = 361,           /* "and"  */
    TFX_TOKEN_AND_REVERSE = 362,   /* "and_reverse"  */
    TFX_TOKEN_COPY = 363,          /* "copy"  */
    TFX_TOKEN_AND_INVERTED = 364,  /* "and_inverted"  */
    TFX_TOKEN_NO_OP = 365,         /* "no_op"  */
    TFX_TOKEN_XOR = 366,           /* "xor"  */
    TFX_TOKEN_OR = 367,            /* "or"  */
    TFX_TOKEN_NOR = 368,           /* "nor"  */
    TFX_TOKEN_EQUIVALENT = 369,    /* "equivalent"  */
    TFX_TOKEN_OR_REVERSE = 370,    /* "or_reverse"  */
    TFX_TOKEN_COPY_INVERTED = 371, /* "copy_inverted"  */
    TFX_TOKEN_OR_INVERTED = 372,   /* "or_inverted"  */
    TFX_TOKEN_NAND = 373,          /* "nand"  */
    TFX_TOKEN_SET = 374,           /* "set"  */
    TFX_TOKEN_TARGET_VS_6_0 = 375, /* "vs_6_0"  */
    TFX_TOKEN_TARGET_VS_6_1 = 376, /* "vs_6_1"  */
    TFX_TOKEN_TARGET_VS_6_2 = 377, /* "vs_6_2"  */
    TFX_TOKEN_TARGET_VS_6_3 = 378, /* "vs_6_3"  */
    TFX_TOKEN_TARGET_VS_6_4 = 379, /* "vs_6_4"  */
    TFX_TOKEN_TARGET_VS_6_5 = 380, /* "vs_6_5"  */
    TFX_TOKEN_TARGET_VS_6_6 = 381, /* "vs_6_6"  */
    TFX_TOKEN_TARGET_VS_6_7 = 382, /* "vs_6_7"  */
    TFX_TOKEN_TARGET_PS_6_0 = 383, /* "ps_6_0"  */
    TFX_TOKEN_TARGET_PS_6_1 = 384, /* "ps_6_1"  */
    TFX_TOKEN_TARGET_PS_6_2 = 385, /* "ps_6_2"  */
    TFX_TOKEN_TARGET_PS_6_3 = 386, /* "ps_6_3"  */
    TFX_TOKEN_TARGET_PS_6_4 = 387, /* "ps_6_4"  */
    TFX_TOKEN_TARGET_PS_6_5 = 388, /* "ps_6_5"  */
    TFX_TOKEN_TARGET_PS_6_6 = 389, /* "ps_6_6"  */
    TFX_TOKEN_TARGET_PS_6_7 = 390, /* "ps_6_7"  */
    TFX_TOKEN_FLOAT = 391,         /* "float"  */
    TFX_TOKEN_FLOAT2 = 392,        /* "float2"  */
    TFX_TOKEN_FLOAT3 = 393,        /* "float3"  */
    TFX_TOKEN_FLOAT4 = 394,        /* "float4"  */
    TFX_TOKEN_FLOAT4X4 = 395,      /* "float4x4"  */
    TFX_TOKEN_INT = 396,           /* "int"  */
    TFX_TOKEN_INT2 = 397,          /* "int2"  */
    TFX_TOKEN_INT3 = 398,          /* "int3"  */
    TFX_TOKEN_INT4 = 399,          /* "int4"  */
    TFX_TOKEN_TEXTURE2D = 400      /* "Texture2D"  */
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
  DescriptorSetReserveExp* dsetExp;

  ResourceDeclaration* resourceDeclaration;
  ResourceType resourceType;
  ResourceAssignExp* resourceAssignExp;

  gapi::AttributeType attributeType;
  InputBufferExp* inputBufferExp;
  InputAttributeExp* inputAttributeExp;

  RenderStateExp* renderStateExp;
  gapi::PrimitiveTopology primitiveTopology;
  gapi::CompareOp compareOp;
  gapi::StencilOp stencilOp;
  BlendingExp* blendingExp;
  MrtBlendingExp* mrtBlendingExp;
  gapi::BlendFactor blendFactor;
  gapi::BlendOp blendOp;
  gapi::LogicOp logicOp;

  TechniqueExp* techniqueExp;
  TargetProfile targetProfile;

#line 263 "src/shaders_compiler/parser/parser.tab.hpp"

};
typedef union SHSTYPE SHSTYPE;
# define SHSTYPE_IS_TRIVIAL 1
# define SHSTYPE_IS_DECLARED 1
#endif


extern SHSTYPE shlval;

int shparse (Compiler& compiler);

#endif /* !YY_SH_SRC_SHADERS_COMPILER_PARSER_PARSER_TAB_HPP_INCLUDED  */
