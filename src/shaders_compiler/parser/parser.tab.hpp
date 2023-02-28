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
    TFX_TOKEN_TECHNIQUE_MACRO = 285, /* "technique_macro"  */
    TFX_TOKEN_SUPPORT = 286,       /* "support"  */
    TFX_TOKEN_ACTIVATE = 287,      /* "activate"  */
    TFX_TOKEN_COMPILE = 288,       /* "compile"  */
    TFX_TOKEN_RENDER_STATE = 289,  /* "render_state"  */
    TFX_TOKEN_CULL_MODE = 290,     /* "cull_mode"  */
    TFX_TOKEN_NONE = 291,          /* "none"  */
    TFX_TOKEN_CCW = 292,           /* "ccw"  */
    TFX_TOKEN_CW = 293,            /* "cw"  */
    TFX_TOKEN_PRIMITIVE_TOPOLOGY = 294, /* "primitive_topology"  */
    TFX_TOKEN_PT_POINT_LIST = 295, /* "point_list"  */
    TFX_TOKEN_PT_LINE_LIST = 296,  /* "line_list"  */
    TFX_TOKEN_PT_LINE_STRIP = 297, /* "line_strip"  */
    TFX_TOKEN_PT_TRIANGLE_LIST = 298, /* "triangle_list"  */
    TFX_TOKEN_PT_TRIANGLE_FAN = 299, /* "triangle_fan"  */
    TFX_TOKEN_PT_TRIANGLE_STRIP = 300, /* "triangle_strip"  */
    TFX_TOKEN_PT_LINE_LIST_WITH_ADJACENCY = 301, /* "line_list_with_adjacency"  */
    TFX_TOKEN_PT_LINE_STRIP_WITH_ADJACENCY = 302, /* "line_strip_with_adjacency"  */
    TFX_TOKEN_PT_TRIANGLE_LIST_WITH_ADJACENCY = 303, /* "triangle_list_with_adjacency"  */
    TFX_TOKEN_PT_TRIANGLE_STRIP_WITH_ADJACENCY = 304, /* "triangle_strip_with_adjacency"  */
    TFX_TOKEN_PT_PATCH_LIST = 305, /* "patch_list"  */
    TFX_TOKEN_DEPTH = 306,         /* "depth"  */
    TFX_TOKEN_STENCIL = 307,       /* "stencil"  */
    TFX_TOKEN_TEST = 308,          /* "test"  */
    TFX_TOKEN_WRITE = 309,         /* "write"  */
    TFX_TOKEN_FAIL_OP = 310,       /* "fail_op"  */
    TFX_TOKEN_PASS_OP = 311,       /* "pass_op"  */
    TFX_TOKEN_DEPTH_FAIL_OP = 312, /* "depth_fail_op"  */
    TFX_TOKEN_COMPARE_OP = 313,    /* "compare_op"  */
    TFX_TOKEN_REFERENCE_VALUE = 314, /* "ref_val"  */
    TFX_TOKEN_NEVER = 315,         /* "never"  */
    TFX_TOKEN_LESS = 316,          /* "less"  */
    TFX_TOKEN_EQUAL = 317,         /* "equal"  */
    TFX_TOKEN_LESS_OR_EQUAL = 318, /* "less_or_equal"  */
    TFX_TOKEN_GREATER = 319,       /* "greater"  */
    TFX_TOKEN_NOT_EQUAL = 320,     /* "not_equal"  */
    TFX_TOKEN_GREATER_OR_EQUAL = 321, /* "greater_or_equal"  */
    TFX_TOKEN_ALWAYS = 322,        /* "always"  */
    TFX_TOKEN_KEEP = 323,          /* "keep"  */
    TFX_TOKEN_ZERO = 324,          /* "zero"  */
    TFX_TOKEN_REPLACE = 325,       /* "replace"  */
    TFX_TOKEN_INCREMENT_AND_CLAMP = 326, /* "inc_and_clamp"  */
    TFX_TOKEN_DECREMENT_AND_CLAMP = 327, /* "dec_and_clamp"  */
    TFX_TOKEN_INVERT = 328,        /* "invert"  */
    TFX_TOKEN_INCREMENT_AND_WRAP = 329, /* "inc_and_wrap"  */
    TFX_TOKEN_DECREMENT_AND_WRAP = 330, /* "dec_and_wrap"  */
    TFX_TOKEN_BLENDING = 331,      /* "blending"  */
    TFX_TOKEN_HAS_BLANDING = 332,  /* "has_blending"  */
    TFX_TOKEN_LOGIC_OP_ENABLED = 333, /* "logic_op_enabled"  */
    TFX_TOKEN_LOGIC_OP = 334,      /* "logic_op"  */
    TFX_TOKEN_BLEND_CONSTANTS = 335, /* "blend_constants"  */
    TFX_TOKEN_MRT = 336,           /* "mrt"  */
    TFX_TOKEN_SRC_COLOR_BLEND_FACTOR = 337, /* "src_color_blend_factor"  */
    TFX_TOKEN_DST_COLOR_BLEND_FACTOR = 338, /* "dst_color_blend_factor"  */
    TFX_TOKEN_COLOR_BLEND_OP = 339, /* "color_blend_op"  */
    TFX_TOKEN_SRC_ALPHA_BLEND_FACTOR = 340, /* "src_alpha_blend_factor"  */
    TFX_TOKEN_DST_ALPHA_BLEND_FACTOR = 341, /* "dst_alpha_blend_factor"  */
    TFX_TOKEN_ALPHA_BLEND_OP = 342, /* "alpha_blend_op"  */
    TFX_TOKEN_ONE = 343,           /* "one"  */
    TFX_TOKEN_SRC_COLOR = 344,     /* "src_color"  */
    TFX_TOKEN_ONE_MINUS_SRC_COLOR = 345, /* "one_minus_src_color"  */
    TFX_TOKEN_DST_COLOR = 346,     /* "dst_color"  */
    TFX_TOKEN_ONE_MINUS_DST_COLOR = 347, /* "one_minus_dst_color"  */
    TFX_TOKEN_SRC_ALPHA = 348,     /* "src_alpha"  */
    TFX_TOKEN_ONE_MINUS_SRC_ALPHA = 349, /* "one_minus_src_alpha"  */
    TFX_TOKEN_DST_ALPHA = 350,     /* "dst_alpha"  */
    TFX_TOKEN_ONE_MINUS_DST_ALPHA = 351, /* "one_minus_dst_alpha"  */
    TFX_TOKEN_CONSTANT_COLOR = 352, /* "constant_color"  */
    TFX_TOKEN_ONE_MINUS_CONSTANT_COLOR = 353, /* "one_minus_constant_color"  */
    TFX_TOKEN_CONSTANT_ALPHA = 354, /* "constant_alpha"  */
    TFX_TOKEN_ONE_MINUS_CONSTANT_ALPHA = 355, /* "one_minus_constant_alpha"  */
    TFX_TOKEN_SRC_ALPHA_SATURATE = 356, /* "src_alpha_saturate"  */
    TFX_TOKEN_SRC_ONE_COLOR = 357, /* "src_one_color"  */
    TFX_TOKEN_ONE_MINUS_SRC_ONE_COLOR = 358, /* "one_minus_src_one_color"  */
    TFX_TOKEN_SRC_ONE_ALPHA = 359, /* "src_one_alpha"  */
    TFX_TOKEN_ONE_MINUS_SRC_ONE_ALPHA = 360, /* "one_minus_src_one_alpha"  */
    TFX_TOKEN_ADD = 361,           /* "add"  */
    TFX_TOKEN_SUBTRACT = 362,      /* "subtract"  */
    TFX_TOKEN_REVERSE_SUBTRACT = 363, /* "reverse_subtract"  */
    TFX_TOKEN_MIN = 364,           /* "min"  */
    TFX_TOKEN_MAX = 365,           /* "max"  */
    TFX_TOKEN_CLEAR = 366,         /* "clear"  */
    TFX_TOKEN_AND = 367,           /* "and"  */
    TFX_TOKEN_AND_REVERSE = 368,   /* "and_reverse"  */
    TFX_TOKEN_COPY = 369,          /* "copy"  */
    TFX_TOKEN_AND_INVERTED = 370,  /* "and_inverted"  */
    TFX_TOKEN_NO_OP = 371,         /* "no_op"  */
    TFX_TOKEN_XOR = 372,           /* "xor"  */
    TFX_TOKEN_OR = 373,            /* "or"  */
    TFX_TOKEN_NOR = 374,           /* "nor"  */
    TFX_TOKEN_EQUIVALENT = 375,    /* "equivalent"  */
    TFX_TOKEN_OR_REVERSE = 376,    /* "or_reverse"  */
    TFX_TOKEN_COPY_INVERTED = 377, /* "copy_inverted"  */
    TFX_TOKEN_OR_INVERTED = 378,   /* "or_inverted"  */
    TFX_TOKEN_NAND = 379,          /* "nand"  */
    TFX_TOKEN_SET = 380,           /* "set"  */
    TFX_TOKEN_TARGET_VS_6_0 = 381, /* "vs_6_0"  */
    TFX_TOKEN_TARGET_VS_6_1 = 382, /* "vs_6_1"  */
    TFX_TOKEN_TARGET_VS_6_2 = 383, /* "vs_6_2"  */
    TFX_TOKEN_TARGET_VS_6_3 = 384, /* "vs_6_3"  */
    TFX_TOKEN_TARGET_VS_6_4 = 385, /* "vs_6_4"  */
    TFX_TOKEN_TARGET_VS_6_5 = 386, /* "vs_6_5"  */
    TFX_TOKEN_TARGET_VS_6_6 = 387, /* "vs_6_6"  */
    TFX_TOKEN_TARGET_VS_6_7 = 388, /* "vs_6_7"  */
    TFX_TOKEN_TARGET_PS_6_0 = 389, /* "ps_6_0"  */
    TFX_TOKEN_TARGET_PS_6_1 = 390, /* "ps_6_1"  */
    TFX_TOKEN_TARGET_PS_6_2 = 391, /* "ps_6_2"  */
    TFX_TOKEN_TARGET_PS_6_3 = 392, /* "ps_6_3"  */
    TFX_TOKEN_TARGET_PS_6_4 = 393, /* "ps_6_4"  */
    TFX_TOKEN_TARGET_PS_6_5 = 394, /* "ps_6_5"  */
    TFX_TOKEN_TARGET_PS_6_6 = 395, /* "ps_6_6"  */
    TFX_TOKEN_TARGET_PS_6_7 = 396, /* "ps_6_7"  */
    TFX_TOKEN_FLOAT = 397,         /* "float"  */
    TFX_TOKEN_FLOAT2 = 398,        /* "float2"  */
    TFX_TOKEN_FLOAT3 = 399,        /* "float3"  */
    TFX_TOKEN_FLOAT4 = 400,        /* "float4"  */
    TFX_TOKEN_FLOAT4_u8 = 401,     /* "float4_u8"  */
    TFX_TOKEN_FLOAT4X4 = 402,      /* "float4x4"  */
    TFX_TOKEN_INT = 403,           /* "int"  */
    TFX_TOKEN_INT2 = 404,          /* "int2"  */
    TFX_TOKEN_INT3 = 405,          /* "int3"  */
    TFX_TOKEN_INT4 = 406,          /* "int4"  */
    TFX_TOKEN_TEXTURE2D = 407,     /* "Texture2D"  */
    TFX_TOKEN_TEXTURE_CUBE = 408   /* "TextureCube"  */
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
  gapi::CullMode cullMode;
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

#line 272 "src/shaders_compiler/parser/parser.tab.hpp"

};
typedef union SHSTYPE SHSTYPE;
# define SHSTYPE_IS_TRIVIAL 1
# define SHSTYPE_IS_DECLARED 1
#endif


extern SHSTYPE shlval;

int shparse (Compiler& compiler);

#endif /* !YY_SH_SRC_SHADERS_COMPILER_PARSER_PARSER_TAB_HPP_INCLUDED  */
