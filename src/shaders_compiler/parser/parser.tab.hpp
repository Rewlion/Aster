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
#line 24 "src/shaders_compiler/parser/parser.y"

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
    TFX_TOKEN_LEFT_SQUARE_BRACKET = 271, /* "["  */
    TFX_TOKEN_RIGHT_SQUARE_BRACKET = 272, /* "]"  */
    TFX_TOKEN_LESS_SIGN = 273,     /* "<"  */
    TFX_TOKEN_GREATER_SIGN = 274,  /* ">"  */
    TFX_TOKEN_COMMA = 275,         /* ","  */
    TFX_TOKEN_MINUS = 276,         /* "-"  */
    TFX_TOKEN_INPUT = 277,         /* "input"  */
    TFX_TOKEN_BUFFER = 278,        /* "buffer"  */
    TFX_TOKEN_SCOPE = 279,         /* "scope"  */
    TFX_TOKEN_RESERVE = 280,       /* "reserve"  */
    TFX_TOKEN_REGISTER = 281,      /* "register"  */
    TFX_TOKEN_TEXTURE = 282,       /* "texture"  */
    TFX_TOKEN_SAMPLER = 283,       /* "sampler"  */
    TFX_TOKEN_CBUFFER = 284,       /* "cbuffer"  */
    TFX_TOKEN_EXTERN = 285,        /* "extern"  */
    TFX_TOKEN_CHANNEL = 286,       /* "channel"  */
    TFX_TOKEN_DESCRIPTOR_SET = 287, /* "descriptor_set"  */
    TFX_TOKEN_TECHNIQUE = 288,     /* "technique"  */
    TFX_TOKEN_TECHNIQUE_MACRO = 289, /* "technique_macro"  */
    TFX_TOKEN_SUPPORT = 290,       /* "support"  */
    TFX_TOKEN_ACTIVATE = 291,      /* "activate"  */
    TFX_TOKEN_COMPILE = 292,       /* "compile"  */
    TFX_TOKEN_RENDER_STATE = 293,  /* "render_state"  */
    TFX_TOKEN_CULL_MODE = 294,     /* "cull_mode"  */
    TFX_TOKEN_NONE = 295,          /* "none"  */
    TFX_TOKEN_CCW = 296,           /* "ccw"  */
    TFX_TOKEN_CW = 297,            /* "cw"  */
    TFX_TOKEN_POLYGON_MODE = 298,  /* "polygon_mode"  */
    TFX_TOKEN_FILL = 299,          /* "fill"  */
    TFX_TOKEN_LINE = 300,          /* "line"  */
    TFX_TOKEN_POINT = 301,         /* "point"  */
    TFX_TOKEN_PRIMITIVE_TOPOLOGY = 302, /* "primitive_topology"  */
    TFX_TOKEN_PT_POINT_LIST = 303, /* "point_list"  */
    TFX_TOKEN_PT_LINE_LIST = 304,  /* "line_list"  */
    TFX_TOKEN_PT_LINE_STRIP = 305, /* "line_strip"  */
    TFX_TOKEN_PT_TRIANGLE_LIST = 306, /* "triangle_list"  */
    TFX_TOKEN_PT_TRIANGLE_FAN = 307, /* "triangle_fan"  */
    TFX_TOKEN_PT_TRIANGLE_STRIP = 308, /* "triangle_strip"  */
    TFX_TOKEN_PT_LINE_LIST_WITH_ADJACENCY = 309, /* "line_list_with_adjacency"  */
    TFX_TOKEN_PT_LINE_STRIP_WITH_ADJACENCY = 310, /* "line_strip_with_adjacency"  */
    TFX_TOKEN_PT_TRIANGLE_LIST_WITH_ADJACENCY = 311, /* "triangle_list_with_adjacency"  */
    TFX_TOKEN_PT_TRIANGLE_STRIP_WITH_ADJACENCY = 312, /* "triangle_strip_with_adjacency"  */
    TFX_TOKEN_PT_PATCH_LIST = 313, /* "patch_list"  */
    TFX_TOKEN_DEPTH = 314,         /* "depth"  */
    TFX_TOKEN_STENCIL = 315,       /* "stencil"  */
    TFX_TOKEN_TEST = 316,          /* "test"  */
    TFX_TOKEN_WRITE = 317,         /* "write"  */
    TFX_TOKEN_FAIL_OP = 318,       /* "fail_op"  */
    TFX_TOKEN_PASS_OP = 319,       /* "pass_op"  */
    TFX_TOKEN_DEPTH_FAIL_OP = 320, /* "depth_fail_op"  */
    TFX_TOKEN_COMPARE_OP = 321,    /* "compare_op"  */
    TFX_TOKEN_REFERENCE_VALUE = 322, /* "ref_val"  */
    TFX_TOKEN_NEVER = 323,         /* "never"  */
    TFX_TOKEN_LESS = 324,          /* "less"  */
    TFX_TOKEN_EQUAL = 325,         /* "equal"  */
    TFX_TOKEN_LESS_OR_EQUAL = 326, /* "less_or_equal"  */
    TFX_TOKEN_GREATER = 327,       /* "greater"  */
    TFX_TOKEN_NOT_EQUAL = 328,     /* "not_equal"  */
    TFX_TOKEN_GREATER_OR_EQUAL = 329, /* "greater_or_equal"  */
    TFX_TOKEN_ALWAYS = 330,        /* "always"  */
    TFX_TOKEN_KEEP = 331,          /* "keep"  */
    TFX_TOKEN_ZERO = 332,          /* "zero"  */
    TFX_TOKEN_REPLACE = 333,       /* "replace"  */
    TFX_TOKEN_INCREMENT_AND_CLAMP = 334, /* "inc_and_clamp"  */
    TFX_TOKEN_DECREMENT_AND_CLAMP = 335, /* "dec_and_clamp"  */
    TFX_TOKEN_INVERT = 336,        /* "invert"  */
    TFX_TOKEN_INCREMENT_AND_WRAP = 337, /* "inc_and_wrap"  */
    TFX_TOKEN_DECREMENT_AND_WRAP = 338, /* "dec_and_wrap"  */
    TFX_TOKEN_BLENDING = 339,      /* "blending"  */
    TFX_TOKEN_HAS_BLANDING = 340,  /* "has_blending"  */
    TFX_TOKEN_LOGIC_OP_ENABLED = 341, /* "logic_op_enabled"  */
    TFX_TOKEN_LOGIC_OP = 342,      /* "logic_op"  */
    TFX_TOKEN_BLEND_CONSTANTS = 343, /* "blend_constants"  */
    TFX_TOKEN_MRT = 344,           /* "mrt"  */
    TFX_TOKEN_SRC_COLOR_BLEND_FACTOR = 345, /* "src_color_blend_factor"  */
    TFX_TOKEN_DST_COLOR_BLEND_FACTOR = 346, /* "dst_color_blend_factor"  */
    TFX_TOKEN_COLOR_BLEND_OP = 347, /* "color_blend_op"  */
    TFX_TOKEN_SRC_ALPHA_BLEND_FACTOR = 348, /* "src_alpha_blend_factor"  */
    TFX_TOKEN_DST_ALPHA_BLEND_FACTOR = 349, /* "dst_alpha_blend_factor"  */
    TFX_TOKEN_ALPHA_BLEND_OP = 350, /* "alpha_blend_op"  */
    TFX_TOKEN_ONE = 351,           /* "one"  */
    TFX_TOKEN_SRC_COLOR = 352,     /* "src_color"  */
    TFX_TOKEN_ONE_MINUS_SRC_COLOR = 353, /* "one_minus_src_color"  */
    TFX_TOKEN_DST_COLOR = 354,     /* "dst_color"  */
    TFX_TOKEN_ONE_MINUS_DST_COLOR = 355, /* "one_minus_dst_color"  */
    TFX_TOKEN_SRC_ALPHA = 356,     /* "src_alpha"  */
    TFX_TOKEN_ONE_MINUS_SRC_ALPHA = 357, /* "one_minus_src_alpha"  */
    TFX_TOKEN_DST_ALPHA = 358,     /* "dst_alpha"  */
    TFX_TOKEN_ONE_MINUS_DST_ALPHA = 359, /* "one_minus_dst_alpha"  */
    TFX_TOKEN_CONSTANT_COLOR = 360, /* "constant_color"  */
    TFX_TOKEN_ONE_MINUS_CONSTANT_COLOR = 361, /* "one_minus_constant_color"  */
    TFX_TOKEN_CONSTANT_ALPHA = 362, /* "constant_alpha"  */
    TFX_TOKEN_ONE_MINUS_CONSTANT_ALPHA = 363, /* "one_minus_constant_alpha"  */
    TFX_TOKEN_SRC_ALPHA_SATURATE = 364, /* "src_alpha_saturate"  */
    TFX_TOKEN_SRC_ONE_COLOR = 365, /* "src_one_color"  */
    TFX_TOKEN_ONE_MINUS_SRC_ONE_COLOR = 366, /* "one_minus_src_one_color"  */
    TFX_TOKEN_SRC_ONE_ALPHA = 367, /* "src_one_alpha"  */
    TFX_TOKEN_ONE_MINUS_SRC_ONE_ALPHA = 368, /* "one_minus_src_one_alpha"  */
    TFX_TOKEN_ADD = 369,           /* "add"  */
    TFX_TOKEN_SUBTRACT = 370,      /* "subtract"  */
    TFX_TOKEN_REVERSE_SUBTRACT = 371, /* "reverse_subtract"  */
    TFX_TOKEN_MIN = 372,           /* "min"  */
    TFX_TOKEN_MAX = 373,           /* "max"  */
    TFX_TOKEN_CLEAR = 374,         /* "clear"  */
    TFX_TOKEN_AND = 375,           /* "and"  */
    TFX_TOKEN_AND_REVERSE = 376,   /* "and_reverse"  */
    TFX_TOKEN_COPY = 377,          /* "copy"  */
    TFX_TOKEN_AND_INVERTED = 378,  /* "and_inverted"  */
    TFX_TOKEN_NO_OP = 379,         /* "no_op"  */
    TFX_TOKEN_XOR = 380,           /* "xor"  */
    TFX_TOKEN_OR = 381,            /* "or"  */
    TFX_TOKEN_NOR = 382,           /* "nor"  */
    TFX_TOKEN_EQUIVALENT = 383,    /* "equivalent"  */
    TFX_TOKEN_OR_REVERSE = 384,    /* "or_reverse"  */
    TFX_TOKEN_COPY_INVERTED = 385, /* "copy_inverted"  */
    TFX_TOKEN_OR_INVERTED = 386,   /* "or_inverted"  */
    TFX_TOKEN_NAND = 387,          /* "nand"  */
    TFX_TOKEN_SET = 388,           /* "set"  */
    TFX_TOKEN_FLOAT = 389,         /* "float"  */
    TFX_TOKEN_FLOAT2 = 390,        /* "float2"  */
    TFX_TOKEN_FLOAT3 = 391,        /* "float3"  */
    TFX_TOKEN_FLOAT4 = 392,        /* "float4"  */
    TFX_TOKEN_FLOAT4_u8 = 393,     /* "float4_u8"  */
    TFX_TOKEN_FLOAT4X4 = 394,      /* "float4x4"  */
    TFX_TOKEN_UINT = 395,          /* "uint"  */
    TFX_TOKEN_UINT2 = 396,         /* "uint2"  */
    TFX_TOKEN_UINT3 = 397,         /* "uint3"  */
    TFX_TOKEN_UINT4 = 398,         /* "uint4"  */
    TFX_TOKEN_INT = 399,           /* "int"  */
    TFX_TOKEN_INT2 = 400,          /* "int2"  */
    TFX_TOKEN_INT3 = 401,          /* "int3"  */
    TFX_TOKEN_INT4 = 402,          /* "int4"  */
    TFX_TOKEN_ROBUFFER = 403,      /* "Buffer"  */
    TFX_TOKEN_STRUCTUREDBUFFER = 404, /* "StructuredBuffer"  */
    TFX_TOKEN_RWSTRUCTUREDBUFFER = 405, /* "RWStructuredBuffer"  */
    TFX_TOKEN_RWBUFFER = 406,      /* "RWBuffer"  */
    TFX_TOKEN_TEXTURE2D = 407,     /* "Texture2D"  */
    TFX_TOKEN_TEXTURE3D = 408,     /* "Texture3D"  */
    TFX_TOKEN_TEXTURE_CUBE = 409,  /* "TextureCube"  */
    TFX_TOKEN_RWTEXTURE3D = 410,   /* "RWTexture3D"  */
    TFX_TOKEN_RWTEXTURE2D = 411    /* "RWTexture2D"  */
  };
  typedef enum shtokentype shtoken_kind_t;
#endif

/* Value type.  */
#if ! defined SHSTYPE && ! defined SHSTYPE_IS_DECLARED
union SHSTYPE
{
#line 36 "src/shaders_compiler/parser/parser.y"

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
  ResourceDeclarationExp* resourceDeclExp;
  FullResourceType* fullResourceType;
  ResourceType resourceType;
  ResourceAssignExp* resourceAssignExp;

  gapi::AttributeType attributeType;
  InputBufferExp* inputBufferExp;
  InputAttributeExp* inputAttributeExp;

  RenderStateExp* renderStateExp;
  gapi::CullMode cullMode;
  gapi::PolygonMode polygonMode;
  gapi::PrimitiveTopology primitiveTopology;
  gapi::CompareOp compareOp;
  gapi::StencilOp stencilOp;
  DepthExp* depthExp;
  StencilExp* stencilExp;
  BlendingExp* blendingExp;
  MrtBlendingExp* mrtBlendingExp;
  gapi::BlendFactor blendFactor;
  gapi::BlendOp blendOp;
  gapi::LogicOp logicOp;

  TechniqueExp* techniqueExp;

#line 279 "src/shaders_compiler/parser/parser.tab.hpp"

};
typedef union SHSTYPE SHSTYPE;
# define SHSTYPE_IS_TRIVIAL 1
# define SHSTYPE_IS_DECLARED 1
#endif


extern SHSTYPE shlval;

int shparse (Compiler& compiler);

#endif /* !YY_SH_SRC_SHADERS_COMPILER_PARSER_PARSER_TAB_HPP_INCLUDED  */
