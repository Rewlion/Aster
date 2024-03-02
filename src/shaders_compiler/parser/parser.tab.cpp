/* A Bison parser, made by GNU Bison 3.7.4.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30704

/* Bison version string.  */
#define YYBISON_VERSION "3.7.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Substitute the type names.  */
#define YYSTYPE         SHSTYPE
/* Substitute the variable and function names.  */
#define yyparse         shparse
#define yylex           shlex
#define yyerror         sherror
#define yydebug         shdebug
#define yynerrs         shnerrs
#define yylval          shlval
#define yychar          shchar

/* First part of user prologue.  */
#line 2 "src/shaders_compiler/parser/parser.y"

  #include <shaders_compiler/compiler.h>
  #include <shaders_compiler/limits.h>

  #include <engine/types.h>
  #include <engine/log.h>
  #include <engine/gapi/resources.h>
  
  #include <glm/glm.hpp>

  using namespace ShadersSystem;

// Declare stuff from Flex that Bison needs to know about:
  extern int shlex(Compiler& compiler);
  extern int shparse(Compiler& compiler);
  extern void sherror(Compiler& compiler, const char* msg);
  extern FILE *shin;
  extern int shlineno;
  extern int columno;
  extern char* shtext;

#line 101 "src/shaders_compiler/parser/parser.tab.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_TFX_TOKEN_FLOAT_VAL = 3,        /* TFX_TOKEN_FLOAT_VAL  */
  YYSYMBOL_TFX_TOKEN_INT_VAL = 4,          /* TFX_TOKEN_INT_VAL  */
  YYSYMBOL_TFX_TOKEN_NAME_VAL = 5,         /* TFX_TOKEN_NAME_VAL  */
  YYSYMBOL_TFX_TOKEN_BOOL_VAL = 6,         /* TFX_TOKEN_BOOL_VAL  */
  YYSYMBOL_TFX_TOKEN_HLSL_CODE = 7,        /* TFX_TOKEN_HLSL_CODE  */
  YYSYMBOL_TFX_TOKEN_AT = 8,               /* "@"  */
  YYSYMBOL_TFX_TOKEN_COLON = 9,            /* ":"  */
  YYSYMBOL_TFX_TOKEN_SEMICOLON = 10,       /* ";"  */
  YYSYMBOL_TFX_TOKEN_EQUAL_OP = 11,        /* "="  */
  YYSYMBOL_TFX_TOKEN_LEFT_PARENTHESIS = 12, /* "("  */
  YYSYMBOL_TFX_TOKEN_RIGHT_PARENTHESIS = 13, /* ")"  */
  YYSYMBOL_TFX_TOKEN_LEFT_BRACKET = 14,    /* "{"  */
  YYSYMBOL_TFX_TOKEN_RIGHT_BRACKET = 15,   /* "}"  */
  YYSYMBOL_TFX_TOKEN_LEFT_SQUARE_BRACKET = 16, /* "["  */
  YYSYMBOL_TFX_TOKEN_RIGHT_SQUARE_BRACKET = 17, /* "]"  */
  YYSYMBOL_TFX_TOKEN_LESS_SIGN = 18,       /* "<"  */
  YYSYMBOL_TFX_TOKEN_GREATER_SIGN = 19,    /* ">"  */
  YYSYMBOL_TFX_TOKEN_COMMA = 20,           /* ","  */
  YYSYMBOL_TFX_TOKEN_MINUS = 21,           /* "-"  */
  YYSYMBOL_TFX_TOKEN_INPUT = 22,           /* "input"  */
  YYSYMBOL_TFX_TOKEN_BUFFER = 23,          /* "buffer"  */
  YYSYMBOL_TFX_TOKEN_SCOPE = 24,           /* "scope"  */
  YYSYMBOL_TFX_TOKEN_RESERVE = 25,         /* "reserve"  */
  YYSYMBOL_TFX_TOKEN_REGISTER = 26,        /* "register"  */
  YYSYMBOL_TFX_TOKEN_TEXTURE = 27,         /* "texture"  */
  YYSYMBOL_TFX_TOKEN_SAMPLER = 28,         /* "sampler"  */
  YYSYMBOL_TFX_TOKEN_CBUFFER = 29,         /* "cbuffer"  */
  YYSYMBOL_TFX_TOKEN_EXTERN = 30,          /* "extern"  */
  YYSYMBOL_TFX_TOKEN_CHANNEL = 31,         /* "channel"  */
  YYSYMBOL_TFX_TOKEN_DESCRIPTOR_SET = 32,  /* "descriptor_set"  */
  YYSYMBOL_TFX_TOKEN_TECHNIQUE = 33,       /* "technique"  */
  YYSYMBOL_TFX_TOKEN_TECHNIQUE_MACRO = 34, /* "technique_macro"  */
  YYSYMBOL_TFX_TOKEN_SUPPORT = 35,         /* "support"  */
  YYSYMBOL_TFX_TOKEN_ACTIVATE = 36,        /* "activate"  */
  YYSYMBOL_TFX_TOKEN_COMPILE = 37,         /* "compile"  */
  YYSYMBOL_TFX_TOKEN_RENDER_STATE = 38,    /* "render_state"  */
  YYSYMBOL_TFX_TOKEN_CULL_MODE = 39,       /* "cull_mode"  */
  YYSYMBOL_TFX_TOKEN_NONE = 40,            /* "none"  */
  YYSYMBOL_TFX_TOKEN_CCW = 41,             /* "ccw"  */
  YYSYMBOL_TFX_TOKEN_CW = 42,              /* "cw"  */
  YYSYMBOL_TFX_TOKEN_POLYGON_MODE = 43,    /* "polygon_mode"  */
  YYSYMBOL_TFX_TOKEN_FILL = 44,            /* "fill"  */
  YYSYMBOL_TFX_TOKEN_LINE = 45,            /* "line"  */
  YYSYMBOL_TFX_TOKEN_POINT = 46,           /* "point"  */
  YYSYMBOL_TFX_TOKEN_PRIMITIVE_TOPOLOGY = 47, /* "primitive_topology"  */
  YYSYMBOL_TFX_TOKEN_PT_POINT_LIST = 48,   /* "point_list"  */
  YYSYMBOL_TFX_TOKEN_PT_LINE_LIST = 49,    /* "line_list"  */
  YYSYMBOL_TFX_TOKEN_PT_LINE_STRIP = 50,   /* "line_strip"  */
  YYSYMBOL_TFX_TOKEN_PT_TRIANGLE_LIST = 51, /* "triangle_list"  */
  YYSYMBOL_TFX_TOKEN_PT_TRIANGLE_FAN = 52, /* "triangle_fan"  */
  YYSYMBOL_TFX_TOKEN_PT_TRIANGLE_STRIP = 53, /* "triangle_strip"  */
  YYSYMBOL_TFX_TOKEN_PT_LINE_LIST_WITH_ADJACENCY = 54, /* "line_list_with_adjacency"  */
  YYSYMBOL_TFX_TOKEN_PT_LINE_STRIP_WITH_ADJACENCY = 55, /* "line_strip_with_adjacency"  */
  YYSYMBOL_TFX_TOKEN_PT_TRIANGLE_LIST_WITH_ADJACENCY = 56, /* "triangle_list_with_adjacency"  */
  YYSYMBOL_TFX_TOKEN_PT_TRIANGLE_STRIP_WITH_ADJACENCY = 57, /* "triangle_strip_with_adjacency"  */
  YYSYMBOL_TFX_TOKEN_PT_PATCH_LIST = 58,   /* "patch_list"  */
  YYSYMBOL_TFX_TOKEN_DEPTH = 59,           /* "depth"  */
  YYSYMBOL_TFX_TOKEN_STENCIL = 60,         /* "stencil"  */
  YYSYMBOL_TFX_TOKEN_TEST = 61,            /* "test"  */
  YYSYMBOL_TFX_TOKEN_WRITE = 62,           /* "write"  */
  YYSYMBOL_TFX_TOKEN_FAIL_OP = 63,         /* "fail_op"  */
  YYSYMBOL_TFX_TOKEN_PASS_OP = 64,         /* "pass_op"  */
  YYSYMBOL_TFX_TOKEN_DEPTH_FAIL_OP = 65,   /* "depth_fail_op"  */
  YYSYMBOL_TFX_TOKEN_COMPARE_OP = 66,      /* "compare_op"  */
  YYSYMBOL_TFX_TOKEN_REFERENCE_VALUE = 67, /* "ref_val"  */
  YYSYMBOL_TFX_TOKEN_NEVER = 68,           /* "never"  */
  YYSYMBOL_TFX_TOKEN_LESS = 69,            /* "less"  */
  YYSYMBOL_TFX_TOKEN_EQUAL = 70,           /* "equal"  */
  YYSYMBOL_TFX_TOKEN_LESS_OR_EQUAL = 71,   /* "less_or_equal"  */
  YYSYMBOL_TFX_TOKEN_GREATER = 72,         /* "greater"  */
  YYSYMBOL_TFX_TOKEN_NOT_EQUAL = 73,       /* "not_equal"  */
  YYSYMBOL_TFX_TOKEN_GREATER_OR_EQUAL = 74, /* "greater_or_equal"  */
  YYSYMBOL_TFX_TOKEN_ALWAYS = 75,          /* "always"  */
  YYSYMBOL_TFX_TOKEN_KEEP = 76,            /* "keep"  */
  YYSYMBOL_TFX_TOKEN_ZERO = 77,            /* "zero"  */
  YYSYMBOL_TFX_TOKEN_REPLACE = 78,         /* "replace"  */
  YYSYMBOL_TFX_TOKEN_INCREMENT_AND_CLAMP = 79, /* "inc_and_clamp"  */
  YYSYMBOL_TFX_TOKEN_DECREMENT_AND_CLAMP = 80, /* "dec_and_clamp"  */
  YYSYMBOL_TFX_TOKEN_INVERT = 81,          /* "invert"  */
  YYSYMBOL_TFX_TOKEN_INCREMENT_AND_WRAP = 82, /* "inc_and_wrap"  */
  YYSYMBOL_TFX_TOKEN_DECREMENT_AND_WRAP = 83, /* "dec_and_wrap"  */
  YYSYMBOL_TFX_TOKEN_BLENDING = 84,        /* "blending"  */
  YYSYMBOL_TFX_TOKEN_HAS_BLANDING = 85,    /* "has_blending"  */
  YYSYMBOL_TFX_TOKEN_LOGIC_OP_ENABLED = 86, /* "logic_op_enabled"  */
  YYSYMBOL_TFX_TOKEN_LOGIC_OP = 87,        /* "logic_op"  */
  YYSYMBOL_TFX_TOKEN_BLEND_CONSTANTS = 88, /* "blend_constants"  */
  YYSYMBOL_TFX_TOKEN_MRT = 89,             /* "mrt"  */
  YYSYMBOL_TFX_TOKEN_SRC_COLOR_BLEND_FACTOR = 90, /* "src_color_blend_factor"  */
  YYSYMBOL_TFX_TOKEN_DST_COLOR_BLEND_FACTOR = 91, /* "dst_color_blend_factor"  */
  YYSYMBOL_TFX_TOKEN_COLOR_BLEND_OP = 92,  /* "color_blend_op"  */
  YYSYMBOL_TFX_TOKEN_SRC_ALPHA_BLEND_FACTOR = 93, /* "src_alpha_blend_factor"  */
  YYSYMBOL_TFX_TOKEN_DST_ALPHA_BLEND_FACTOR = 94, /* "dst_alpha_blend_factor"  */
  YYSYMBOL_TFX_TOKEN_ALPHA_BLEND_OP = 95,  /* "alpha_blend_op"  */
  YYSYMBOL_TFX_TOKEN_ONE = 96,             /* "one"  */
  YYSYMBOL_TFX_TOKEN_SRC_COLOR = 97,       /* "src_color"  */
  YYSYMBOL_TFX_TOKEN_ONE_MINUS_SRC_COLOR = 98, /* "one_minus_src_color"  */
  YYSYMBOL_TFX_TOKEN_DST_COLOR = 99,       /* "dst_color"  */
  YYSYMBOL_TFX_TOKEN_ONE_MINUS_DST_COLOR = 100, /* "one_minus_dst_color"  */
  YYSYMBOL_TFX_TOKEN_SRC_ALPHA = 101,      /* "src_alpha"  */
  YYSYMBOL_TFX_TOKEN_ONE_MINUS_SRC_ALPHA = 102, /* "one_minus_src_alpha"  */
  YYSYMBOL_TFX_TOKEN_DST_ALPHA = 103,      /* "dst_alpha"  */
  YYSYMBOL_TFX_TOKEN_ONE_MINUS_DST_ALPHA = 104, /* "one_minus_dst_alpha"  */
  YYSYMBOL_TFX_TOKEN_CONSTANT_COLOR = 105, /* "constant_color"  */
  YYSYMBOL_TFX_TOKEN_ONE_MINUS_CONSTANT_COLOR = 106, /* "one_minus_constant_color"  */
  YYSYMBOL_TFX_TOKEN_CONSTANT_ALPHA = 107, /* "constant_alpha"  */
  YYSYMBOL_TFX_TOKEN_ONE_MINUS_CONSTANT_ALPHA = 108, /* "one_minus_constant_alpha"  */
  YYSYMBOL_TFX_TOKEN_SRC_ALPHA_SATURATE = 109, /* "src_alpha_saturate"  */
  YYSYMBOL_TFX_TOKEN_SRC_ONE_COLOR = 110,  /* "src_one_color"  */
  YYSYMBOL_TFX_TOKEN_ONE_MINUS_SRC_ONE_COLOR = 111, /* "one_minus_src_one_color"  */
  YYSYMBOL_TFX_TOKEN_SRC_ONE_ALPHA = 112,  /* "src_one_alpha"  */
  YYSYMBOL_TFX_TOKEN_ONE_MINUS_SRC_ONE_ALPHA = 113, /* "one_minus_src_one_alpha"  */
  YYSYMBOL_TFX_TOKEN_ADD = 114,            /* "add"  */
  YYSYMBOL_TFX_TOKEN_SUBTRACT = 115,       /* "subtract"  */
  YYSYMBOL_TFX_TOKEN_REVERSE_SUBTRACT = 116, /* "reverse_subtract"  */
  YYSYMBOL_TFX_TOKEN_MIN = 117,            /* "min"  */
  YYSYMBOL_TFX_TOKEN_MAX = 118,            /* "max"  */
  YYSYMBOL_TFX_TOKEN_CLEAR = 119,          /* "clear"  */
  YYSYMBOL_TFX_TOKEN_AND = 120,            /* "and"  */
  YYSYMBOL_TFX_TOKEN_AND_REVERSE = 121,    /* "and_reverse"  */
  YYSYMBOL_TFX_TOKEN_COPY = 122,           /* "copy"  */
  YYSYMBOL_TFX_TOKEN_AND_INVERTED = 123,   /* "and_inverted"  */
  YYSYMBOL_TFX_TOKEN_NO_OP = 124,          /* "no_op"  */
  YYSYMBOL_TFX_TOKEN_XOR = 125,            /* "xor"  */
  YYSYMBOL_TFX_TOKEN_OR = 126,             /* "or"  */
  YYSYMBOL_TFX_TOKEN_NOR = 127,            /* "nor"  */
  YYSYMBOL_TFX_TOKEN_EQUIVALENT = 128,     /* "equivalent"  */
  YYSYMBOL_TFX_TOKEN_OR_REVERSE = 129,     /* "or_reverse"  */
  YYSYMBOL_TFX_TOKEN_COPY_INVERTED = 130,  /* "copy_inverted"  */
  YYSYMBOL_TFX_TOKEN_OR_INVERTED = 131,    /* "or_inverted"  */
  YYSYMBOL_TFX_TOKEN_NAND = 132,           /* "nand"  */
  YYSYMBOL_TFX_TOKEN_SET = 133,            /* "set"  */
  YYSYMBOL_TFX_TOKEN_FLOAT = 134,          /* "float"  */
  YYSYMBOL_TFX_TOKEN_FLOAT2 = 135,         /* "float2"  */
  YYSYMBOL_TFX_TOKEN_FLOAT3 = 136,         /* "float3"  */
  YYSYMBOL_TFX_TOKEN_FLOAT4 = 137,         /* "float4"  */
  YYSYMBOL_TFX_TOKEN_FLOAT4_u8 = 138,      /* "float4_u8"  */
  YYSYMBOL_TFX_TOKEN_FLOAT4X4 = 139,       /* "float4x4"  */
  YYSYMBOL_TFX_TOKEN_UINT = 140,           /* "uint"  */
  YYSYMBOL_TFX_TOKEN_UINT2 = 141,          /* "uint2"  */
  YYSYMBOL_TFX_TOKEN_UINT3 = 142,          /* "uint3"  */
  YYSYMBOL_TFX_TOKEN_UINT4 = 143,          /* "uint4"  */
  YYSYMBOL_TFX_TOKEN_INT = 144,            /* "int"  */
  YYSYMBOL_TFX_TOKEN_INT2 = 145,           /* "int2"  */
  YYSYMBOL_TFX_TOKEN_INT3 = 146,           /* "int3"  */
  YYSYMBOL_TFX_TOKEN_INT4 = 147,           /* "int4"  */
  YYSYMBOL_TFX_TOKEN_ROBUFFER = 148,       /* "Buffer"  */
  YYSYMBOL_TFX_TOKEN_STRUCTUREDBUFFER = 149, /* "StructuredBuffer"  */
  YYSYMBOL_TFX_TOKEN_RWSTRUCTUREDBUFFER = 150, /* "RWStructuredBuffer"  */
  YYSYMBOL_TFX_TOKEN_RWBUFFER = 151,       /* "RWBuffer"  */
  YYSYMBOL_TFX_TOKEN_TEXTURE2D = 152,      /* "Texture2D"  */
  YYSYMBOL_TFX_TOKEN_TEXTURE3D = 153,      /* "Texture3D"  */
  YYSYMBOL_TFX_TOKEN_TEXTURE_CUBE = 154,   /* "TextureCube"  */
  YYSYMBOL_TFX_TOKEN_RWTEXTURE3D = 155,    /* "RWTexture3D"  */
  YYSYMBOL_TFX_TOKEN_RWTEXTURE2D = 156,    /* "RWTexture2D"  */
  YYSYMBOL_YYACCEPT = 157,                 /* $accept  */
  YYSYMBOL_MODULE_EXPRESSIONS = 158,       /* MODULE_EXPRESSIONS  */
  YYSYMBOL_MODULE_EXPRESSION = 159,        /* MODULE_EXPRESSION  */
  YYSYMBOL_TECHNIQUE_MACRO_DECLARATION = 160, /* TECHNIQUE_MACRO_DECLARATION  */
  YYSYMBOL_TECHNIQUE_DECLARATION = 161,    /* TECHNIQUE_DECLARATION  */
  YYSYMBOL_TECHNIQUE_EXP_LIST = 162,       /* TECHNIQUE_EXP_LIST  */
  YYSYMBOL_TECHNIQUE_EXP = 163,            /* TECHNIQUE_EXP  */
  YYSYMBOL_RENDER_STATE_EXP_LIST = 164,    /* RENDER_STATE_EXP_LIST  */
  YYSYMBOL_RENDER_STATE_EXP = 165,         /* RENDER_STATE_EXP  */
  YYSYMBOL_DEPTH_EXP_LIST = 166,           /* DEPTH_EXP_LIST  */
  YYSYMBOL_DEPTH_EXP = 167,                /* DEPTH_EXP  */
  YYSYMBOL_STENCIL_EXP_LIST = 168,         /* STENCIL_EXP_LIST  */
  YYSYMBOL_STENCIL_EXP = 169,              /* STENCIL_EXP  */
  YYSYMBOL_CULL_MODE = 170,                /* CULL_MODE  */
  YYSYMBOL_PRIMITIVE_TOPOLOGY = 171,       /* PRIMITIVE_TOPOLOGY  */
  YYSYMBOL_POLYGON_MODE = 172,             /* POLYGON_MODE  */
  YYSYMBOL_INPUT_BUFFER_LIST = 173,        /* INPUT_BUFFER_LIST  */
  YYSYMBOL_INPUT_BUFFER = 174,             /* INPUT_BUFFER  */
  YYSYMBOL_INPUT_ATTRIBUTE_LIST = 175,     /* INPUT_ATTRIBUTE_LIST  */
  YYSYMBOL_INPUT_ATTRIBUTE = 176,          /* INPUT_ATTRIBUTE  */
  YYSYMBOL_COMPARE_OP = 177,               /* COMPARE_OP  */
  YYSYMBOL_STENCIL_OP = 178,               /* STENCIL_OP  */
  YYSYMBOL_BLENDING_EXP_LIST = 179,        /* BLENDING_EXP_LIST  */
  YYSYMBOL_BLENDING_EXP = 180,             /* BLENDING_EXP  */
  YYSYMBOL_MRT_BLENDING_EXP_LIST = 181,    /* MRT_BLENDING_EXP_LIST  */
  YYSYMBOL_MRT_BLENDING_EXP = 182,         /* MRT_BLENDING_EXP  */
  YYSYMBOL_BLEND_FACTOR = 183,             /* BLEND_FACTOR  */
  YYSYMBOL_BLEND_OP = 184,                 /* BLEND_OP  */
  YYSYMBOL_LOGIC_OP = 185,                 /* LOGIC_OP  */
  YYSYMBOL_SCOPE_DECLARATION = 186,        /* SCOPE_DECLARATION  */
  YYSYMBOL_SCOPE_EXP_LIST = 187,           /* SCOPE_EXP_LIST  */
  YYSYMBOL_SCOPE_EXP = 188,                /* SCOPE_EXP  */
  YYSYMBOL_RESOURCE_RESERVE_EXP_LIST = 189, /* RESOURCE_RESERVE_EXP_LIST  */
  YYSYMBOL_RESOURCE_RESERVE_EXP = 190,     /* RESOURCE_RESERVE_EXP  */
  YYSYMBOL_ASSIGN_EXP = 191,               /* ASSIGN_EXP  */
  YYSYMBOL_RESOURCE_DECLARATION = 192,     /* RESOURCE_DECLARATION  */
  YYSYMBOL_FULL_RESOURCE_TYPE = 193,       /* FULL_RESOURCE_TYPE  */
  YYSYMBOL_RESOURCE_TYPE = 194,            /* RESOURCE_TYPE  */
  YYSYMBOL_RESOURCE_TYPE_WITH_STORAGE = 195, /* RESOURCE_TYPE_WITH_STORAGE  */
  YYSYMBOL_ATTRIBUTE_TYPE = 196,           /* ATTRIBUTE_TYPE  */
  YYSYMBOL_FLOAT4_VALUE = 197,             /* FLOAT4_VALUE  */
  YYSYMBOL_INT_VALUE = 198,                /* INT_VALUE  */
  YYSYMBOL_BOOL_VALUE = 199                /* BOOL_VALUE  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined SHSTYPE_IS_TRIVIAL && SHSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  12
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   361

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  157
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  43
/* YYNRULES -- Number of rules.  */
#define YYNRULES  184
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  361

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   411


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156
};

#if SHDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   277,   277,   279,   284,   286,   288,   293,   300,   308,
     312,   318,   321,   324,   327,   330,   333,   339,   343,   349,
     352,   355,   358,   361,   364,   367,   373,   377,   383,   386,
     389,   395,   399,   405,   408,   411,   414,   417,   420,   426,
     429,   432,   437,   440,   443,   446,   449,   452,   455,   458,
     461,   464,   467,   473,   476,   479,   485,   489,   496,   502,
     506,   513,   519,   522,   525,   528,   531,   534,   537,   540,
     546,   549,   552,   555,   558,   561,   564,   567,   573,   577,
     583,   586,   589,   592,   599,   603,   610,   613,   616,   619,
     622,   625,   628,   634,   637,   640,   643,   646,   649,   652,
     655,   658,   661,   664,   667,   670,   673,   676,   679,   682,
     685,   688,   694,   697,   700,   703,   706,   712,   715,   718,
     721,   724,   727,   730,   733,   736,   739,   742,   745,   748,
     751,   754,   757,   763,   771,   775,   782,   785,   788,   791,
     797,   801,   807,   810,   813,   816,   819,   822,   825,   831,
     834,   840,   843,   846,   852,   858,   865,   874,   877,   880,
     883,   889,   892,   895,   898,   901,   904,   907,   913,   916,
     919,   922,   925,   928,   931,   934,   937,   940,   943,   946,
     949,   952,   958,   964,   970
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  static const char *const yy_sname[] =
  {
  "end of file", "error", "invalid token", "TFX_TOKEN_FLOAT_VAL",
  "TFX_TOKEN_INT_VAL", "TFX_TOKEN_NAME_VAL", "TFX_TOKEN_BOOL_VAL",
  "TFX_TOKEN_HLSL_CODE", "@", ":", ";", "=", "(", ")", "{", "}", "[", "]",
  "<", ">", ",", "-", "input", "buffer", "scope", "reserve", "register",
  "texture", "sampler", "cbuffer", "extern", "channel", "descriptor_set",
  "technique", "technique_macro", "support", "activate", "compile",
  "render_state", "cull_mode", "none", "ccw", "cw", "polygon_mode", "fill",
  "line", "point", "primitive_topology", "point_list", "line_list",
  "line_strip", "triangle_list", "triangle_fan", "triangle_strip",
  "line_list_with_adjacency", "line_strip_with_adjacency",
  "triangle_list_with_adjacency", "triangle_strip_with_adjacency",
  "patch_list", "depth", "stencil", "test", "write", "fail_op", "pass_op",
  "depth_fail_op", "compare_op", "ref_val", "never", "less", "equal",
  "less_or_equal", "greater", "not_equal", "greater_or_equal", "always",
  "keep", "zero", "replace", "inc_and_clamp", "dec_and_clamp", "invert",
  "inc_and_wrap", "dec_and_wrap", "blending", "has_blending",
  "logic_op_enabled", "logic_op", "blend_constants", "mrt",
  "src_color_blend_factor", "dst_color_blend_factor", "color_blend_op",
  "src_alpha_blend_factor", "dst_alpha_blend_factor", "alpha_blend_op",
  "one", "src_color", "one_minus_src_color", "dst_color",
  "one_minus_dst_color", "src_alpha", "one_minus_src_alpha", "dst_alpha",
  "one_minus_dst_alpha", "constant_color", "one_minus_constant_color",
  "constant_alpha", "one_minus_constant_alpha", "src_alpha_saturate",
  "src_one_color", "one_minus_src_one_color", "src_one_alpha",
  "one_minus_src_one_alpha", "add", "subtract", "reverse_subtract", "min",
  "max", "clear", "and", "and_reverse", "copy", "and_inverted", "no_op",
  "xor", "or", "nor", "equivalent", "or_reverse", "copy_inverted",
  "or_inverted", "nand", "set", "float", "float2", "float3", "float4",
  "float4_u8", "float4x4", "uint", "uint2", "uint3", "uint4", "int",
  "int2", "int3", "int4", "Buffer", "StructuredBuffer",
  "RWStructuredBuffer", "RWBuffer", "Texture2D", "Texture3D",
  "TextureCube", "RWTexture3D", "RWTexture2D", "$accept",
  "MODULE_EXPRESSIONS", "MODULE_EXPRESSION", "TECHNIQUE_MACRO_DECLARATION",
  "TECHNIQUE_DECLARATION", "TECHNIQUE_EXP_LIST", "TECHNIQUE_EXP",
  "RENDER_STATE_EXP_LIST", "RENDER_STATE_EXP", "DEPTH_EXP_LIST",
  "DEPTH_EXP", "STENCIL_EXP_LIST", "STENCIL_EXP", "CULL_MODE",
  "PRIMITIVE_TOPOLOGY", "POLYGON_MODE", "INPUT_BUFFER_LIST",
  "INPUT_BUFFER", "INPUT_ATTRIBUTE_LIST", "INPUT_ATTRIBUTE", "COMPARE_OP",
  "STENCIL_OP", "BLENDING_EXP_LIST", "BLENDING_EXP",
  "MRT_BLENDING_EXP_LIST", "MRT_BLENDING_EXP", "BLEND_FACTOR", "BLEND_OP",
  "LOGIC_OP", "SCOPE_DECLARATION", "SCOPE_EXP_LIST", "SCOPE_EXP",
  "RESOURCE_RESERVE_EXP_LIST", "RESOURCE_RESERVE_EXP", "ASSIGN_EXP",
  "RESOURCE_DECLARATION", "FULL_RESOURCE_TYPE", "RESOURCE_TYPE",
  "RESOURCE_TYPE_WITH_STORAGE", "ATTRIBUTE_TYPE", "FLOAT4_VALUE",
  "INT_VALUE", "BOOL_VALUE", YY_NULLPTR
  };
  return yy_sname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411
};
#endif

#define YYPACT_NINF (-237)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-167)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -15,    45,    69,    94,   101,   -15,  -237,  -237,  -237,   103,
     104,   114,  -237,  -237,    -5,    33,    33,  -237,   144,  -237,
    -237,  -237,  -237,  -237,  -237,  -237,  -237,  -237,  -237,  -237,
    -237,  -237,  -237,  -237,  -237,  -237,  -237,  -237,   148,  -237,
    -237,  -237,  -237,   147,    -5,   172,   212,  -237,   200,   214,
     208,  -237,   198,   199,   213,   215,   211,    33,   216,    -1,
     217,  -237,  -237,     1,    34,   218,   219,   225,   228,   229,
     -17,   226,  -237,   227,   223,   230,  -237,   231,  -237,    -2,
       4,   220,   221,    -2,   233,   234,   235,   232,   237,   236,
     238,   239,   242,   244,   245,  -237,   -17,  -237,  -237,    75,
     251,    -1,   246,   247,  -237,  -237,   249,   240,  -237,  -237,
     252,  -237,  -237,  -237,   243,   241,    74,   115,    39,   -51,
      91,   -42,  -237,   248,   250,   253,   254,  -237,   256,   258,
      -2,   255,  -237,   259,   257,  -237,   241,  -237,  -237,  -237,
     261,  -237,  -237,  -237,   264,  -237,  -237,  -237,  -237,  -237,
    -237,  -237,  -237,  -237,  -237,  -237,   265,   266,   267,   268,
    -237,   -51,   269,   270,   271,   272,   273,   274,  -237,    91,
     275,   276,   277,   278,  -237,   -42,   251,   251,   251,  -237,
     263,   279,  -237,    -2,   281,   251,  -237,  -237,  -237,  -237,
     283,   283,    38,  -237,   283,    44,    44,    44,    38,   251,
    -237,   283,   -67,   119,   251,  -237,     3,    28,    59,  -237,
    -237,  -237,  -237,   280,  -237,   284,   285,  -237,  -237,  -237,
    -237,  -237,  -237,  -237,  -237,   286,   287,  -237,  -237,  -237,
    -237,  -237,  -237,  -237,  -237,   288,   289,   290,   291,   292,
     293,  -237,  -237,  -237,  -237,  -237,  -237,  -237,  -237,  -237,
    -237,  -237,  -237,  -237,  -237,  -237,  -237,   294,   295,   296,
     297,  -237,   251,  -237,   251,  -237,   251,   299,  -237,  -237,
    -237,  -237,  -237,  -237,  -237,  -237,  -237,  -237,  -237,   262,
    -237,   300,   298,   301,   302,    67,   303,   -58,  -237,  -237,
    -237,  -237,    67,   307,   310,   305,   306,   308,   309,   311,
     313,   314,  -237,   -58,  -237,   312,   315,   283,    87,    87,
     -33,    87,    87,   -33,  -237,   321,   324,   318,  -237,  -237,
    -237,  -237,  -237,  -237,  -237,  -237,  -237,  -237,  -237,  -237,
    -237,  -237,  -237,  -237,  -237,  -237,  -237,   319,   320,  -237,
    -237,  -237,  -237,  -237,   322,   323,   326,   327,   328,   325,
    -237,  -237,  -237,  -237,  -237,  -237,  -237,  -237,   331,   329,
    -237
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     3,     6,     5,     4,     0,
       0,     0,     1,     2,     0,     0,     0,   138,     0,   160,
     176,   177,   178,   179,   180,   181,   168,   169,   170,   171,
     172,   173,   174,   175,   167,   164,   165,   163,   157,   158,
     159,   161,   162,     0,   135,     0,     0,   156,     0,     0,
       0,    14,     0,     0,     0,     0,     0,    10,     0,     0,
       0,   134,   137,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     9,     0,     0,     0,   136,     0,   133,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    15,    18,     8,     7,     0,
       0,   141,     0,     0,   151,   183,     0,     0,   155,   154,
       0,    16,    11,    12,     0,     0,     0,     0,     0,     0,
       0,     0,    17,     0,     0,     0,     0,   140,     0,     0,
       0,     0,   139,     0,     0,    22,    57,    39,    41,    40,
       0,    53,    54,    55,     0,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,     0,     0,     0,     0,
      23,    27,     0,     0,     0,     0,     0,     0,    24,    32,
       0,     0,     0,     0,    25,    79,     0,     0,     0,   148,
       0,     0,   153,     0,     0,     0,    56,    19,    21,    20,
       0,     0,     0,    26,     0,     0,     0,     0,     0,     0,
      31,     0,     0,     0,     0,    78,     0,     0,     0,   149,
     150,   152,    13,     0,   184,     0,     0,    62,    63,    64,
      65,    66,    67,    68,    69,     0,     0,    70,    71,    72,
      73,    74,    75,    76,    77,     0,     0,     0,     0,     0,
       0,   127,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   128,   129,   130,   131,   132,     0,     0,     0,
       0,   147,     0,   143,     0,   145,     0,     0,    28,    29,
      30,    33,    34,    35,    36,    37,    38,    80,    81,     0,
      82,     0,     0,     0,     0,     0,     0,     0,   146,   142,
     144,    58,    60,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    85,    59,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    84,     0,     0,     0,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,     0,     0,   112,
     113,   114,   115,   116,     0,     0,     0,     0,     0,     0,
      86,    87,    88,    89,    90,    91,    92,    61,     0,     0,
     182
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -237,   334,  -237,  -237,  -237,    -9,  -237,   142,  -237,   157,
    -237,   136,  -237,  -237,  -237,  -237,   195,  -237,    48,  -237,
     143,    19,   168,  -237,    41,  -237,  -236,    35,  -237,  -237,
     316,  -237,   260,  -237,   -79,  -237,  -237,  -237,  -237,   -64,
    -237,   -99,  -188
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     4,     5,     6,     7,    56,    57,    95,    96,   160,
     161,   168,   169,   140,   156,   144,   135,   136,   291,   292,
     225,   235,   174,   175,   302,   303,   337,   344,   257,     8,
      43,    44,    76,    77,   104,    45,    46,    47,    48,    49,
     259,   107,   215
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      82,   126,    17,   216,   110,    88,   226,    58,   105,     1,
     157,   158,    79,   240,   241,   159,   261,    80,     2,     3,
      18,   106,    89,    19,   262,    74,    90,   295,   102,   103,
      91,    75,   296,   297,   298,   299,   300,   301,    50,    81,
      51,   263,    92,    93,   170,   171,   172,   173,    72,   264,
       9,   182,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,    94,    52,    53,
      54,    55,   265,   338,    10,   345,   346,   206,   207,   208,
     266,   339,   340,   341,   342,   343,   213,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   123,    11,
     239,    12,   124,   125,   211,   260,   217,   218,   219,   220,
     221,   222,   223,   224,   137,   138,   139,    14,    15,   317,
     227,   228,   229,   230,   231,   232,   233,   234,    16,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,   162,    59,   163,   164,   165,   166,   167,   141,
     142,   143,    60,   282,   318,   283,  -166,   284,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    62,   319,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,   236,   237,    63,    64,    65,
      66,   293,    67,    68,    70,    69,    71,    78,   293,    83,
      85,    73,    84,    86,    87,    99,    97,    98,   122,   108,
     109,   101,   100,   111,   112,   113,   115,   116,   133,   117,
     118,   119,   114,   120,   121,   105,   258,   131,   128,   129,
     130,   180,   132,   181,   134,   286,   183,   179,   176,   185,
     177,   187,   184,   178,   188,   189,   209,   190,   191,   192,
     194,   195,   196,   197,   198,   199,   201,   202,   203,   214,
     204,   212,   210,   267,   268,   269,   270,   271,   272,   273,
     274,   275,   276,   277,   278,   200,   280,   279,   285,   287,
     281,   288,   305,   306,   289,   290,   307,   308,   193,   309,
     310,   315,   311,   294,   312,   313,   348,   349,   350,   351,
     352,   186,   353,   354,   359,   316,   355,   356,   357,    13,
     304,   238,   360,   205,   314,   358,     0,     0,   347,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      61,   127
};

static const yytype_int16 yycheck[] =
{
      64,   100,     7,   191,    83,    22,   194,    16,     4,    24,
      61,    62,    11,   201,    81,    66,    13,    16,    33,    34,
      25,    17,    39,    28,    21,    26,    43,    85,    30,    31,
      47,    32,    90,    91,    92,    93,    94,    95,     5,     5,
       7,    13,    59,    60,    86,    87,    88,    89,    57,    21,
       5,   130,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,    84,    35,    36,
      37,    38,    13,   309,     5,   311,   312,   176,   177,   178,
      21,   114,   115,   116,   117,   118,   185,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    23,     5,
     199,     0,    27,    28,   183,   204,    68,    69,    70,    71,
      72,    73,    74,    75,    40,    41,    42,    14,    14,   307,
      76,    77,    78,    79,    80,    81,    82,    83,    14,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,    61,     9,    63,    64,    65,    66,    67,    44,
      45,    46,    15,   262,    77,   264,    18,   266,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,    10,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   196,   197,     5,    18,     5,
      12,   285,    24,    24,     9,    12,    15,    10,   292,    11,
       5,    15,    13,     5,     5,    12,    10,    10,    96,    19,
      19,    10,    12,    10,    10,    10,     9,    11,     5,    11,
      11,     9,    20,     9,     9,     4,   137,    17,    12,    12,
      11,     5,    10,     5,    23,     3,    11,    13,    20,    12,
      20,    10,    13,    20,    10,    10,    13,    11,    11,    11,
      11,    11,    11,    11,    11,    11,    11,    11,    11,     6,
      12,    10,    13,    13,    10,    10,    10,    10,    10,    10,
      10,    10,    10,    10,    10,   169,    10,    12,     9,     9,
      13,    13,     5,     3,    13,    13,    11,    11,   161,    11,
      11,     9,    11,    20,    11,    11,     5,     3,    10,    10,
      10,   136,    10,    10,     3,    20,    10,    10,    10,     5,
     292,   198,    13,   175,   303,    20,    -1,    -1,   313,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      44,   101
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    24,    33,    34,   158,   159,   160,   161,   186,     5,
       5,     5,     0,   158,    14,    14,    14,     7,    25,    28,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   187,   188,   192,   193,   194,   195,   196,
       5,     7,    35,    36,    37,    38,   162,   163,   162,     9,
      15,   187,    10,     5,    18,     5,    12,    24,    24,    12,
       9,    15,   162,    15,    26,    32,   189,   190,    10,    11,
      16,     5,   196,    11,    13,     5,     5,     5,    22,    39,
      43,    47,    59,    60,    84,   164,   165,    10,    10,    12,
      12,    10,    30,    31,   191,     4,    17,   198,    19,    19,
     191,    10,    10,    10,    20,     9,    11,    11,    11,     9,
       9,     9,   164,    23,    27,    28,   198,   189,    12,    12,
      11,    17,    10,     5,    23,   173,   174,    40,    41,    42,
     170,    44,    45,    46,   172,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,   171,    61,    62,    66,
     166,   167,    61,    63,    64,    65,    66,    67,   168,   169,
      86,    87,    88,    89,   179,   180,    20,    20,    20,    13,
       5,     5,   191,    11,    13,    12,   173,    10,    10,    10,
      11,    11,    11,   166,    11,    11,    11,    11,    11,    11,
     168,    11,    11,    11,    12,   179,   198,   198,   198,    13,
      13,   191,    10,   198,     6,   199,   199,    68,    69,    70,
      71,    72,    73,    74,    75,   177,   199,    76,    77,    78,
      79,    80,    81,    82,    83,   178,   178,   178,   177,   198,
     199,    81,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   185,   137,   197,
     198,    13,    21,    13,    21,    13,    21,    13,    10,    10,
      10,    10,    10,    10,    10,    10,    10,    10,    10,    12,
      10,    13,   198,   198,   198,     9,     3,     9,    13,    13,
      13,   175,   176,   196,    20,    85,    90,    91,    92,    93,
      94,    95,   181,   182,   175,     5,     3,    11,    11,    11,
      11,    11,    11,    11,   181,     9,    20,   199,    77,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   183,   183,   114,
     115,   116,   117,   118,   184,   183,   183,   184,     5,     3,
      10,    10,    10,    10,    10,    10,    10,    10,    20,     3,
      13
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   157,   158,   158,   159,   159,   159,   160,   161,   162,
     162,   163,   163,   163,   163,   163,   163,   164,   164,   165,
     165,   165,   165,   165,   165,   165,   166,   166,   167,   167,
     167,   168,   168,   169,   169,   169,   169,   169,   169,   170,
     170,   170,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   172,   172,   172,   173,   173,   174,   175,
     175,   176,   177,   177,   177,   177,   177,   177,   177,   177,
     178,   178,   178,   178,   178,   178,   178,   178,   179,   179,
     180,   180,   180,   180,   181,   181,   182,   182,   182,   182,
     182,   182,   182,   183,   183,   183,   183,   183,   183,   183,
     183,   183,   183,   183,   183,   183,   183,   183,   183,   183,
     183,   183,   184,   184,   184,   184,   184,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   186,   187,   187,   188,   188,   188,   188,
     189,   189,   190,   190,   190,   190,   190,   190,   190,   191,
     191,   192,   192,   192,   193,   193,   193,   194,   194,   194,
     194,   195,   195,   195,   195,   195,   195,   195,   196,   196,
     196,   196,   196,   196,   196,   196,   196,   196,   196,   196,
     196,   196,   197,   198,   199
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     1,     6,     6,     2,
       1,     4,     4,     7,     1,     3,     4,     2,     1,     4,
       4,     4,     3,     3,     3,     3,     2,     1,     4,     4,
       4,     2,     1,     4,     4,     4,     4,     4,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     1,     6,     2,
       1,     5,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     1,
       4,     4,     4,     6,     2,     1,     4,     4,     4,     4,
       4,     4,     4,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     6,     2,     1,     3,     2,     1,     5,
       3,     2,     8,     6,     8,     6,     8,     6,     4,     4,
       4,     4,     7,     6,     4,     4,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,    10,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = SHEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == SHEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (compiler, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use SHerror or SHUNDEF. */
#define YYERRCODE SHUNDEF


/* Enable debugging if requested.  */
#if SHDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
# ifndef YY_LOCATION_PRINT
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, compiler); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, Compiler& compiler)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (compiler);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, Compiler& compiler)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep, compiler);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule, Compiler& compiler)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)], compiler);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, compiler); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !SHDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !SHDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif



static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yystrlen (yysymbol_name (yyarg[yyi]));
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp = yystpcpy (yyp, yysymbol_name (yyarg[yyi++]));
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, Compiler& compiler)
{
  YYUSE (yyvaluep);
  YYUSE (compiler);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (Compiler& compiler)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = SHEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == SHEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (compiler);
    }

  if (yychar <= SHEOF)
    {
      yychar = SHEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == SHerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = SHUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = SHEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* MODULE_EXPRESSIONS: MODULE_EXPRESSION MODULE_EXPRESSIONS  */
#line 277 "src/shaders_compiler/parser/parser.y"
                                         {
  }
#line 1805 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 3: /* MODULE_EXPRESSIONS: MODULE_EXPRESSION  */
#line 279 "src/shaders_compiler/parser/parser.y"
                      {
  }
#line 1812 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 4: /* MODULE_EXPRESSION: SCOPE_DECLARATION  */
#line 284 "src/shaders_compiler/parser/parser.y"
                      {
  }
#line 1819 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 5: /* MODULE_EXPRESSION: TECHNIQUE_DECLARATION  */
#line 286 "src/shaders_compiler/parser/parser.y"
                          {
  }
#line 1826 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 6: /* MODULE_EXPRESSION: TECHNIQUE_MACRO_DECLARATION  */
#line 288 "src/shaders_compiler/parser/parser.y"
                                {
  }
#line 1833 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 7: /* TECHNIQUE_MACRO_DECLARATION: "technique_macro" TFX_TOKEN_NAME_VAL "{" TECHNIQUE_EXP_LIST "}" ";"  */
#line 293 "src/shaders_compiler/parser/parser.y"
                                                                                    {
    const bool isOk = compiler.onTechniqueMacroDeclaration(new TechniqueMacroDeclarationExp{(yyvsp[-4].sval), (yyvsp[-2].techniqueExp)});
    if (!isOk)
      YYABORT;
}
#line 1843 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 8: /* TECHNIQUE_DECLARATION: "technique" TFX_TOKEN_NAME_VAL "{" TECHNIQUE_EXP_LIST "}" ";"  */
#line 300 "src/shaders_compiler/parser/parser.y"
                                                                              {
    const bool isOk = compiler.onTechniqueDeclaration(new TechniqueDeclarationExp{(yyvsp[-4].sval), (yyvsp[-2].techniqueExp)});
    if (!isOk)
      YYABORT;
  }
#line 1853 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 9: /* TECHNIQUE_EXP_LIST: TECHNIQUE_EXP TECHNIQUE_EXP_LIST  */
#line 308 "src/shaders_compiler/parser/parser.y"
                                                {
    (yyval.techniqueExp) = (yyvsp[-1].techniqueExp);
    (yyval.techniqueExp)->next = (yyvsp[0].techniqueExp);
  }
#line 1862 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 10: /* TECHNIQUE_EXP_LIST: TECHNIQUE_EXP  */
#line 312 "src/shaders_compiler/parser/parser.y"
                       {
    (yyval.techniqueExp) = (yyvsp[0].techniqueExp);
  }
#line 1870 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 11: /* TECHNIQUE_EXP: "support" "scope" TFX_TOKEN_NAME_VAL ";"  */
#line 318 "src/shaders_compiler/parser/parser.y"
                                                   {
    (yyval.techniqueExp) = new ScopeSupportExp((yyvsp[-1].sval));
  }
#line 1878 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 12: /* TECHNIQUE_EXP: "activate" "scope" TFX_TOKEN_NAME_VAL ";"  */
#line 321 "src/shaders_compiler/parser/parser.y"
                                                    {
    (yyval.techniqueExp) = new ScopeActivateExp((yyvsp[-1].sval));
  }
#line 1886 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 13: /* TECHNIQUE_EXP: "compile" "(" TFX_TOKEN_NAME_VAL "," TFX_TOKEN_NAME_VAL ")" ";"  */
#line 324 "src/shaders_compiler/parser/parser.y"
                                                                                   {
    (yyval.techniqueExp) = new CompileExp((yyvsp[-4].sval), (yyvsp[-2].sval));
  }
#line 1894 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 14: /* TECHNIQUE_EXP: TFX_TOKEN_HLSL_CODE  */
#line 327 "src/shaders_compiler/parser/parser.y"
                              {
    (yyval.techniqueExp) = new HlslExp((yyvsp[0].sval));
  }
#line 1902 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 15: /* TECHNIQUE_EXP: "render_state" ":" RENDER_STATE_EXP_LIST  */
#line 330 "src/shaders_compiler/parser/parser.y"
                                                 {
    (yyval.techniqueExp) = (yyvsp[0].renderStateExp);
  }
#line 1910 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 16: /* TECHNIQUE_EXP: TFX_TOKEN_NAME_VAL "(" ")" ";"  */
#line 333 "src/shaders_compiler/parser/parser.y"
                                         {
    (yyval.techniqueExp) = new TechniqueMacroInvokeExp((yyvsp[-3].sval));
  }
#line 1918 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 17: /* RENDER_STATE_EXP_LIST: RENDER_STATE_EXP RENDER_STATE_EXP_LIST  */
#line 339 "src/shaders_compiler/parser/parser.y"
                                                        {
    (yyval.renderStateExp) = (yyvsp[-1].renderStateExp);
    (yyval.renderStateExp)->next = (yyvsp[0].renderStateExp);
  }
#line 1927 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 18: /* RENDER_STATE_EXP_LIST: RENDER_STATE_EXP  */
#line 343 "src/shaders_compiler/parser/parser.y"
                            {
    (yyval.renderStateExp) = (yyvsp[0].renderStateExp);
  }
#line 1935 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 19: /* RENDER_STATE_EXP: "cull_mode" "=" CULL_MODE ";"  */
#line 349 "src/shaders_compiler/parser/parser.y"
                                     {
    (yyval.renderStateExp) = new CullModeExp((yyvsp[-1].cullMode));
  }
#line 1943 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 20: /* RENDER_STATE_EXP: "primitive_topology" "=" PRIMITIVE_TOPOLOGY ";"  */
#line 352 "src/shaders_compiler/parser/parser.y"
                                                      {
    (yyval.renderStateExp) = new PrimitiveTopologyExp((yyvsp[-1].primitiveTopology));
  }
#line 1951 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 21: /* RENDER_STATE_EXP: "polygon_mode" "=" POLYGON_MODE ";"  */
#line 355 "src/shaders_compiler/parser/parser.y"
                                           {
    (yyval.renderStateExp) = new PolygonModeExp((yyvsp[-1].polygonMode));
  }
#line 1959 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 22: /* RENDER_STATE_EXP: "input" ":" INPUT_BUFFER_LIST  */
#line 358 "src/shaders_compiler/parser/parser.y"
                                           {
    (yyval.renderStateExp) = new InputExp((yyvsp[0].inputBufferExp));
  }
#line 1967 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 23: /* RENDER_STATE_EXP: "depth" ":" DEPTH_EXP_LIST  */
#line 361 "src/shaders_compiler/parser/parser.y"
                                     {
    (yyval.renderStateExp) = (yyvsp[0].depthExp);
  }
#line 1975 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 24: /* RENDER_STATE_EXP: "stencil" ":" STENCIL_EXP_LIST  */
#line 364 "src/shaders_compiler/parser/parser.y"
                                         {
    (yyval.renderStateExp) = (yyvsp[0].stencilExp);
  }
#line 1983 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 25: /* RENDER_STATE_EXP: "blending" ":" BLENDING_EXP_LIST  */
#line 367 "src/shaders_compiler/parser/parser.y"
                                           {
    (yyval.renderStateExp) = (yyvsp[0].blendingExp);
  }
#line 1991 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 26: /* DEPTH_EXP_LIST: DEPTH_EXP DEPTH_EXP_LIST  */
#line 373 "src/shaders_compiler/parser/parser.y"
                                        {
    (yyval.depthExp) = (yyvsp[-1].depthExp);
    (yyval.depthExp)->next = (yyvsp[0].depthExp);
  }
#line 2000 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 27: /* DEPTH_EXP_LIST: DEPTH_EXP  */
#line 377 "src/shaders_compiler/parser/parser.y"
                   {
    (yyval.depthExp) = (yyvsp[0].depthExp);
  }
#line 2008 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 28: /* DEPTH_EXP: "test" "=" BOOL_VALUE ";"  */
#line 383 "src/shaders_compiler/parser/parser.y"
                                 {
    (yyval.depthExp) = new DepthTestExp((yyvsp[-1].bval));
  }
#line 2016 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 29: /* DEPTH_EXP: "write" "=" BOOL_VALUE ";"  */
#line 386 "src/shaders_compiler/parser/parser.y"
                                  {
    (yyval.depthExp) = new DepthWriteExp((yyvsp[-1].bval));
  }
#line 2024 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 30: /* DEPTH_EXP: "compare_op" "=" COMPARE_OP ";"  */
#line 389 "src/shaders_compiler/parser/parser.y"
                                        {
    (yyval.depthExp) = new DepthOpExp((yyvsp[-1].compareOp));
  }
#line 2032 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 31: /* STENCIL_EXP_LIST: STENCIL_EXP STENCIL_EXP_LIST  */
#line 395 "src/shaders_compiler/parser/parser.y"
                                            {
    (yyval.stencilExp) = (yyvsp[-1].stencilExp);
    (yyval.stencilExp)->next = (yyvsp[0].stencilExp);
  }
#line 2041 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 32: /* STENCIL_EXP_LIST: STENCIL_EXP  */
#line 399 "src/shaders_compiler/parser/parser.y"
                     {
    (yyval.stencilExp) = (yyvsp[0].stencilExp);
  }
#line 2049 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 33: /* STENCIL_EXP: "test" "=" BOOL_VALUE ";"  */
#line 405 "src/shaders_compiler/parser/parser.y"
                                 {
    (yyval.stencilExp) = new StencilTestExp((yyvsp[-1].bval));
  }
#line 2057 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 34: /* STENCIL_EXP: "fail_op" "=" STENCIL_OP ";"  */
#line 408 "src/shaders_compiler/parser/parser.y"
                                     {
    (yyval.stencilExp) = new StencilFailOpExp((yyvsp[-1].stencilOp));
  }
#line 2065 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 35: /* STENCIL_EXP: "pass_op" "=" STENCIL_OP ";"  */
#line 411 "src/shaders_compiler/parser/parser.y"
                                     {
    (yyval.stencilExp) = new StencilPassOpExp((yyvsp[-1].stencilOp));
  }
#line 2073 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 36: /* STENCIL_EXP: "depth_fail_op" "=" STENCIL_OP ";"  */
#line 414 "src/shaders_compiler/parser/parser.y"
                                           {
    (yyval.stencilExp) = new StencilDepthFailOpExp((yyvsp[-1].stencilOp));
  }
#line 2081 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 37: /* STENCIL_EXP: "compare_op" "=" COMPARE_OP ";"  */
#line 417 "src/shaders_compiler/parser/parser.y"
                                        {
    (yyval.stencilExp) = new StencilCompareOpExp((yyvsp[-1].compareOp));
  }
#line 2089 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 38: /* STENCIL_EXP: "ref_val" "=" INT_VALUE ";"  */
#line 420 "src/shaders_compiler/parser/parser.y"
                                   {
    (yyval.stencilExp) = new StencilReferenceValueExp((yyvsp[-1].ival));
  }
#line 2097 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 39: /* CULL_MODE: "none"  */
#line 426 "src/shaders_compiler/parser/parser.y"
           {
    (yyval.cullMode) = gapi::CullMode::None;
  }
#line 2105 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 40: /* CULL_MODE: "cw"  */
#line 429 "src/shaders_compiler/parser/parser.y"
         {
    (yyval.cullMode) = gapi::CullMode::CW;
  }
#line 2113 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 41: /* CULL_MODE: "ccw"  */
#line 432 "src/shaders_compiler/parser/parser.y"
          {
    (yyval.cullMode) = gapi::CullMode::CCW;
  }
#line 2121 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 42: /* PRIMITIVE_TOPOLOGY: "point_list"  */
#line 437 "src/shaders_compiler/parser/parser.y"
                 {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::PointList;
  }
#line 2129 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 43: /* PRIMITIVE_TOPOLOGY: "line_list"  */
#line 440 "src/shaders_compiler/parser/parser.y"
                {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::LineList;
  }
#line 2137 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 44: /* PRIMITIVE_TOPOLOGY: "line_strip"  */
#line 443 "src/shaders_compiler/parser/parser.y"
                 {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::LineStrip;
  }
#line 2145 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 45: /* PRIMITIVE_TOPOLOGY: "triangle_list"  */
#line 446 "src/shaders_compiler/parser/parser.y"
                    {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::TriangleList;
  }
#line 2153 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 46: /* PRIMITIVE_TOPOLOGY: "triangle_fan"  */
#line 449 "src/shaders_compiler/parser/parser.y"
                   {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::TriangleFan;
  }
#line 2161 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 47: /* PRIMITIVE_TOPOLOGY: "triangle_strip"  */
#line 452 "src/shaders_compiler/parser/parser.y"
                     {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::TriangleStrip;
  }
#line 2169 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 48: /* PRIMITIVE_TOPOLOGY: "line_list_with_adjacency"  */
#line 455 "src/shaders_compiler/parser/parser.y"
                               {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::LineListWithAdjacency;
  }
#line 2177 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 49: /* PRIMITIVE_TOPOLOGY: "line_strip_with_adjacency"  */
#line 458 "src/shaders_compiler/parser/parser.y"
                                {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::LineStripWithAdjacency;
  }
#line 2185 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 50: /* PRIMITIVE_TOPOLOGY: "triangle_list_with_adjacency"  */
#line 461 "src/shaders_compiler/parser/parser.y"
                                   {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::TriangleListWithAdjacency;
  }
#line 2193 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 51: /* PRIMITIVE_TOPOLOGY: "triangle_strip_with_adjacency"  */
#line 464 "src/shaders_compiler/parser/parser.y"
                                    {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::TriangleStripWithAdjacency;
  }
#line 2201 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 52: /* PRIMITIVE_TOPOLOGY: "patch_list"  */
#line 467 "src/shaders_compiler/parser/parser.y"
                 {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::PatchList;
  }
#line 2209 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 53: /* POLYGON_MODE: "fill"  */
#line 473 "src/shaders_compiler/parser/parser.y"
           {
    (yyval.polygonMode) = gapi::PolygonMode::Fill;
  }
#line 2217 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 54: /* POLYGON_MODE: "line"  */
#line 476 "src/shaders_compiler/parser/parser.y"
           {
    (yyval.polygonMode) = gapi::PolygonMode::Line;
  }
#line 2225 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 55: /* POLYGON_MODE: "point"  */
#line 479 "src/shaders_compiler/parser/parser.y"
            {
    (yyval.polygonMode) = gapi::PolygonMode::Point;
  }
#line 2233 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 56: /* INPUT_BUFFER_LIST: INPUT_BUFFER INPUT_BUFFER_LIST  */
#line 485 "src/shaders_compiler/parser/parser.y"
                                                 {
    (yyval.inputBufferExp) = (yyvsp[-1].inputBufferExp);
    (yyval.inputBufferExp)->next = (yyvsp[0].inputBufferExp);
  }
#line 2242 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 57: /* INPUT_BUFFER_LIST: INPUT_BUFFER  */
#line 489 "src/shaders_compiler/parser/parser.y"
                         {
    (yyval.inputBufferExp) = (yyvsp[0].inputBufferExp);
    (yyval.inputBufferExp)->next = nullptr;
  }
#line 2251 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 58: /* INPUT_BUFFER: "buffer" "(" INT_VALUE ")" ":" INPUT_ATTRIBUTE_LIST  */
#line 496 "src/shaders_compiler/parser/parser.y"
                                                                    {
    (yyval.inputBufferExp) = new InputBufferExp((yyvsp[-3].ival), (yyvsp[0].inputAttributeExp));
  }
#line 2259 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 59: /* INPUT_ATTRIBUTE_LIST: INPUT_ATTRIBUTE INPUT_ATTRIBUTE_LIST  */
#line 502 "src/shaders_compiler/parser/parser.y"
                                                     {
    (yyval.inputAttributeExp) = (yyvsp[-1].inputAttributeExp);
    (yyval.inputAttributeExp)->next = (yyvsp[0].inputAttributeExp);
  }
#line 2268 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 60: /* INPUT_ATTRIBUTE_LIST: INPUT_ATTRIBUTE  */
#line 506 "src/shaders_compiler/parser/parser.y"
                          {
    (yyval.inputAttributeExp) = (yyvsp[0].inputAttributeExp);
    (yyval.inputAttributeExp)->next = nullptr;
  }
#line 2277 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 61: /* INPUT_ATTRIBUTE: ATTRIBUTE_TYPE TFX_TOKEN_NAME_VAL ":" TFX_TOKEN_NAME_VAL ";"  */
#line 513 "src/shaders_compiler/parser/parser.y"
                                                                                       {
    (yyval.inputAttributeExp) = new InputAttributeExp((yyvsp[-4].attributeType), (yyvsp[-3].sval), (yyvsp[-1].sval));
  }
#line 2285 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 62: /* COMPARE_OP: "never"  */
#line 519 "src/shaders_compiler/parser/parser.y"
            {
   (yyval.compareOp) = gapi::CompareOp::Never;
  }
#line 2293 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 63: /* COMPARE_OP: "less"  */
#line 522 "src/shaders_compiler/parser/parser.y"
           {
   (yyval.compareOp) = gapi::CompareOp::Less;
  }
#line 2301 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 64: /* COMPARE_OP: "equal"  */
#line 525 "src/shaders_compiler/parser/parser.y"
            {
   (yyval.compareOp) = gapi::CompareOp::Equal;
  }
#line 2309 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 65: /* COMPARE_OP: "less_or_equal"  */
#line 528 "src/shaders_compiler/parser/parser.y"
                    {
   (yyval.compareOp) = gapi::CompareOp::LessOrEqual;
  }
#line 2317 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 66: /* COMPARE_OP: "greater"  */
#line 531 "src/shaders_compiler/parser/parser.y"
              {
   (yyval.compareOp) = gapi::CompareOp::Greater;
  }
#line 2325 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 67: /* COMPARE_OP: "not_equal"  */
#line 534 "src/shaders_compiler/parser/parser.y"
                {
   (yyval.compareOp) = gapi::CompareOp::NotEqual;
  }
#line 2333 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 68: /* COMPARE_OP: "greater_or_equal"  */
#line 537 "src/shaders_compiler/parser/parser.y"
                       {
   (yyval.compareOp) = gapi::CompareOp::GreaterOrEqual;
  }
#line 2341 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 69: /* COMPARE_OP: "always"  */
#line 540 "src/shaders_compiler/parser/parser.y"
             {
   (yyval.compareOp) = gapi::CompareOp::Always;
  }
#line 2349 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 70: /* STENCIL_OP: "keep"  */
#line 546 "src/shaders_compiler/parser/parser.y"
           {
    (yyval.stencilOp) = gapi::StencilOp::Keep;
  }
#line 2357 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 71: /* STENCIL_OP: "zero"  */
#line 549 "src/shaders_compiler/parser/parser.y"
           {
    (yyval.stencilOp) = gapi::StencilOp::Zero;
  }
#line 2365 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 72: /* STENCIL_OP: "replace"  */
#line 552 "src/shaders_compiler/parser/parser.y"
              {
    (yyval.stencilOp) = gapi::StencilOp::Replace;
  }
#line 2373 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 73: /* STENCIL_OP: "inc_and_clamp"  */
#line 555 "src/shaders_compiler/parser/parser.y"
                    {
    (yyval.stencilOp) = gapi::StencilOp::IncrementAndClamp;
  }
#line 2381 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 74: /* STENCIL_OP: "dec_and_clamp"  */
#line 558 "src/shaders_compiler/parser/parser.y"
                    {
    (yyval.stencilOp) = gapi::StencilOp::DecrementAndClamp;
  }
#line 2389 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 75: /* STENCIL_OP: "invert"  */
#line 561 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.stencilOp) = gapi::StencilOp::Invert;
  }
#line 2397 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 76: /* STENCIL_OP: "inc_and_wrap"  */
#line 564 "src/shaders_compiler/parser/parser.y"
                   {
    (yyval.stencilOp) = gapi::StencilOp::IncrementAndWrap;
  }
#line 2405 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 77: /* STENCIL_OP: "dec_and_wrap"  */
#line 567 "src/shaders_compiler/parser/parser.y"
                   {
    (yyval.stencilOp) = gapi::StencilOp::DecrementAndWrap;
  }
#line 2413 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 78: /* BLENDING_EXP_LIST: BLENDING_EXP BLENDING_EXP_LIST  */
#line 573 "src/shaders_compiler/parser/parser.y"
                                              {
    (yyval.blendingExp) = (yyvsp[-1].blendingExp);
    (yyval.blendingExp)->next = (yyvsp[0].blendingExp);
  }
#line 2422 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 79: /* BLENDING_EXP_LIST: BLENDING_EXP  */
#line 577 "src/shaders_compiler/parser/parser.y"
                      {
    (yyval.blendingExp) = (yyvsp[0].blendingExp);
    (yyval.blendingExp)->next = nullptr;
  }
#line 2431 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 80: /* BLENDING_EXP: "logic_op_enabled" "=" BOOL_VALUE ";"  */
#line 583 "src/shaders_compiler/parser/parser.y"
                                             {
    (yyval.blendingExp) = new LogicOpEnablingExp((yyvsp[-1].bval));
  }
#line 2439 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 81: /* BLENDING_EXP: "logic_op" "=" LOGIC_OP ";"  */
#line 586 "src/shaders_compiler/parser/parser.y"
                                    {
    (yyval.blendingExp) = new LogicOpExp((yyvsp[-1].logicOp));
  }
#line 2447 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 82: /* BLENDING_EXP: "blend_constants" "=" FLOAT4_VALUE ";"  */
#line 589 "src/shaders_compiler/parser/parser.y"
                                              {
    (yyval.blendingExp) = new BlendConstants((yyvsp[-1].f4val));
  }
#line 2455 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 83: /* BLENDING_EXP: "mrt" "(" INT_VALUE ")" ":" MRT_BLENDING_EXP_LIST  */
#line 592 "src/shaders_compiler/parser/parser.y"
                                                               {
    (yyvsp[0].mrtBlendingExp)->n = (yyvsp[-3].ival);
    (yyval.blendingExp) = (yyvsp[0].mrtBlendingExp);
  }
#line 2464 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 84: /* MRT_BLENDING_EXP_LIST: MRT_BLENDING_EXP MRT_BLENDING_EXP_LIST  */
#line 599 "src/shaders_compiler/parser/parser.y"
                                                     {
    (yyval.mrtBlendingExp) = (yyvsp[-1].mrtBlendingExp);
    (yyval.mrtBlendingExp)->next = (yyvsp[0].mrtBlendingExp);
  }
#line 2473 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 85: /* MRT_BLENDING_EXP_LIST: MRT_BLENDING_EXP  */
#line 603 "src/shaders_compiler/parser/parser.y"
                         {
    (yyval.mrtBlendingExp) = (yyvsp[0].mrtBlendingExp);
    (yyval.mrtBlendingExp)->next = nullptr;
  }
#line 2482 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 86: /* MRT_BLENDING_EXP: "has_blending" "=" BOOL_VALUE ";"  */
#line 610 "src/shaders_compiler/parser/parser.y"
                                         {
    (yyval.mrtBlendingExp) = new MrtBlendingEnabledExp((yyvsp[-1].bval));
  }
#line 2490 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 87: /* MRT_BLENDING_EXP: "src_color_blend_factor" "=" BLEND_FACTOR ";"  */
#line 613 "src/shaders_compiler/parser/parser.y"
                                                     {
    (yyval.mrtBlendingExp) = new MrtSrcColorBlendFactorExp((yyvsp[-1].blendFactor));
  }
#line 2498 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 88: /* MRT_BLENDING_EXP: "dst_color_blend_factor" "=" BLEND_FACTOR ";"  */
#line 616 "src/shaders_compiler/parser/parser.y"
                                                     {
    (yyval.mrtBlendingExp) = new MrtDstColorBlendFactorExp((yyvsp[-1].blendFactor));
  }
#line 2506 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 89: /* MRT_BLENDING_EXP: "color_blend_op" "=" BLEND_OP ";"  */
#line 619 "src/shaders_compiler/parser/parser.y"
                                          {
    (yyval.mrtBlendingExp) = new MrtColorBlendOpExp((yyvsp[-1].blendOp));
  }
#line 2514 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 90: /* MRT_BLENDING_EXP: "src_alpha_blend_factor" "=" BLEND_FACTOR ";"  */
#line 622 "src/shaders_compiler/parser/parser.y"
                                                     {
    (yyval.mrtBlendingExp) = new MrtSrcAlphaBlendFactorExp((yyvsp[-1].blendFactor));
  }
#line 2522 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 91: /* MRT_BLENDING_EXP: "dst_alpha_blend_factor" "=" BLEND_FACTOR ";"  */
#line 625 "src/shaders_compiler/parser/parser.y"
                                                     {
    (yyval.mrtBlendingExp) = new MrtDstAlphaBlendFactorExp((yyvsp[-1].blendFactor));
  }
#line 2530 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 92: /* MRT_BLENDING_EXP: "alpha_blend_op" "=" BLEND_OP ";"  */
#line 628 "src/shaders_compiler/parser/parser.y"
                                          {
    (yyval.mrtBlendingExp) = new MrtAlphaBlendOpExp((yyvsp[-1].blendOp));
  }
#line 2538 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 93: /* BLEND_FACTOR: "zero"  */
#line 634 "src/shaders_compiler/parser/parser.y"
           {
    (yyval.blendFactor) = gapi::BlendFactor::Zero;
  }
#line 2546 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 94: /* BLEND_FACTOR: "one"  */
#line 637 "src/shaders_compiler/parser/parser.y"
          {
    (yyval.blendFactor) = gapi::BlendFactor::One;
  }
#line 2554 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 95: /* BLEND_FACTOR: "src_color"  */
#line 640 "src/shaders_compiler/parser/parser.y"
                {
    (yyval.blendFactor) = gapi::BlendFactor::SrcColor;
  }
#line 2562 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 96: /* BLEND_FACTOR: "one_minus_src_color"  */
#line 643 "src/shaders_compiler/parser/parser.y"
                          {
    (yyval.blendFactor) = gapi::BlendFactor::OneMinusSrcColor;
  }
#line 2570 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 97: /* BLEND_FACTOR: "dst_color"  */
#line 646 "src/shaders_compiler/parser/parser.y"
                {
    (yyval.blendFactor) = gapi::BlendFactor::DstColor;
  }
#line 2578 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 98: /* BLEND_FACTOR: "one_minus_dst_color"  */
#line 649 "src/shaders_compiler/parser/parser.y"
                          {
    (yyval.blendFactor) = gapi::BlendFactor::OneMinusDstColor;
  }
#line 2586 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 99: /* BLEND_FACTOR: "src_alpha"  */
#line 652 "src/shaders_compiler/parser/parser.y"
                {
    (yyval.blendFactor) = gapi::BlendFactor::SrcAlpha;
  }
#line 2594 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 100: /* BLEND_FACTOR: "one_minus_src_alpha"  */
#line 655 "src/shaders_compiler/parser/parser.y"
                          {
    (yyval.blendFactor) = gapi::BlendFactor::OneMinusSrcAlpha;
  }
#line 2602 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 101: /* BLEND_FACTOR: "dst_alpha"  */
#line 658 "src/shaders_compiler/parser/parser.y"
                {
    (yyval.blendFactor) = gapi::BlendFactor::DstAlpha;
  }
#line 2610 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 102: /* BLEND_FACTOR: "one_minus_dst_alpha"  */
#line 661 "src/shaders_compiler/parser/parser.y"
                          {
    (yyval.blendFactor) = gapi::BlendFactor::OneMinusDstAlpha;
  }
#line 2618 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 103: /* BLEND_FACTOR: "constant_color"  */
#line 664 "src/shaders_compiler/parser/parser.y"
                     {
    (yyval.blendFactor) = gapi::BlendFactor::ConstantColor;
  }
#line 2626 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 104: /* BLEND_FACTOR: "one_minus_constant_color"  */
#line 667 "src/shaders_compiler/parser/parser.y"
                               {
    (yyval.blendFactor) = gapi::BlendFactor::OneMinusConstantColor;
  }
#line 2634 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 105: /* BLEND_FACTOR: "constant_alpha"  */
#line 670 "src/shaders_compiler/parser/parser.y"
                     {
    (yyval.blendFactor) = gapi::BlendFactor::ConstantAlpha;
  }
#line 2642 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 106: /* BLEND_FACTOR: "one_minus_constant_alpha"  */
#line 673 "src/shaders_compiler/parser/parser.y"
                               {
    (yyval.blendFactor) = gapi::BlendFactor::OneMinusConstantAlpha;
  }
#line 2650 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 107: /* BLEND_FACTOR: "src_alpha_saturate"  */
#line 676 "src/shaders_compiler/parser/parser.y"
                         {
    (yyval.blendFactor) = gapi::BlendFactor::SrcAlphaSaturate;
  }
#line 2658 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 108: /* BLEND_FACTOR: "src_one_color"  */
#line 679 "src/shaders_compiler/parser/parser.y"
                    {
    (yyval.blendFactor) = gapi::BlendFactor::SrcOneColor;
  }
#line 2666 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 109: /* BLEND_FACTOR: "one_minus_src_one_color"  */
#line 682 "src/shaders_compiler/parser/parser.y"
                              {
    (yyval.blendFactor) = gapi::BlendFactor::OneMinusSrcOneColor;
  }
#line 2674 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 110: /* BLEND_FACTOR: "src_one_alpha"  */
#line 685 "src/shaders_compiler/parser/parser.y"
                    {
    (yyval.blendFactor) = gapi::BlendFactor::SrcOneAlpha;
  }
#line 2682 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 111: /* BLEND_FACTOR: "one_minus_src_one_alpha"  */
#line 688 "src/shaders_compiler/parser/parser.y"
                              {
    (yyval.blendFactor) = gapi::BlendFactor::OneMinusSrcOneAlpha;
  }
#line 2690 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 112: /* BLEND_OP: "add"  */
#line 694 "src/shaders_compiler/parser/parser.y"
          {
    (yyval.blendOp) = gapi::BlendOp::Add;
  }
#line 2698 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 113: /* BLEND_OP: "subtract"  */
#line 697 "src/shaders_compiler/parser/parser.y"
               {
    (yyval.blendOp) = gapi::BlendOp::Subtract;
  }
#line 2706 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 114: /* BLEND_OP: "reverse_subtract"  */
#line 700 "src/shaders_compiler/parser/parser.y"
                       {
    (yyval.blendOp) = gapi::BlendOp::ReverseSubtract;
  }
#line 2714 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 115: /* BLEND_OP: "min"  */
#line 703 "src/shaders_compiler/parser/parser.y"
          {
    (yyval.blendOp) = gapi::BlendOp::Min;
  }
#line 2722 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 116: /* BLEND_OP: "max"  */
#line 706 "src/shaders_compiler/parser/parser.y"
          {
    (yyval.blendOp) = gapi::BlendOp::Max;
  }
#line 2730 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 117: /* LOGIC_OP: "clear"  */
#line 712 "src/shaders_compiler/parser/parser.y"
            {
    (yyval.logicOp) = gapi::LogicOp::Clear;
  }
#line 2738 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 118: /* LOGIC_OP: "and"  */
#line 715 "src/shaders_compiler/parser/parser.y"
          {
    (yyval.logicOp) = gapi::LogicOp::And;
  }
#line 2746 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 119: /* LOGIC_OP: "and_reverse"  */
#line 718 "src/shaders_compiler/parser/parser.y"
                  {
    (yyval.logicOp) = gapi::LogicOp::AndReverse;
  }
#line 2754 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 120: /* LOGIC_OP: "copy"  */
#line 721 "src/shaders_compiler/parser/parser.y"
           {
    (yyval.logicOp) = gapi::LogicOp::Copy;
  }
#line 2762 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 121: /* LOGIC_OP: "and_inverted"  */
#line 724 "src/shaders_compiler/parser/parser.y"
                   {
    (yyval.logicOp) = gapi::LogicOp::AndInverted;
  }
#line 2770 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 122: /* LOGIC_OP: "no_op"  */
#line 727 "src/shaders_compiler/parser/parser.y"
            {
    (yyval.logicOp) = gapi::LogicOp::NoOp;
  }
#line 2778 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 123: /* LOGIC_OP: "xor"  */
#line 730 "src/shaders_compiler/parser/parser.y"
          {
    (yyval.logicOp) = gapi::LogicOp::Xor;
  }
#line 2786 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 124: /* LOGIC_OP: "or"  */
#line 733 "src/shaders_compiler/parser/parser.y"
         {
    (yyval.logicOp) = gapi::LogicOp::Or;
  }
#line 2794 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 125: /* LOGIC_OP: "nor"  */
#line 736 "src/shaders_compiler/parser/parser.y"
          {
    (yyval.logicOp) = gapi::LogicOp::Nor;
  }
#line 2802 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 126: /* LOGIC_OP: "equivalent"  */
#line 739 "src/shaders_compiler/parser/parser.y"
                 {
    (yyval.logicOp) = gapi::LogicOp::Equivalent;
  }
#line 2810 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 127: /* LOGIC_OP: "invert"  */
#line 742 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.logicOp) = gapi::LogicOp::Invert;
  }
#line 2818 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 128: /* LOGIC_OP: "or_reverse"  */
#line 745 "src/shaders_compiler/parser/parser.y"
                 {
    (yyval.logicOp) = gapi::LogicOp::OrReverse;
  }
#line 2826 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 129: /* LOGIC_OP: "copy_inverted"  */
#line 748 "src/shaders_compiler/parser/parser.y"
                    {
    (yyval.logicOp) = gapi::LogicOp::CopyInverted;
  }
#line 2834 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 130: /* LOGIC_OP: "or_inverted"  */
#line 751 "src/shaders_compiler/parser/parser.y"
                  {
    (yyval.logicOp) = gapi::LogicOp::OrInverted;
  }
#line 2842 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 131: /* LOGIC_OP: "nand"  */
#line 754 "src/shaders_compiler/parser/parser.y"
           {
    (yyval.logicOp) = gapi::LogicOp::Nand;
  }
#line 2850 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 132: /* LOGIC_OP: "set"  */
#line 757 "src/shaders_compiler/parser/parser.y"
          {
    (yyval.logicOp) = gapi::LogicOp::Set;
  }
#line 2858 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 133: /* SCOPE_DECLARATION: "scope" TFX_TOKEN_NAME_VAL "{" SCOPE_EXP_LIST "}" ";"  */
#line 763 "src/shaders_compiler/parser/parser.y"
                                                                      {
    const bool isOk = compiler.onScopeDeclaration(new ScopeDeclarationExp{(yyvsp[-4].sval), (yyvsp[-2].scopeExp)});
    if (!isOk)
      YYABORT;
  }
#line 2868 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 134: /* SCOPE_EXP_LIST: SCOPE_EXP SCOPE_EXP_LIST  */
#line 771 "src/shaders_compiler/parser/parser.y"
                                       {
    (yyval.scopeExp) = (yyvsp[-1].scopeExp);
    (yyval.scopeExp)->next = (yyvsp[0].scopeExp);
  }
#line 2877 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 135: /* SCOPE_EXP_LIST: SCOPE_EXP  */
#line 775 "src/shaders_compiler/parser/parser.y"
                  {
    (yyval.scopeExp) = (yyvsp[0].scopeExp);
    (yyval.scopeExp)->next = nullptr;
  }
#line 2886 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 136: /* SCOPE_EXP: "reserve" ":" RESOURCE_RESERVE_EXP_LIST  */
#line 782 "src/shaders_compiler/parser/parser.y"
                                                  {
    (yyval.scopeExp) = new ShadersResourcesReserveExp((yyvsp[0].resReserveExp));
  }
#line 2894 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 137: /* SCOPE_EXP: RESOURCE_DECLARATION ";"  */
#line 785 "src/shaders_compiler/parser/parser.y"
                                   {
    (yyval.scopeExp) = (yyvsp[-1].resourceDeclExp);
  }
#line 2902 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 138: /* SCOPE_EXP: TFX_TOKEN_HLSL_CODE  */
#line 788 "src/shaders_compiler/parser/parser.y"
                              {
    (yyval.scopeExp) = new HlslResourceDeclarationExp((yyvsp[0].sval));
  }
#line 2910 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 139: /* SCOPE_EXP: ATTRIBUTE_TYPE TFX_TOKEN_NAME_VAL "=" ASSIGN_EXP ";"  */
#line 791 "src/shaders_compiler/parser/parser.y"
                                                                           {
    (yyval.scopeExp) = new CbufferVarDeclarationExp((yyvsp[-4].attributeType), (yyvsp[-3].sval), (yyvsp[-1].resourceAssignExp));
  }
#line 2918 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 140: /* RESOURCE_RESERVE_EXP_LIST: RESOURCE_RESERVE_EXP ";" RESOURCE_RESERVE_EXP_LIST  */
#line 797 "src/shaders_compiler/parser/parser.y"
                                                                {
  (yyval.resReserveExp) = (yyvsp[-2].resReserveExp);
  (yyval.resReserveExp)->next = (yyvsp[0].resReserveExp);
 }
#line 2927 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 141: /* RESOURCE_RESERVE_EXP_LIST: RESOURCE_RESERVE_EXP ";"  */
#line 801 "src/shaders_compiler/parser/parser.y"
                                {
   (yyval.resReserveExp) = (yyvsp[-1].resReserveExp);
 }
#line 2935 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 142: /* RESOURCE_RESERVE_EXP: "register" "(" "texture" "," INT_VALUE "-" INT_VALUE ")"  */
#line 807 "src/shaders_compiler/parser/parser.y"
                                                                     {
    (yyval.resReserveExp) = new RegistersReserveExp(RegistersReserveExp::Register::Texture, (yyvsp[-3].ival), (yyvsp[-1].ival));
  }
#line 2943 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 143: /* RESOURCE_RESERVE_EXP: "register" "(" "texture" "," INT_VALUE ")"  */
#line 810 "src/shaders_compiler/parser/parser.y"
                                                   {
    (yyval.resReserveExp) = new RegistersReserveExp(RegistersReserveExp::Register::Texture, (yyvsp[-1].ival), (yyvsp[-1].ival));
  }
#line 2951 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 144: /* RESOURCE_RESERVE_EXP: "register" "(" "sampler" "," INT_VALUE "-" INT_VALUE ")"  */
#line 813 "src/shaders_compiler/parser/parser.y"
                                                                     {
    (yyval.resReserveExp) = new RegistersReserveExp(RegistersReserveExp::Register::Sampler, (yyvsp[-3].ival), (yyvsp[-1].ival));
  }
#line 2959 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 145: /* RESOURCE_RESERVE_EXP: "register" "(" "sampler" "," INT_VALUE ")"  */
#line 816 "src/shaders_compiler/parser/parser.y"
                                                   {
    (yyval.resReserveExp) = new RegistersReserveExp(RegistersReserveExp::Register::Sampler, (yyvsp[-1].ival), (yyvsp[-1].ival));
  }
#line 2967 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 146: /* RESOURCE_RESERVE_EXP: "register" "(" "buffer" "," INT_VALUE "-" INT_VALUE ")"  */
#line 819 "src/shaders_compiler/parser/parser.y"
                                                                    {
    (yyval.resReserveExp) = new RegistersReserveExp(RegistersReserveExp::Register::Buffer, (yyvsp[-3].ival), (yyvsp[-1].ival));
  }
#line 2975 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 147: /* RESOURCE_RESERVE_EXP: "register" "(" "buffer" "," INT_VALUE ")"  */
#line 822 "src/shaders_compiler/parser/parser.y"
                                                  {
    (yyval.resReserveExp) = new RegistersReserveExp(RegistersReserveExp::Register::Buffer, (yyvsp[-1].ival), (yyvsp[-1].ival));
  }
#line 2983 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 148: /* RESOURCE_RESERVE_EXP: "descriptor_set" "(" INT_VALUE ")"  */
#line 825 "src/shaders_compiler/parser/parser.y"
                                          {
    (yyval.resReserveExp) = new DescriptorSetReserveExp((yyvsp[-1].ival));
  }
#line 2991 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 149: /* ASSIGN_EXP: "extern" "(" TFX_TOKEN_NAME_VAL ")"  */
#line 831 "src/shaders_compiler/parser/parser.y"
                                              {
    (yyval.resourceAssignExp) = new AccessResource(ResourceAccessType::Extern, (yyvsp[-1].sval));
  }
#line 2999 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 150: /* ASSIGN_EXP: "channel" "(" TFX_TOKEN_NAME_VAL ")"  */
#line 834 "src/shaders_compiler/parser/parser.y"
                                               {
    (yyval.resourceAssignExp) = new AccessResource(ResourceAccessType::Channel, (yyvsp[-1].sval));
  }
#line 3007 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 151: /* RESOURCE_DECLARATION: FULL_RESOURCE_TYPE TFX_TOKEN_NAME_VAL "=" ASSIGN_EXP  */
#line 840 "src/shaders_compiler/parser/parser.y"
                                                                           {
    (yyval.resourceDeclExp) = new ResourceDeclarationExp((yyvsp[-3].fullResourceType), 1, (yyvsp[-2].sval), (yyvsp[0].resourceAssignExp));
  }
#line 3015 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 152: /* RESOURCE_DECLARATION: FULL_RESOURCE_TYPE TFX_TOKEN_NAME_VAL "[" INT_VALUE "]" "=" ASSIGN_EXP  */
#line 843 "src/shaders_compiler/parser/parser.y"
                                                                                                {
    (yyval.resourceDeclExp) = new ResourceDeclarationExp((yyvsp[-6].fullResourceType), (yyvsp[-3].ival), (yyvsp[-5].sval), (yyvsp[0].resourceAssignExp));
  }
#line 3023 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 153: /* RESOURCE_DECLARATION: FULL_RESOURCE_TYPE TFX_TOKEN_NAME_VAL "[" "]" "=" ASSIGN_EXP  */
#line 846 "src/shaders_compiler/parser/parser.y"
                                                                                   {
    (yyval.resourceDeclExp) = new ResourceDeclarationExp((yyvsp[-5].fullResourceType), 0, (yyvsp[-4].sval), (yyvsp[0].resourceAssignExp));
  }
#line 3031 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 154: /* FULL_RESOURCE_TYPE: RESOURCE_TYPE_WITH_STORAGE "<" ATTRIBUTE_TYPE ">"  */
#line 852 "src/shaders_compiler/parser/parser.y"
                                                                             {
    (yyval.fullResourceType) = new FullResourceType {
      .type = (yyvsp[-3].resourceType),
      .storage = (yyvsp[-1].attributeType)
    };
  }
#line 3042 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 155: /* FULL_RESOURCE_TYPE: RESOURCE_TYPE_WITH_STORAGE "<" TFX_TOKEN_NAME_VAL ">"  */
#line 858 "src/shaders_compiler/parser/parser.y"
                                                                            {
    (yyval.fullResourceType) = new FullResourceType {
      .type = (yyvsp[-3].resourceType),
      .storage = string{(yyvsp[-1].sval)}
    };
    delete (yyvsp[-1].sval);
  }
#line 3054 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 156: /* FULL_RESOURCE_TYPE: RESOURCE_TYPE  */
#line 865 "src/shaders_compiler/parser/parser.y"
                        {
    (yyval.fullResourceType) = new FullResourceType {
      .type = (yyvsp[0].resourceType),
      .storage = gapi::AttributeType::None
    };
  }
#line 3065 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 157: /* RESOURCE_TYPE: "Texture2D"  */
#line 874 "src/shaders_compiler/parser/parser.y"
                        {
    (yyval.resourceType) = ResourceType::Texture2D;
  }
#line 3073 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 158: /* RESOURCE_TYPE: "Texture3D"  */
#line 877 "src/shaders_compiler/parser/parser.y"
                {
    (yyval.resourceType) = ResourceType::Texture3D;
  }
#line 3081 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 159: /* RESOURCE_TYPE: "TextureCube"  */
#line 880 "src/shaders_compiler/parser/parser.y"
                           {
    (yyval.resourceType) = ResourceType::TextureCube;
  }
#line 3089 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 160: /* RESOURCE_TYPE: "sampler"  */
#line 883 "src/shaders_compiler/parser/parser.y"
                      {
    (yyval.resourceType) = ResourceType::Sampler;
  }
#line 3097 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 161: /* RESOURCE_TYPE_WITH_STORAGE: "RWTexture3D"  */
#line 889 "src/shaders_compiler/parser/parser.y"
                  {
    (yyval.resourceType) = ResourceType::RWTexture3D;
  }
#line 3105 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 162: /* RESOURCE_TYPE_WITH_STORAGE: "RWTexture2D"  */
#line 892 "src/shaders_compiler/parser/parser.y"
                  {
    (yyval.resourceType) = ResourceType::RWTexture2D;
  }
#line 3113 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 163: /* RESOURCE_TYPE_WITH_STORAGE: "RWBuffer"  */
#line 895 "src/shaders_compiler/parser/parser.y"
               {
    (yyval.resourceType) = ResourceType::RWBuffer;
  }
#line 3121 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 164: /* RESOURCE_TYPE_WITH_STORAGE: "StructuredBuffer"  */
#line 898 "src/shaders_compiler/parser/parser.y"
                       {
    (yyval.resourceType) = ResourceType::StructuredBuffer;
  }
#line 3129 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 165: /* RESOURCE_TYPE_WITH_STORAGE: "RWStructuredBuffer"  */
#line 901 "src/shaders_compiler/parser/parser.y"
                         {
    (yyval.resourceType) = ResourceType::RWStructuredBuffer;
  }
#line 3137 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 166: /* RESOURCE_TYPE_WITH_STORAGE: "Texture2D"  */
#line 904 "src/shaders_compiler/parser/parser.y"
                {
    (yyval.resourceType) = ResourceType::Texture2D;
  }
#line 3145 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 167: /* RESOURCE_TYPE_WITH_STORAGE: "Buffer"  */
#line 907 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.resourceType) = ResourceType::Buffer;
  }
#line 3153 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 168: /* ATTRIBUTE_TYPE: "uint"  */
#line 913 "src/shaders_compiler/parser/parser.y"
                   {
    (yyval.attributeType) = gapi::AttributeType::UInt;
  }
#line 3161 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 169: /* ATTRIBUTE_TYPE: "uint2"  */
#line 916 "src/shaders_compiler/parser/parser.y"
                    {
    (yyval.attributeType) = gapi::AttributeType::UInt2;
  }
#line 3169 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 170: /* ATTRIBUTE_TYPE: "uint3"  */
#line 919 "src/shaders_compiler/parser/parser.y"
                    {
    (yyval.attributeType) = gapi::AttributeType::UInt3;
  }
#line 3177 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 171: /* ATTRIBUTE_TYPE: "uint4"  */
#line 922 "src/shaders_compiler/parser/parser.y"
                    {
    (yyval.attributeType) = gapi::AttributeType::UInt4;
  }
#line 3185 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 172: /* ATTRIBUTE_TYPE: "int"  */
#line 925 "src/shaders_compiler/parser/parser.y"
                  {
    (yyval.attributeType) = gapi::AttributeType::Int;
  }
#line 3193 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 173: /* ATTRIBUTE_TYPE: "int2"  */
#line 928 "src/shaders_compiler/parser/parser.y"
                   {
    (yyval.attributeType) = gapi::AttributeType::Int2;
  }
#line 3201 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 174: /* ATTRIBUTE_TYPE: "int3"  */
#line 931 "src/shaders_compiler/parser/parser.y"
                   {
    (yyval.attributeType) = gapi::AttributeType::Int3;
  }
#line 3209 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 175: /* ATTRIBUTE_TYPE: "int4"  */
#line 934 "src/shaders_compiler/parser/parser.y"
                   {
    (yyval.attributeType) = gapi::AttributeType::Int4;
  }
#line 3217 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 176: /* ATTRIBUTE_TYPE: "float"  */
#line 937 "src/shaders_compiler/parser/parser.y"
                    {
    (yyval.attributeType) = gapi::AttributeType::Float;
  }
#line 3225 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 177: /* ATTRIBUTE_TYPE: "float2"  */
#line 940 "src/shaders_compiler/parser/parser.y"
                     {
    (yyval.attributeType) = gapi::AttributeType::Float2;
  }
#line 3233 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 178: /* ATTRIBUTE_TYPE: "float3"  */
#line 943 "src/shaders_compiler/parser/parser.y"
                     {
    (yyval.attributeType) = gapi::AttributeType::Float3;
  }
#line 3241 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 179: /* ATTRIBUTE_TYPE: "float4"  */
#line 946 "src/shaders_compiler/parser/parser.y"
                     {
    (yyval.attributeType) = gapi::AttributeType::Float4;
  }
#line 3249 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 180: /* ATTRIBUTE_TYPE: "float4_u8"  */
#line 949 "src/shaders_compiler/parser/parser.y"
                        {
    (yyval.attributeType) = gapi::AttributeType::Float4_u8;
  }
#line 3257 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 181: /* ATTRIBUTE_TYPE: "float4x4"  */
#line 952 "src/shaders_compiler/parser/parser.y"
                       {
    (yyval.attributeType) = gapi::AttributeType::Float4x4;
  }
#line 3265 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 182: /* FLOAT4_VALUE: "float4" "(" TFX_TOKEN_FLOAT_VAL "," TFX_TOKEN_FLOAT_VAL "," TFX_TOKEN_FLOAT_VAL "," TFX_TOKEN_FLOAT_VAL ")"  */
#line 958 "src/shaders_compiler/parser/parser.y"
                                                                                                                                 {
    (yyval.f4val) = float4((yyvsp[-7].fval), (yyvsp[-5].fval), (yyvsp[-3].fval), (yyvsp[-1].fval));
  }
#line 3273 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 183: /* INT_VALUE: TFX_TOKEN_INT_VAL  */
#line 964 "src/shaders_compiler/parser/parser.y"
                         {
    (yyval.ival) = (yyvsp[0].ival);
  }
#line 3281 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 184: /* BOOL_VALUE: TFX_TOKEN_BOOL_VAL  */
#line 970 "src/shaders_compiler/parser/parser.y"
                          {
    (yyval.bval) = (yyvsp[0].bval);
  }
#line 3289 "src/shaders_compiler/parser/parser.tab.cpp"
    break;


#line 3293 "src/shaders_compiler/parser/parser.tab.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == SHEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (compiler, yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          goto yyexhaustedlab;
      }
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= SHEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == SHEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, compiler);
          yychar = SHEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, compiler);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if 1
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (compiler, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
  if (yychar != SHEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, compiler);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, compiler);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 974 "src/shaders_compiler/parser/parser.y"


void sherror(Compiler& compiler, const char* msg) {
  logerror("parsing error: {} [{}:{}]", msg, compiler.getCurrentFileName(), compiler.getLine());
  compiler.markCompilationFailed();
}
