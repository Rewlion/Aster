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
  YYSYMBOL_TFX_TOKEN_COMMA = 16,           /* ","  */
  YYSYMBOL_TFX_TOKEN_MINUS = 17,           /* "-"  */
  YYSYMBOL_TFX_TOKEN_INPUT = 18,           /* "input"  */
  YYSYMBOL_TFX_TOKEN_BUFFER = 19,          /* "buffer"  */
  YYSYMBOL_TFX_TOKEN_SCOPE = 20,           /* "scope"  */
  YYSYMBOL_TFX_TOKEN_RESERVE = 21,         /* "reserve"  */
  YYSYMBOL_TFX_TOKEN_REGISTER = 22,        /* "register"  */
  YYSYMBOL_TFX_TOKEN_TEXTURE = 23,         /* "texture"  */
  YYSYMBOL_TFX_TOKEN_SAMPLER = 24,         /* "sampler"  */
  YYSYMBOL_TFX_TOKEN_CBUFFER = 25,         /* "cbuffer"  */
  YYSYMBOL_TFX_TOKEN_EXTERN = 26,          /* "extern"  */
  YYSYMBOL_TFX_TOKEN_CHANNEL = 27,         /* "channel"  */
  YYSYMBOL_TFX_TOKEN_DESCRIPTOR_SET = 28,  /* "descriptor_set"  */
  YYSYMBOL_TFX_TOKEN_TECHNIQUE = 29,       /* "technique"  */
  YYSYMBOL_TFX_TOKEN_TECHNIQUE_MACRO = 30, /* "technique_macro"  */
  YYSYMBOL_TFX_TOKEN_SUPPORT = 31,         /* "support"  */
  YYSYMBOL_TFX_TOKEN_ACTIVATE = 32,        /* "activate"  */
  YYSYMBOL_TFX_TOKEN_COMPILE = 33,         /* "compile"  */
  YYSYMBOL_TFX_TOKEN_RENDER_STATE = 34,    /* "render_state"  */
  YYSYMBOL_TFX_TOKEN_CULL_MODE = 35,       /* "cull_mode"  */
  YYSYMBOL_TFX_TOKEN_NONE = 36,            /* "none"  */
  YYSYMBOL_TFX_TOKEN_CCW = 37,             /* "ccw"  */
  YYSYMBOL_TFX_TOKEN_CW = 38,              /* "cw"  */
  YYSYMBOL_TFX_TOKEN_PRIMITIVE_TOPOLOGY = 39, /* "primitive_topology"  */
  YYSYMBOL_TFX_TOKEN_PT_POINT_LIST = 40,   /* "point_list"  */
  YYSYMBOL_TFX_TOKEN_PT_LINE_LIST = 41,    /* "line_list"  */
  YYSYMBOL_TFX_TOKEN_PT_LINE_STRIP = 42,   /* "line_strip"  */
  YYSYMBOL_TFX_TOKEN_PT_TRIANGLE_LIST = 43, /* "triangle_list"  */
  YYSYMBOL_TFX_TOKEN_PT_TRIANGLE_FAN = 44, /* "triangle_fan"  */
  YYSYMBOL_TFX_TOKEN_PT_TRIANGLE_STRIP = 45, /* "triangle_strip"  */
  YYSYMBOL_TFX_TOKEN_PT_LINE_LIST_WITH_ADJACENCY = 46, /* "line_list_with_adjacency"  */
  YYSYMBOL_TFX_TOKEN_PT_LINE_STRIP_WITH_ADJACENCY = 47, /* "line_strip_with_adjacency"  */
  YYSYMBOL_TFX_TOKEN_PT_TRIANGLE_LIST_WITH_ADJACENCY = 48, /* "triangle_list_with_adjacency"  */
  YYSYMBOL_TFX_TOKEN_PT_TRIANGLE_STRIP_WITH_ADJACENCY = 49, /* "triangle_strip_with_adjacency"  */
  YYSYMBOL_TFX_TOKEN_PT_PATCH_LIST = 50,   /* "patch_list"  */
  YYSYMBOL_TFX_TOKEN_DEPTH = 51,           /* "depth"  */
  YYSYMBOL_TFX_TOKEN_STENCIL = 52,         /* "stencil"  */
  YYSYMBOL_TFX_TOKEN_TEST = 53,            /* "test"  */
  YYSYMBOL_TFX_TOKEN_WRITE = 54,           /* "write"  */
  YYSYMBOL_TFX_TOKEN_FAIL_OP = 55,         /* "fail_op"  */
  YYSYMBOL_TFX_TOKEN_PASS_OP = 56,         /* "pass_op"  */
  YYSYMBOL_TFX_TOKEN_DEPTH_FAIL_OP = 57,   /* "depth_fail_op"  */
  YYSYMBOL_TFX_TOKEN_COMPARE_OP = 58,      /* "compare_op"  */
  YYSYMBOL_TFX_TOKEN_REFERENCE_VALUE = 59, /* "ref_val"  */
  YYSYMBOL_TFX_TOKEN_NEVER = 60,           /* "never"  */
  YYSYMBOL_TFX_TOKEN_LESS = 61,            /* "less"  */
  YYSYMBOL_TFX_TOKEN_EQUAL = 62,           /* "equal"  */
  YYSYMBOL_TFX_TOKEN_LESS_OR_EQUAL = 63,   /* "less_or_equal"  */
  YYSYMBOL_TFX_TOKEN_GREATER = 64,         /* "greater"  */
  YYSYMBOL_TFX_TOKEN_NOT_EQUAL = 65,       /* "not_equal"  */
  YYSYMBOL_TFX_TOKEN_GREATER_OR_EQUAL = 66, /* "greater_or_equal"  */
  YYSYMBOL_TFX_TOKEN_ALWAYS = 67,          /* "always"  */
  YYSYMBOL_TFX_TOKEN_KEEP = 68,            /* "keep"  */
  YYSYMBOL_TFX_TOKEN_ZERO = 69,            /* "zero"  */
  YYSYMBOL_TFX_TOKEN_REPLACE = 70,         /* "replace"  */
  YYSYMBOL_TFX_TOKEN_INCREMENT_AND_CLAMP = 71, /* "inc_and_clamp"  */
  YYSYMBOL_TFX_TOKEN_DECREMENT_AND_CLAMP = 72, /* "dec_and_clamp"  */
  YYSYMBOL_TFX_TOKEN_INVERT = 73,          /* "invert"  */
  YYSYMBOL_TFX_TOKEN_INCREMENT_AND_WRAP = 74, /* "inc_and_wrap"  */
  YYSYMBOL_TFX_TOKEN_DECREMENT_AND_WRAP = 75, /* "dec_and_wrap"  */
  YYSYMBOL_TFX_TOKEN_BLENDING = 76,        /* "blending"  */
  YYSYMBOL_TFX_TOKEN_HAS_BLANDING = 77,    /* "has_blending"  */
  YYSYMBOL_TFX_TOKEN_LOGIC_OP_ENABLED = 78, /* "logic_op_enabled"  */
  YYSYMBOL_TFX_TOKEN_LOGIC_OP = 79,        /* "logic_op"  */
  YYSYMBOL_TFX_TOKEN_BLEND_CONSTANTS = 80, /* "blend_constants"  */
  YYSYMBOL_TFX_TOKEN_MRT = 81,             /* "mrt"  */
  YYSYMBOL_TFX_TOKEN_SRC_COLOR_BLEND_FACTOR = 82, /* "src_color_blend_factor"  */
  YYSYMBOL_TFX_TOKEN_DST_COLOR_BLEND_FACTOR = 83, /* "dst_color_blend_factor"  */
  YYSYMBOL_TFX_TOKEN_COLOR_BLEND_OP = 84,  /* "color_blend_op"  */
  YYSYMBOL_TFX_TOKEN_SRC_ALPHA_BLEND_FACTOR = 85, /* "src_alpha_blend_factor"  */
  YYSYMBOL_TFX_TOKEN_DST_ALPHA_BLEND_FACTOR = 86, /* "dst_alpha_blend_factor"  */
  YYSYMBOL_TFX_TOKEN_ALPHA_BLEND_OP = 87,  /* "alpha_blend_op"  */
  YYSYMBOL_TFX_TOKEN_ONE = 88,             /* "one"  */
  YYSYMBOL_TFX_TOKEN_SRC_COLOR = 89,       /* "src_color"  */
  YYSYMBOL_TFX_TOKEN_ONE_MINUS_SRC_COLOR = 90, /* "one_minus_src_color"  */
  YYSYMBOL_TFX_TOKEN_DST_COLOR = 91,       /* "dst_color"  */
  YYSYMBOL_TFX_TOKEN_ONE_MINUS_DST_COLOR = 92, /* "one_minus_dst_color"  */
  YYSYMBOL_TFX_TOKEN_SRC_ALPHA = 93,       /* "src_alpha"  */
  YYSYMBOL_TFX_TOKEN_ONE_MINUS_SRC_ALPHA = 94, /* "one_minus_src_alpha"  */
  YYSYMBOL_TFX_TOKEN_DST_ALPHA = 95,       /* "dst_alpha"  */
  YYSYMBOL_TFX_TOKEN_ONE_MINUS_DST_ALPHA = 96, /* "one_minus_dst_alpha"  */
  YYSYMBOL_TFX_TOKEN_CONSTANT_COLOR = 97,  /* "constant_color"  */
  YYSYMBOL_TFX_TOKEN_ONE_MINUS_CONSTANT_COLOR = 98, /* "one_minus_constant_color"  */
  YYSYMBOL_TFX_TOKEN_CONSTANT_ALPHA = 99,  /* "constant_alpha"  */
  YYSYMBOL_TFX_TOKEN_ONE_MINUS_CONSTANT_ALPHA = 100, /* "one_minus_constant_alpha"  */
  YYSYMBOL_TFX_TOKEN_SRC_ALPHA_SATURATE = 101, /* "src_alpha_saturate"  */
  YYSYMBOL_TFX_TOKEN_SRC_ONE_COLOR = 102,  /* "src_one_color"  */
  YYSYMBOL_TFX_TOKEN_ONE_MINUS_SRC_ONE_COLOR = 103, /* "one_minus_src_one_color"  */
  YYSYMBOL_TFX_TOKEN_SRC_ONE_ALPHA = 104,  /* "src_one_alpha"  */
  YYSYMBOL_TFX_TOKEN_ONE_MINUS_SRC_ONE_ALPHA = 105, /* "one_minus_src_one_alpha"  */
  YYSYMBOL_TFX_TOKEN_ADD = 106,            /* "add"  */
  YYSYMBOL_TFX_TOKEN_SUBTRACT = 107,       /* "subtract"  */
  YYSYMBOL_TFX_TOKEN_REVERSE_SUBTRACT = 108, /* "reverse_subtract"  */
  YYSYMBOL_TFX_TOKEN_MIN = 109,            /* "min"  */
  YYSYMBOL_TFX_TOKEN_MAX = 110,            /* "max"  */
  YYSYMBOL_TFX_TOKEN_CLEAR = 111,          /* "clear"  */
  YYSYMBOL_TFX_TOKEN_AND = 112,            /* "and"  */
  YYSYMBOL_TFX_TOKEN_AND_REVERSE = 113,    /* "and_reverse"  */
  YYSYMBOL_TFX_TOKEN_COPY = 114,           /* "copy"  */
  YYSYMBOL_TFX_TOKEN_AND_INVERTED = 115,   /* "and_inverted"  */
  YYSYMBOL_TFX_TOKEN_NO_OP = 116,          /* "no_op"  */
  YYSYMBOL_TFX_TOKEN_XOR = 117,            /* "xor"  */
  YYSYMBOL_TFX_TOKEN_OR = 118,             /* "or"  */
  YYSYMBOL_TFX_TOKEN_NOR = 119,            /* "nor"  */
  YYSYMBOL_TFX_TOKEN_EQUIVALENT = 120,     /* "equivalent"  */
  YYSYMBOL_TFX_TOKEN_OR_REVERSE = 121,     /* "or_reverse"  */
  YYSYMBOL_TFX_TOKEN_COPY_INVERTED = 122,  /* "copy_inverted"  */
  YYSYMBOL_TFX_TOKEN_OR_INVERTED = 123,    /* "or_inverted"  */
  YYSYMBOL_TFX_TOKEN_NAND = 124,           /* "nand"  */
  YYSYMBOL_TFX_TOKEN_SET = 125,            /* "set"  */
  YYSYMBOL_TFX_TOKEN_TARGET_VS_6_0 = 126,  /* "vs_6_0"  */
  YYSYMBOL_TFX_TOKEN_TARGET_VS_6_1 = 127,  /* "vs_6_1"  */
  YYSYMBOL_TFX_TOKEN_TARGET_VS_6_2 = 128,  /* "vs_6_2"  */
  YYSYMBOL_TFX_TOKEN_TARGET_VS_6_3 = 129,  /* "vs_6_3"  */
  YYSYMBOL_TFX_TOKEN_TARGET_VS_6_4 = 130,  /* "vs_6_4"  */
  YYSYMBOL_TFX_TOKEN_TARGET_VS_6_5 = 131,  /* "vs_6_5"  */
  YYSYMBOL_TFX_TOKEN_TARGET_VS_6_6 = 132,  /* "vs_6_6"  */
  YYSYMBOL_TFX_TOKEN_TARGET_VS_6_7 = 133,  /* "vs_6_7"  */
  YYSYMBOL_TFX_TOKEN_TARGET_PS_6_0 = 134,  /* "ps_6_0"  */
  YYSYMBOL_TFX_TOKEN_TARGET_PS_6_1 = 135,  /* "ps_6_1"  */
  YYSYMBOL_TFX_TOKEN_TARGET_PS_6_2 = 136,  /* "ps_6_2"  */
  YYSYMBOL_TFX_TOKEN_TARGET_PS_6_3 = 137,  /* "ps_6_3"  */
  YYSYMBOL_TFX_TOKEN_TARGET_PS_6_4 = 138,  /* "ps_6_4"  */
  YYSYMBOL_TFX_TOKEN_TARGET_PS_6_5 = 139,  /* "ps_6_5"  */
  YYSYMBOL_TFX_TOKEN_TARGET_PS_6_6 = 140,  /* "ps_6_6"  */
  YYSYMBOL_TFX_TOKEN_TARGET_PS_6_7 = 141,  /* "ps_6_7"  */
  YYSYMBOL_TFX_TOKEN_FLOAT = 142,          /* "float"  */
  YYSYMBOL_TFX_TOKEN_FLOAT2 = 143,         /* "float2"  */
  YYSYMBOL_TFX_TOKEN_FLOAT3 = 144,         /* "float3"  */
  YYSYMBOL_TFX_TOKEN_FLOAT4 = 145,         /* "float4"  */
  YYSYMBOL_TFX_TOKEN_FLOAT4_u8 = 146,      /* "float4_u8"  */
  YYSYMBOL_TFX_TOKEN_FLOAT4X4 = 147,       /* "float4x4"  */
  YYSYMBOL_TFX_TOKEN_INT = 148,            /* "int"  */
  YYSYMBOL_TFX_TOKEN_INT2 = 149,           /* "int2"  */
  YYSYMBOL_TFX_TOKEN_INT3 = 150,           /* "int3"  */
  YYSYMBOL_TFX_TOKEN_INT4 = 151,           /* "int4"  */
  YYSYMBOL_TFX_TOKEN_TEXTURE2D = 152,      /* "Texture2D"  */
  YYSYMBOL_TFX_TOKEN_TEXTURE_CUBE = 153,   /* "TextureCube"  */
  YYSYMBOL_YYACCEPT = 154,                 /* $accept  */
  YYSYMBOL_MODULE_EXPRESSIONS = 155,       /* MODULE_EXPRESSIONS  */
  YYSYMBOL_MODULE_EXPRESSION = 156,        /* MODULE_EXPRESSION  */
  YYSYMBOL_TECHNIQUE_MACRO_DECLARATION = 157, /* TECHNIQUE_MACRO_DECLARATION  */
  YYSYMBOL_TECHNIQUE_DECLARATION = 158,    /* TECHNIQUE_DECLARATION  */
  YYSYMBOL_TECHNIQUE_EXP_LIST = 159,       /* TECHNIQUE_EXP_LIST  */
  YYSYMBOL_TECHNIQUE_EXP = 160,            /* TECHNIQUE_EXP  */
  YYSYMBOL_RENDER_STATE_EXP_LIST = 161,    /* RENDER_STATE_EXP_LIST  */
  YYSYMBOL_RENDER_STATE_EXP = 162,         /* RENDER_STATE_EXP  */
  YYSYMBOL_DEPTH_EXP_LIST = 163,           /* DEPTH_EXP_LIST  */
  YYSYMBOL_DEPTH_EXP = 164,                /* DEPTH_EXP  */
  YYSYMBOL_STENCIL_EXP_LIST = 165,         /* STENCIL_EXP_LIST  */
  YYSYMBOL_STENCIL_EXP = 166,              /* STENCIL_EXP  */
  YYSYMBOL_CULL_MODE = 167,                /* CULL_MODE  */
  YYSYMBOL_PRIMITIVE_TOPOLOGY = 168,       /* PRIMITIVE_TOPOLOGY  */
  YYSYMBOL_INPUT_BUFFER_LIST = 169,        /* INPUT_BUFFER_LIST  */
  YYSYMBOL_INPUT_BUFFER = 170,             /* INPUT_BUFFER  */
  YYSYMBOL_INPUT_ATTRIBUTE_LIST = 171,     /* INPUT_ATTRIBUTE_LIST  */
  YYSYMBOL_INPUT_ATTRIBUTE = 172,          /* INPUT_ATTRIBUTE  */
  YYSYMBOL_COMPARE_OP = 173,               /* COMPARE_OP  */
  YYSYMBOL_STENCIL_OP = 174,               /* STENCIL_OP  */
  YYSYMBOL_BLENDING_EXP_LIST = 175,        /* BLENDING_EXP_LIST  */
  YYSYMBOL_BLENDING_EXP = 176,             /* BLENDING_EXP  */
  YYSYMBOL_MRT_BLENDING_EXP_LIST = 177,    /* MRT_BLENDING_EXP_LIST  */
  YYSYMBOL_MRT_BLENDING_EXP = 178,         /* MRT_BLENDING_EXP  */
  YYSYMBOL_BLEND_FACTOR = 179,             /* BLEND_FACTOR  */
  YYSYMBOL_BLEND_OP = 180,                 /* BLEND_OP  */
  YYSYMBOL_LOGIC_OP = 181,                 /* LOGIC_OP  */
  YYSYMBOL_SCOPE_DECLARATION = 182,        /* SCOPE_DECLARATION  */
  YYSYMBOL_SCOPE_EXP_LIST = 183,           /* SCOPE_EXP_LIST  */
  YYSYMBOL_SCOPE_EXP = 184,                /* SCOPE_EXP  */
  YYSYMBOL_RESOURCE_RESERVE_EXP_LIST = 185, /* RESOURCE_RESERVE_EXP_LIST  */
  YYSYMBOL_RESOURCE_RESERVE_EXP = 186,     /* RESOURCE_RESERVE_EXP  */
  YYSYMBOL_ASSIGN_EXP = 187,               /* ASSIGN_EXP  */
  YYSYMBOL_RESOURCE_TYPE = 188,            /* RESOURCE_TYPE  */
  YYSYMBOL_ATTRIBUTE_TYPE = 189,           /* ATTRIBUTE_TYPE  */
  YYSYMBOL_FLOAT4_VALUE = 190,             /* FLOAT4_VALUE  */
  YYSYMBOL_INT_VALUE = 191,                /* INT_VALUE  */
  YYSYMBOL_BOOL_VALUE = 192,               /* BOOL_VALUE  */
  YYSYMBOL_TARGET_PROFILE = 193            /* TARGET_PROFILE  */
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
#define YYLAST   344

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  154
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  40
/* YYNRULES -- Number of rules.  */
#define YYNRULES  177
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  342

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   408


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
     145,   146,   147,   148,   149,   150,   151,   152,   153
};

#if SHDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   267,   267,   269,   274,   276,   278,   283,   290,   298,
     302,   308,   311,   314,   317,   320,   323,   329,   333,   339,
     342,   345,   348,   351,   354,   360,   364,   370,   373,   376,
     382,   386,   392,   395,   398,   401,   404,   407,   413,   416,
     419,   424,   427,   430,   433,   436,   439,   442,   445,   448,
     451,   454,   460,   464,   471,   477,   481,   488,   494,   497,
     500,   503,   506,   509,   512,   515,   521,   524,   527,   530,
     533,   536,   539,   542,   548,   552,   558,   561,   564,   567,
     574,   578,   585,   588,   591,   594,   597,   600,   603,   609,
     612,   615,   618,   621,   624,   627,   630,   633,   636,   639,
     642,   645,   648,   651,   654,   657,   660,   663,   669,   672,
     675,   678,   681,   687,   690,   693,   696,   699,   702,   705,
     708,   711,   714,   717,   720,   723,   726,   729,   732,   738,
     746,   750,   757,   760,   763,   769,   773,   779,   782,   785,
     788,   791,   794,   797,   803,   806,   812,   815,   818,   824,
     827,   830,   833,   836,   839,   842,   845,   848,   851,   857,
     863,   869,   874,   877,   880,   883,   886,   889,   892,   895,
     898,   901,   904,   907,   910,   913,   916,   919
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
  "TFX_TOKEN_HLSL_CODE", "@", ":", ";", "=", "(", ")", "{", "}", ",", "-",
  "input", "buffer", "scope", "reserve", "register", "texture", "sampler",
  "cbuffer", "extern", "channel", "descriptor_set", "technique",
  "technique_macro", "support", "activate", "compile", "render_state",
  "cull_mode", "none", "ccw", "cw", "primitive_topology", "point_list",
  "line_list", "line_strip", "triangle_list", "triangle_fan",
  "triangle_strip", "line_list_with_adjacency",
  "line_strip_with_adjacency", "triangle_list_with_adjacency",
  "triangle_strip_with_adjacency", "patch_list", "depth", "stencil",
  "test", "write", "fail_op", "pass_op", "depth_fail_op", "compare_op",
  "ref_val", "never", "less", "equal", "less_or_equal", "greater",
  "not_equal", "greater_or_equal", "always", "keep", "zero", "replace",
  "inc_and_clamp", "dec_and_clamp", "invert", "inc_and_wrap",
  "dec_and_wrap", "blending", "has_blending", "logic_op_enabled",
  "logic_op", "blend_constants", "mrt", "src_color_blend_factor",
  "dst_color_blend_factor", "color_blend_op", "src_alpha_blend_factor",
  "dst_alpha_blend_factor", "alpha_blend_op", "one", "src_color",
  "one_minus_src_color", "dst_color", "one_minus_dst_color", "src_alpha",
  "one_minus_src_alpha", "dst_alpha", "one_minus_dst_alpha",
  "constant_color", "one_minus_constant_color", "constant_alpha",
  "one_minus_constant_alpha", "src_alpha_saturate", "src_one_color",
  "one_minus_src_one_color", "src_one_alpha", "one_minus_src_one_alpha",
  "add", "subtract", "reverse_subtract", "min", "max", "clear", "and",
  "and_reverse", "copy", "and_inverted", "no_op", "xor", "or", "nor",
  "equivalent", "or_reverse", "copy_inverted", "or_inverted", "nand",
  "set", "vs_6_0", "vs_6_1", "vs_6_2", "vs_6_3", "vs_6_4", "vs_6_5",
  "vs_6_6", "vs_6_7", "ps_6_0", "ps_6_1", "ps_6_2", "ps_6_3", "ps_6_4",
  "ps_6_5", "ps_6_6", "ps_6_7", "float", "float2", "float3", "float4",
  "float4_u8", "float4x4", "int", "int2", "int3", "int4", "Texture2D",
  "TextureCube", "$accept", "MODULE_EXPRESSIONS", "MODULE_EXPRESSION",
  "TECHNIQUE_MACRO_DECLARATION", "TECHNIQUE_DECLARATION",
  "TECHNIQUE_EXP_LIST", "TECHNIQUE_EXP", "RENDER_STATE_EXP_LIST",
  "RENDER_STATE_EXP", "DEPTH_EXP_LIST", "DEPTH_EXP", "STENCIL_EXP_LIST",
  "STENCIL_EXP", "CULL_MODE", "PRIMITIVE_TOPOLOGY", "INPUT_BUFFER_LIST",
  "INPUT_BUFFER", "INPUT_ATTRIBUTE_LIST", "INPUT_ATTRIBUTE", "COMPARE_OP",
  "STENCIL_OP", "BLENDING_EXP_LIST", "BLENDING_EXP",
  "MRT_BLENDING_EXP_LIST", "MRT_BLENDING_EXP", "BLEND_FACTOR", "BLEND_OP",
  "LOGIC_OP", "SCOPE_DECLARATION", "SCOPE_EXP_LIST", "SCOPE_EXP",
  "RESOURCE_RESERVE_EXP_LIST", "RESOURCE_RESERVE_EXP", "ASSIGN_EXP",
  "RESOURCE_TYPE", "ATTRIBUTE_TYPE", "FLOAT4_VALUE", "INT_VALUE",
  "BOOL_VALUE", "TARGET_PROFILE", YY_NULLPTR
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
     405,   406,   407,   408
};
#endif

#define YYPACT_NINF (-258)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      -3,     5,    15,    17,    59,    -3,  -258,  -258,  -258,    47,
      74,    76,  -258,  -258,   -21,    24,    24,   137,  -258,  -258,
    -258,  -258,  -258,  -258,  -258,  -258,  -258,  -258,  -258,  -258,
    -258,   149,   -21,   179,   185,   180,  -258,   171,   173,   182,
     186,   181,    24,   183,    -7,   187,  -258,   188,   189,   190,
     196,   197,   -34,   -16,   194,  -258,   195,   198,   199,  -258,
     202,  -258,   -13,   -13,   203,   204,   205,  -258,  -258,  -258,
    -258,  -258,  -258,  -258,  -258,  -258,  -258,  -258,  -258,  -258,
    -258,  -258,  -258,   191,   200,   206,   207,   210,   211,   212,
    -258,   -16,  -258,  -258,     9,   218,    -7,   213,   214,   217,
     219,  -258,  -258,  -258,   201,   209,   136,    93,   117,   110,
      39,  -258,   192,   208,   215,  -258,   220,  -258,   225,   227,
    -258,  -258,   221,   223,  -258,   209,  -258,  -258,  -258,   226,
    -258,  -258,  -258,  -258,  -258,  -258,  -258,  -258,  -258,  -258,
    -258,   228,   229,   230,   231,  -258,   117,   232,   233,   234,
     235,   236,   237,  -258,   110,   238,   239,   240,   241,  -258,
      39,   218,   218,   218,  -258,   224,   242,   244,   218,  -258,
    -258,  -258,   246,   246,    48,  -258,   246,    87,    87,    87,
      48,   218,  -258,   246,   -49,    71,   218,  -258,   168,   169,
     170,  -258,  -258,  -258,   243,  -258,   247,   248,  -258,  -258,
    -258,  -258,  -258,  -258,  -258,  -258,   249,   250,  -258,  -258,
    -258,  -258,  -258,  -258,  -258,  -258,   251,   252,   253,   254,
     255,   256,  -258,  -258,  -258,  -258,  -258,  -258,  -258,  -258,
    -258,  -258,  -258,  -258,  -258,  -258,  -258,  -258,   257,   258,
     259,   260,  -258,   218,  -258,   218,  -258,   218,   262,  -258,
    -258,  -258,  -258,  -258,  -258,  -258,  -258,  -258,  -258,  -258,
     265,  -258,   263,   261,   264,   266,   -65,   267,    67,  -258,
    -258,  -258,  -258,   -65,   270,   273,   269,   271,   274,   275,
     276,   277,   278,  -258,    67,  -258,   272,   268,   246,   -51,
     -51,    70,   -51,   -51,    70,  -258,   285,   288,   282,  -258,
    -258,  -258,  -258,  -258,  -258,  -258,  -258,  -258,  -258,  -258,
    -258,  -258,  -258,  -258,  -258,  -258,  -258,  -258,   283,   284,
    -258,  -258,  -258,  -258,  -258,   286,   287,   289,   290,   291,
     279,  -258,  -258,  -258,  -258,  -258,  -258,  -258,  -258,   295,
     292,  -258
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     3,     6,     5,     4,     0,
       0,     0,     1,     2,     0,     0,     0,     0,   148,   153,
     154,   155,   156,   157,   158,   149,   150,   151,   152,   146,
     147,     0,   131,     0,     0,     0,    14,     0,     0,     0,
       0,     0,    10,     0,     0,     0,   130,     0,     0,     0,
       0,     0,     0,     0,     0,     9,     0,     0,     0,   132,
       0,   129,     0,     0,     0,     0,     0,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,     0,     0,     0,     0,     0,     0,     0,
      15,    18,     8,     7,     0,     0,   136,     0,     0,     0,
       0,    16,    11,    12,     0,     0,     0,     0,     0,     0,
       0,    17,     0,     0,     0,   160,     0,   135,     0,     0,
     133,   134,     0,     0,    21,    53,    38,    40,    39,     0,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,     0,     0,     0,     0,    22,    26,     0,     0,     0,
       0,     0,     0,    23,    31,     0,     0,     0,     0,    24,
      75,     0,     0,     0,   143,     0,     0,     0,     0,    52,
      19,    20,     0,     0,     0,    25,     0,     0,     0,     0,
       0,     0,    30,     0,     0,     0,     0,    74,     0,     0,
       0,   144,   145,    13,     0,   161,     0,     0,    58,    59,
      60,    61,    62,    63,    64,    65,     0,     0,    66,    67,
      68,    69,    70,    71,    72,    73,     0,     0,     0,     0,
       0,     0,   123,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   124,   125,   126,   127,   128,     0,     0,
       0,     0,   142,     0,   138,     0,   140,     0,     0,    27,
      28,    29,    32,    33,    34,    35,    36,    37,    76,    77,
       0,    78,     0,     0,     0,     0,     0,     0,     0,   141,
     137,   139,    54,    56,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    79,    81,    55,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    80,     0,     0,     0,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,     0,     0,
     108,   109,   110,   111,   112,     0,     0,     0,     0,     0,
       0,    82,    83,    84,    85,    86,    87,    88,    57,     0,
       0,   159
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -258,   297,  -258,  -258,  -258,    -8,  -258,   132,  -258,   157,
    -258,    85,  -258,  -258,  -258,   153,  -258,    31,  -258,   126,
      10,   147,  -258,    25,  -258,  -145,    14,  -258,  -258,   280,
    -258,   222,  -258,   281,  -258,  -257,  -258,  -156,  -172,  -258
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     4,     5,     6,     7,    41,    42,    90,    91,   145,
     146,   153,   154,   129,   141,   124,   125,   272,   273,   206,
     216,   159,   160,   283,   284,   318,   325,   238,     8,    31,
      32,    59,    60,    99,    33,    34,   240,   116,   196,    83
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      17,   197,    84,    18,   207,   188,   189,   190,    43,   274,
       9,   221,   194,    97,    98,    57,   274,     1,   299,    85,
      10,    58,    11,    86,   222,   220,     2,     3,   112,    35,
     241,    36,   113,   114,    55,    87,    88,   300,   301,   302,
     303,   304,   305,   306,   307,   308,   309,   310,   311,   312,
     313,   314,   315,   316,   317,    37,    38,    39,    40,    12,
      89,    14,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,   263,    15,   264,
      16,   265,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,   198,   199,
     200,   201,   202,   203,   204,   205,   298,   155,   156,   157,
     158,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   276,   319,    44,   326,   327,   277,
     278,   279,   280,   281,   282,   208,   209,   210,   211,   212,
     213,   214,   215,   147,    45,   148,   149,   150,   151,   152,
     142,   143,   126,   127,   128,   144,   320,   321,   322,   323,
     324,   242,   244,   246,    47,   243,   245,   247,   217,   218,
      48,    50,    49,    51,    52,    53,    54,    61,    56,    62,
      63,    65,    66,    64,    92,    93,   122,   104,   161,   105,
      94,    95,    96,   101,   102,   103,   239,   106,   107,   108,
     109,   110,   115,   111,   162,   118,   119,   120,   123,   121,
     165,   163,   166,   164,   167,   168,   170,   191,   171,   182,
     172,   173,   174,   176,   177,   178,   179,   180,   181,   183,
     184,   185,   195,   186,   193,   192,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   267,   261,
     260,   266,   268,   262,   269,   286,   287,   270,   169,   271,
     288,   296,   289,   275,   297,   290,   291,   292,   293,   294,
     329,   330,   331,   332,   333,   339,   334,   335,   340,   336,
     337,   338,    13,   175,   285,   341,   219,   187,   328,   295,
       0,     0,    46,     0,     0,     0,     0,     0,   117,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   100
};

static const yytype_int16 yycheck[] =
{
      21,   173,    18,    24,   176,   161,   162,   163,    16,   266,
       5,   183,   168,    26,    27,    22,   273,    20,    69,    35,
       5,    28,     5,    39,    73,   181,    29,    30,    19,     5,
     186,     7,    23,    24,    42,    51,    52,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,    31,    32,    33,    34,     0,
      76,    14,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   243,    14,   245,
      14,   247,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,    60,    61,
      62,    63,    64,    65,    66,    67,   288,    78,    79,    80,
      81,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    77,   290,     9,   292,   293,    82,
      83,    84,    85,    86,    87,    68,    69,    70,    71,    72,
      73,    74,    75,    53,    15,    55,    56,    57,    58,    59,
      53,    54,    36,    37,    38,    58,   106,   107,   108,   109,
     110,    13,    13,    13,     5,    17,    17,    17,   178,   179,
       5,    20,    12,    20,    12,     9,    15,    10,    15,    11,
      11,     5,     5,    13,    10,    10,     5,    16,    16,     9,
      12,    12,    10,    10,    10,    10,   145,    11,    11,     9,
       9,     9,     4,    91,    16,    12,    12,    10,    19,    10,
       5,    16,     5,    13,    13,    12,    10,    13,    10,   154,
      11,    11,    11,    11,    11,    11,    11,    11,    11,    11,
      11,    11,     6,    12,    10,    13,    13,    10,    10,    10,
      10,    10,    10,    10,    10,    10,    10,    10,     3,    10,
      12,     9,     9,    13,    13,     5,     3,    13,   125,    13,
      11,     9,    11,    16,    16,    11,    11,    11,    11,    11,
       5,     3,    10,    10,    10,    16,    10,    10,     3,    10,
      10,    10,     5,   146,   273,    13,   180,   160,   294,   284,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    96,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    20,    29,    30,   155,   156,   157,   158,   182,     5,
       5,     5,     0,   155,    14,    14,    14,    21,    24,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   183,   184,   188,   189,     5,     7,    31,    32,    33,
      34,   159,   160,   159,     9,    15,   183,     5,     5,    12,
      20,    20,    12,     9,    15,   159,    15,    22,    28,   185,
     186,    10,    11,    11,    13,     5,     5,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   193,    18,    35,    39,    51,    52,    76,
     161,   162,    10,    10,    12,    12,    10,    26,    27,   187,
     187,    10,    10,    10,    16,     9,    11,    11,     9,     9,
       9,   161,    19,    23,    24,     4,   191,   185,    12,    12,
      10,    10,     5,    19,   169,   170,    36,    37,    38,   167,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,   168,    53,    54,    58,   163,   164,    53,    55,    56,
      57,    58,    59,   165,   166,    78,    79,    80,    81,   175,
     176,    16,    16,    16,    13,     5,     5,    13,    12,   169,
      10,    10,    11,    11,    11,   163,    11,    11,    11,    11,
      11,    11,   165,    11,    11,    11,    12,   175,   191,   191,
     191,    13,    13,    10,   191,     6,   192,   192,    60,    61,
      62,    63,    64,    65,    66,    67,   173,   192,    68,    69,
      70,    71,    72,    73,    74,    75,   174,   174,   174,   173,
     191,   192,    73,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   181,   145,
     190,   191,    13,    17,    13,    17,    13,    17,    13,    10,
      10,    10,    10,    10,    10,    10,    10,    10,    10,    10,
      12,    10,    13,   191,   191,   191,     9,     3,     9,    13,
      13,    13,   171,   172,   189,    16,    77,    82,    83,    84,
      85,    86,    87,   177,   178,   171,     5,     3,    11,    11,
      11,    11,    11,    11,    11,   177,     9,    16,   192,    69,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   179,   179,
     106,   107,   108,   109,   110,   180,   179,   179,   180,     5,
       3,    10,    10,    10,    10,    10,    10,    10,    10,    16,
       3,    13
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   154,   155,   155,   156,   156,   156,   157,   158,   159,
     159,   160,   160,   160,   160,   160,   160,   161,   161,   162,
     162,   162,   162,   162,   162,   163,   163,   164,   164,   164,
     165,   165,   166,   166,   166,   166,   166,   166,   167,   167,
     167,   168,   168,   168,   168,   168,   168,   168,   168,   168,
     168,   168,   169,   169,   170,   171,   171,   172,   173,   173,
     173,   173,   173,   173,   173,   173,   174,   174,   174,   174,
     174,   174,   174,   174,   175,   175,   176,   176,   176,   176,
     177,   177,   178,   178,   178,   178,   178,   178,   178,   179,
     179,   179,   179,   179,   179,   179,   179,   179,   179,   179,
     179,   179,   179,   179,   179,   179,   179,   179,   180,   180,
     180,   180,   180,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   181,   181,   181,   181,   181,   182,
     183,   183,   184,   184,   184,   185,   185,   186,   186,   186,
     186,   186,   186,   186,   187,   187,   188,   188,   188,   189,
     189,   189,   189,   189,   189,   189,   189,   189,   189,   190,
     191,   192,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     1,     6,     6,     2,
       1,     4,     4,     7,     1,     3,     4,     2,     1,     4,
       4,     3,     3,     3,     3,     2,     1,     4,     4,     4,
       2,     1,     4,     4,     4,     4,     4,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     1,     6,     2,     1,     5,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     1,     4,     4,     4,     6,
       2,     1,     4,     4,     4,     4,     4,     4,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     6,
       2,     1,     3,     5,     5,     3,     2,     8,     6,     8,
       6,     8,     6,     4,     4,     4,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,    10,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1
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
#line 267 "src/shaders_compiler/parser/parser.y"
                                         {
  }
#line 1781 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 3: /* MODULE_EXPRESSIONS: MODULE_EXPRESSION  */
#line 269 "src/shaders_compiler/parser/parser.y"
                      {
  }
#line 1788 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 4: /* MODULE_EXPRESSION: SCOPE_DECLARATION  */
#line 274 "src/shaders_compiler/parser/parser.y"
                      {
  }
#line 1795 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 5: /* MODULE_EXPRESSION: TECHNIQUE_DECLARATION  */
#line 276 "src/shaders_compiler/parser/parser.y"
                          {
  }
#line 1802 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 6: /* MODULE_EXPRESSION: TECHNIQUE_MACRO_DECLARATION  */
#line 278 "src/shaders_compiler/parser/parser.y"
                                {
  }
#line 1809 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 7: /* TECHNIQUE_MACRO_DECLARATION: "technique_macro" TFX_TOKEN_NAME_VAL "{" TECHNIQUE_EXP_LIST "}" ";"  */
#line 283 "src/shaders_compiler/parser/parser.y"
                                                                                    {
    const bool isOk = compiler.onTechniqueMacroDeclaration(new TechniqueMacroDeclarationExp{(yyvsp[-4].sval), (yyvsp[-2].techniqueExp)});
    if (!isOk)
      YYABORT;
}
#line 1819 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 8: /* TECHNIQUE_DECLARATION: "technique" TFX_TOKEN_NAME_VAL "{" TECHNIQUE_EXP_LIST "}" ";"  */
#line 290 "src/shaders_compiler/parser/parser.y"
                                                                              {
    const bool isOk = compiler.onTechniqueDeclaration(new TechniqueDeclarationExp{(yyvsp[-4].sval), (yyvsp[-2].techniqueExp)});
    if (!isOk)
      YYABORT;
  }
#line 1829 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 9: /* TECHNIQUE_EXP_LIST: TECHNIQUE_EXP TECHNIQUE_EXP_LIST  */
#line 298 "src/shaders_compiler/parser/parser.y"
                                                {
    (yyval.techniqueExp) = (yyvsp[-1].techniqueExp);
    (yyval.techniqueExp)->next = (yyvsp[0].techniqueExp);
  }
#line 1838 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 10: /* TECHNIQUE_EXP_LIST: TECHNIQUE_EXP  */
#line 302 "src/shaders_compiler/parser/parser.y"
                       {
    (yyval.techniqueExp) = (yyvsp[0].techniqueExp);
  }
#line 1846 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 11: /* TECHNIQUE_EXP: "support" "scope" TFX_TOKEN_NAME_VAL ";"  */
#line 308 "src/shaders_compiler/parser/parser.y"
                                                   {
    (yyval.techniqueExp) = new ScopeSupportExp((yyvsp[-1].sval));
  }
#line 1854 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 12: /* TECHNIQUE_EXP: "activate" "scope" TFX_TOKEN_NAME_VAL ";"  */
#line 311 "src/shaders_compiler/parser/parser.y"
                                                    {
    (yyval.techniqueExp) = new ScopeActivateExp((yyvsp[-1].sval));
  }
#line 1862 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 13: /* TECHNIQUE_EXP: "compile" "(" TARGET_PROFILE "," TFX_TOKEN_NAME_VAL ")" ";"  */
#line 314 "src/shaders_compiler/parser/parser.y"
                                                                               {
    (yyval.techniqueExp) = new CompileExp((yyvsp[-4].targetProfile), (yyvsp[-2].sval));
  }
#line 1870 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 14: /* TECHNIQUE_EXP: TFX_TOKEN_HLSL_CODE  */
#line 317 "src/shaders_compiler/parser/parser.y"
                              {
    (yyval.techniqueExp) = new HlslExp((yyvsp[0].sval));
  }
#line 1878 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 15: /* TECHNIQUE_EXP: "render_state" ":" RENDER_STATE_EXP_LIST  */
#line 320 "src/shaders_compiler/parser/parser.y"
                                                 {
    (yyval.techniqueExp) = (yyvsp[0].renderStateExp);
  }
#line 1886 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 16: /* TECHNIQUE_EXP: TFX_TOKEN_NAME_VAL "(" ")" ";"  */
#line 323 "src/shaders_compiler/parser/parser.y"
                                         {
    (yyval.techniqueExp) = new TechniqueMacroInvokeExp((yyvsp[-3].sval));
  }
#line 1894 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 17: /* RENDER_STATE_EXP_LIST: RENDER_STATE_EXP RENDER_STATE_EXP_LIST  */
#line 329 "src/shaders_compiler/parser/parser.y"
                                                        {
    (yyval.renderStateExp) = (yyvsp[-1].renderStateExp);
    (yyval.renderStateExp)->next = (yyvsp[0].renderStateExp);
  }
#line 1903 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 18: /* RENDER_STATE_EXP_LIST: RENDER_STATE_EXP  */
#line 333 "src/shaders_compiler/parser/parser.y"
                            {
    (yyval.renderStateExp) = (yyvsp[0].renderStateExp);
  }
#line 1911 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 19: /* RENDER_STATE_EXP: "cull_mode" "=" CULL_MODE ";"  */
#line 339 "src/shaders_compiler/parser/parser.y"
                                     {
    (yyval.renderStateExp) = new CullModeExp((yyvsp[-1].cullMode));
  }
#line 1919 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 20: /* RENDER_STATE_EXP: "primitive_topology" "=" PRIMITIVE_TOPOLOGY ";"  */
#line 342 "src/shaders_compiler/parser/parser.y"
                                                      {
    (yyval.renderStateExp) = new PrimitiveTopologyExp((yyvsp[-1].primitiveTopology));
  }
#line 1927 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 21: /* RENDER_STATE_EXP: "input" ":" INPUT_BUFFER_LIST  */
#line 345 "src/shaders_compiler/parser/parser.y"
                                           {
    (yyval.renderStateExp) = new InputExp((yyvsp[0].inputBufferExp));
  }
#line 1935 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 22: /* RENDER_STATE_EXP: "depth" ":" DEPTH_EXP_LIST  */
#line 348 "src/shaders_compiler/parser/parser.y"
                                     {
    (yyval.renderStateExp) = (yyvsp[0].renderStateExp);
  }
#line 1943 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 23: /* RENDER_STATE_EXP: "stencil" ":" STENCIL_EXP_LIST  */
#line 351 "src/shaders_compiler/parser/parser.y"
                                         {
    (yyval.renderStateExp) = (yyvsp[0].renderStateExp);
  }
#line 1951 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 24: /* RENDER_STATE_EXP: "blending" ":" BLENDING_EXP_LIST  */
#line 354 "src/shaders_compiler/parser/parser.y"
                                           {
    (yyval.renderStateExp) = (yyvsp[0].blendingExp);
  }
#line 1959 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 25: /* DEPTH_EXP_LIST: DEPTH_EXP DEPTH_EXP_LIST  */
#line 360 "src/shaders_compiler/parser/parser.y"
                                        {
    (yyval.renderStateExp) = (yyvsp[-1].renderStateExp);
    (yyval.renderStateExp)->next = (yyvsp[0].renderStateExp);
  }
#line 1968 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 26: /* DEPTH_EXP_LIST: DEPTH_EXP  */
#line 364 "src/shaders_compiler/parser/parser.y"
                   {
    (yyval.renderStateExp) = (yyvsp[0].renderStateExp);
  }
#line 1976 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 27: /* DEPTH_EXP: "test" "=" BOOL_VALUE ";"  */
#line 370 "src/shaders_compiler/parser/parser.y"
                                 {
    (yyval.renderStateExp) = new DepthTestExp((yyvsp[-1].bval));
  }
#line 1984 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 28: /* DEPTH_EXP: "write" "=" BOOL_VALUE ";"  */
#line 373 "src/shaders_compiler/parser/parser.y"
                                  {
    (yyval.renderStateExp) = new DepthWriteExp((yyvsp[-1].bval));
  }
#line 1992 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 29: /* DEPTH_EXP: "compare_op" "=" COMPARE_OP ";"  */
#line 376 "src/shaders_compiler/parser/parser.y"
                                        {
    (yyval.renderStateExp) = new DepthOpExp((yyvsp[-1].compareOp));
  }
#line 2000 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 30: /* STENCIL_EXP_LIST: STENCIL_EXP STENCIL_EXP_LIST  */
#line 382 "src/shaders_compiler/parser/parser.y"
                                            {
    (yyval.renderStateExp) = (yyvsp[-1].renderStateExp);
    (yyval.renderStateExp)->next = (yyvsp[0].renderStateExp);
  }
#line 2009 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 31: /* STENCIL_EXP_LIST: STENCIL_EXP  */
#line 386 "src/shaders_compiler/parser/parser.y"
                     {
    (yyval.renderStateExp) = (yyvsp[0].renderStateExp);
  }
#line 2017 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 32: /* STENCIL_EXP: "test" "=" BOOL_VALUE ";"  */
#line 392 "src/shaders_compiler/parser/parser.y"
                                 {
    (yyval.renderStateExp) = new StencilTestExp((yyvsp[-1].bval));
  }
#line 2025 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 33: /* STENCIL_EXP: "fail_op" "=" STENCIL_OP ";"  */
#line 395 "src/shaders_compiler/parser/parser.y"
                                     {
    (yyval.renderStateExp) = new StencilFailOpExp((yyvsp[-1].stencilOp));
  }
#line 2033 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 34: /* STENCIL_EXP: "pass_op" "=" STENCIL_OP ";"  */
#line 398 "src/shaders_compiler/parser/parser.y"
                                     {
    (yyval.renderStateExp) = new StencilPassOpExp((yyvsp[-1].stencilOp));
  }
#line 2041 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 35: /* STENCIL_EXP: "depth_fail_op" "=" STENCIL_OP ";"  */
#line 401 "src/shaders_compiler/parser/parser.y"
                                           {
    (yyval.renderStateExp) = new StencilDepthFailOpExp((yyvsp[-1].stencilOp));
  }
#line 2049 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 36: /* STENCIL_EXP: "compare_op" "=" COMPARE_OP ";"  */
#line 404 "src/shaders_compiler/parser/parser.y"
                                        {
    (yyval.renderStateExp) = new StencilCompareOpExp((yyvsp[-1].compareOp));
  }
#line 2057 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 37: /* STENCIL_EXP: "ref_val" "=" INT_VALUE ";"  */
#line 407 "src/shaders_compiler/parser/parser.y"
                                   {
    (yyval.renderStateExp) = new StencilReferenceValueExp((yyvsp[-1].ival));
  }
#line 2065 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 38: /* CULL_MODE: "none"  */
#line 413 "src/shaders_compiler/parser/parser.y"
           {
    (yyval.cullMode) = gapi::CullMode::None;
  }
#line 2073 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 39: /* CULL_MODE: "cw"  */
#line 416 "src/shaders_compiler/parser/parser.y"
         {
    (yyval.cullMode) = gapi::CullMode::CW;
  }
#line 2081 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 40: /* CULL_MODE: "ccw"  */
#line 419 "src/shaders_compiler/parser/parser.y"
          {
    (yyval.cullMode) = gapi::CullMode::CCW;
  }
#line 2089 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 41: /* PRIMITIVE_TOPOLOGY: "point_list"  */
#line 424 "src/shaders_compiler/parser/parser.y"
                 {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::PointList;
  }
#line 2097 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 42: /* PRIMITIVE_TOPOLOGY: "line_list"  */
#line 427 "src/shaders_compiler/parser/parser.y"
                {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::LineList;
  }
#line 2105 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 43: /* PRIMITIVE_TOPOLOGY: "line_strip"  */
#line 430 "src/shaders_compiler/parser/parser.y"
                 {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::LineStrip;
  }
#line 2113 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 44: /* PRIMITIVE_TOPOLOGY: "triangle_list"  */
#line 433 "src/shaders_compiler/parser/parser.y"
                    {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::TriangleList;
  }
#line 2121 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 45: /* PRIMITIVE_TOPOLOGY: "triangle_fan"  */
#line 436 "src/shaders_compiler/parser/parser.y"
                   {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::TriangleFan;
  }
#line 2129 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 46: /* PRIMITIVE_TOPOLOGY: "triangle_strip"  */
#line 439 "src/shaders_compiler/parser/parser.y"
                     {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::TriangleStrip;
  }
#line 2137 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 47: /* PRIMITIVE_TOPOLOGY: "line_list_with_adjacency"  */
#line 442 "src/shaders_compiler/parser/parser.y"
                               {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::LineListWithAdjacency;
  }
#line 2145 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 48: /* PRIMITIVE_TOPOLOGY: "line_strip_with_adjacency"  */
#line 445 "src/shaders_compiler/parser/parser.y"
                                {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::LineStripWithAdjacency;
  }
#line 2153 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 49: /* PRIMITIVE_TOPOLOGY: "triangle_list_with_adjacency"  */
#line 448 "src/shaders_compiler/parser/parser.y"
                                   {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::TriangleListWithAdjacency;
  }
#line 2161 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 50: /* PRIMITIVE_TOPOLOGY: "triangle_strip_with_adjacency"  */
#line 451 "src/shaders_compiler/parser/parser.y"
                                    {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::TriangleStripWithAdjacency;
  }
#line 2169 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 51: /* PRIMITIVE_TOPOLOGY: "patch_list"  */
#line 454 "src/shaders_compiler/parser/parser.y"
                 {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::PatchList;
  }
#line 2177 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 52: /* INPUT_BUFFER_LIST: INPUT_BUFFER INPUT_BUFFER_LIST  */
#line 460 "src/shaders_compiler/parser/parser.y"
                                                 {
    (yyval.inputBufferExp) = (yyvsp[-1].inputBufferExp);
    (yyval.inputBufferExp)->next = (yyvsp[0].inputBufferExp);
  }
#line 2186 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 53: /* INPUT_BUFFER_LIST: INPUT_BUFFER  */
#line 464 "src/shaders_compiler/parser/parser.y"
                         {
    (yyval.inputBufferExp) = (yyvsp[0].inputBufferExp);
    (yyval.inputBufferExp)->next = nullptr;
  }
#line 2195 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 54: /* INPUT_BUFFER: "buffer" "(" INT_VALUE ")" ":" INPUT_ATTRIBUTE_LIST  */
#line 471 "src/shaders_compiler/parser/parser.y"
                                                                    {
    (yyval.inputBufferExp) = new InputBufferExp((yyvsp[-3].ival), (yyvsp[0].inputAttributeExp));
  }
#line 2203 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 55: /* INPUT_ATTRIBUTE_LIST: INPUT_ATTRIBUTE INPUT_ATTRIBUTE_LIST  */
#line 477 "src/shaders_compiler/parser/parser.y"
                                                     {
    (yyval.inputAttributeExp) = (yyvsp[-1].inputAttributeExp);
    (yyval.inputAttributeExp)->next = (yyvsp[0].inputAttributeExp);
  }
#line 2212 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 56: /* INPUT_ATTRIBUTE_LIST: INPUT_ATTRIBUTE  */
#line 481 "src/shaders_compiler/parser/parser.y"
                          {
    (yyval.inputAttributeExp) = (yyvsp[0].inputAttributeExp);
    (yyval.inputAttributeExp)->next = nullptr;
  }
#line 2221 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 57: /* INPUT_ATTRIBUTE: ATTRIBUTE_TYPE TFX_TOKEN_NAME_VAL ":" TFX_TOKEN_NAME_VAL ";"  */
#line 488 "src/shaders_compiler/parser/parser.y"
                                                                                       {
    (yyval.inputAttributeExp) = new InputAttributeExp((yyvsp[-4].attributeType), (yyvsp[-3].sval), (yyvsp[-1].sval));
  }
#line 2229 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 58: /* COMPARE_OP: "never"  */
#line 494 "src/shaders_compiler/parser/parser.y"
            {
   (yyval.compareOp) = gapi::CompareOp::Never;
  }
#line 2237 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 59: /* COMPARE_OP: "less"  */
#line 497 "src/shaders_compiler/parser/parser.y"
           {
   (yyval.compareOp) = gapi::CompareOp::Less;
  }
#line 2245 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 60: /* COMPARE_OP: "equal"  */
#line 500 "src/shaders_compiler/parser/parser.y"
            {
   (yyval.compareOp) = gapi::CompareOp::Equal;
  }
#line 2253 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 61: /* COMPARE_OP: "less_or_equal"  */
#line 503 "src/shaders_compiler/parser/parser.y"
                    {
   (yyval.compareOp) = gapi::CompareOp::LessOrEqual;
  }
#line 2261 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 62: /* COMPARE_OP: "greater"  */
#line 506 "src/shaders_compiler/parser/parser.y"
              {
   (yyval.compareOp) = gapi::CompareOp::Greater;
  }
#line 2269 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 63: /* COMPARE_OP: "not_equal"  */
#line 509 "src/shaders_compiler/parser/parser.y"
                {
   (yyval.compareOp) = gapi::CompareOp::NotEqual;
  }
#line 2277 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 64: /* COMPARE_OP: "greater_or_equal"  */
#line 512 "src/shaders_compiler/parser/parser.y"
                       {
   (yyval.compareOp) = gapi::CompareOp::GreaterOrEqual;
  }
#line 2285 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 65: /* COMPARE_OP: "always"  */
#line 515 "src/shaders_compiler/parser/parser.y"
             {
   (yyval.compareOp) = gapi::CompareOp::Always;
  }
#line 2293 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 66: /* STENCIL_OP: "keep"  */
#line 521 "src/shaders_compiler/parser/parser.y"
           {
    (yyval.stencilOp) = gapi::StencilOp::Keep;
  }
#line 2301 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 67: /* STENCIL_OP: "zero"  */
#line 524 "src/shaders_compiler/parser/parser.y"
           {
    (yyval.stencilOp) = gapi::StencilOp::Zero;
  }
#line 2309 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 68: /* STENCIL_OP: "replace"  */
#line 527 "src/shaders_compiler/parser/parser.y"
              {
    (yyval.stencilOp) = gapi::StencilOp::Replace;
  }
#line 2317 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 69: /* STENCIL_OP: "inc_and_clamp"  */
#line 530 "src/shaders_compiler/parser/parser.y"
                    {
    (yyval.stencilOp) = gapi::StencilOp::IncrementAndClamp;
  }
#line 2325 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 70: /* STENCIL_OP: "dec_and_clamp"  */
#line 533 "src/shaders_compiler/parser/parser.y"
                    {
    (yyval.stencilOp) = gapi::StencilOp::DecrementAndClamp;
  }
#line 2333 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 71: /* STENCIL_OP: "invert"  */
#line 536 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.stencilOp) = gapi::StencilOp::Invert;
  }
#line 2341 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 72: /* STENCIL_OP: "inc_and_wrap"  */
#line 539 "src/shaders_compiler/parser/parser.y"
                   {
    (yyval.stencilOp) = gapi::StencilOp::IncrementAndWrap;
  }
#line 2349 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 73: /* STENCIL_OP: "dec_and_wrap"  */
#line 542 "src/shaders_compiler/parser/parser.y"
                   {
    (yyval.stencilOp) = gapi::StencilOp::DecrementAndWrap;
  }
#line 2357 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 74: /* BLENDING_EXP_LIST: BLENDING_EXP BLENDING_EXP_LIST  */
#line 548 "src/shaders_compiler/parser/parser.y"
                                              {
    (yyval.blendingExp) = (yyvsp[-1].blendingExp);
    (yyval.blendingExp)->next = (yyvsp[0].blendingExp);
  }
#line 2366 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 75: /* BLENDING_EXP_LIST: BLENDING_EXP  */
#line 552 "src/shaders_compiler/parser/parser.y"
                      {
    (yyval.blendingExp) = (yyvsp[0].blendingExp);
    (yyval.blendingExp)->next = nullptr;
  }
#line 2375 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 76: /* BLENDING_EXP: "logic_op_enabled" "=" BOOL_VALUE ";"  */
#line 558 "src/shaders_compiler/parser/parser.y"
                                             {
    (yyval.blendingExp) = new LogicOpEnablingExp((yyvsp[-1].bval));
  }
#line 2383 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 77: /* BLENDING_EXP: "logic_op" "=" LOGIC_OP ";"  */
#line 561 "src/shaders_compiler/parser/parser.y"
                                    {
    (yyval.blendingExp) = new LogicOpExp((yyvsp[-1].logicOp));
  }
#line 2391 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 78: /* BLENDING_EXP: "blend_constants" "=" FLOAT4_VALUE ";"  */
#line 564 "src/shaders_compiler/parser/parser.y"
                                              {
    (yyval.blendingExp) = new BlendConstants((yyvsp[-1].f4val));
  }
#line 2399 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 79: /* BLENDING_EXP: "mrt" "(" INT_VALUE ")" ":" MRT_BLENDING_EXP_LIST  */
#line 567 "src/shaders_compiler/parser/parser.y"
                                                               {
    (yyvsp[0].mrtBlendingExp)->n = (yyvsp[-3].ival);
    (yyval.blendingExp) = (yyvsp[0].mrtBlendingExp);
  }
#line 2408 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 80: /* MRT_BLENDING_EXP_LIST: MRT_BLENDING_EXP MRT_BLENDING_EXP_LIST  */
#line 574 "src/shaders_compiler/parser/parser.y"
                                                     {
    (yyval.mrtBlendingExp) = (yyvsp[-1].mrtBlendingExp);
    (yyval.mrtBlendingExp)->next = (yyvsp[0].mrtBlendingExp);
  }
#line 2417 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 81: /* MRT_BLENDING_EXP_LIST: MRT_BLENDING_EXP  */
#line 578 "src/shaders_compiler/parser/parser.y"
                         {
    (yyval.mrtBlendingExp) = (yyvsp[0].mrtBlendingExp);
    (yyval.mrtBlendingExp)->next = nullptr;
  }
#line 2426 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 82: /* MRT_BLENDING_EXP: "has_blending" "=" BOOL_VALUE ";"  */
#line 585 "src/shaders_compiler/parser/parser.y"
                                         {
    (yyval.mrtBlendingExp) = new MrtBlendingEnabledExp((yyvsp[-1].bval));
  }
#line 2434 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 83: /* MRT_BLENDING_EXP: "src_color_blend_factor" "=" BLEND_FACTOR ";"  */
#line 588 "src/shaders_compiler/parser/parser.y"
                                                     {
    (yyval.mrtBlendingExp) = new MrtSrcColorBlendFactorExp((yyvsp[-1].blendFactor));
  }
#line 2442 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 84: /* MRT_BLENDING_EXP: "dst_color_blend_factor" "=" BLEND_FACTOR ";"  */
#line 591 "src/shaders_compiler/parser/parser.y"
                                                     {
    (yyval.mrtBlendingExp) = new MrtDstColorBlendFactorExp((yyvsp[-1].blendFactor));
  }
#line 2450 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 85: /* MRT_BLENDING_EXP: "color_blend_op" "=" BLEND_OP ";"  */
#line 594 "src/shaders_compiler/parser/parser.y"
                                          {
    (yyval.mrtBlendingExp) = new MrtColorBlendOpExp((yyvsp[-1].blendOp));
  }
#line 2458 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 86: /* MRT_BLENDING_EXP: "src_alpha_blend_factor" "=" BLEND_FACTOR ";"  */
#line 597 "src/shaders_compiler/parser/parser.y"
                                                     {
    (yyval.mrtBlendingExp) = new MrtSrcAlphaBlendFactorExp((yyvsp[-1].blendFactor));
  }
#line 2466 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 87: /* MRT_BLENDING_EXP: "dst_alpha_blend_factor" "=" BLEND_FACTOR ";"  */
#line 600 "src/shaders_compiler/parser/parser.y"
                                                     {
    (yyval.mrtBlendingExp) = new MrtDstAlphaBlendFactorExp((yyvsp[-1].blendFactor));
  }
#line 2474 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 88: /* MRT_BLENDING_EXP: "alpha_blend_op" "=" BLEND_OP ";"  */
#line 603 "src/shaders_compiler/parser/parser.y"
                                          {
    (yyval.mrtBlendingExp) = new MrtAlphaBlendOpExp((yyvsp[-1].blendOp));
  }
#line 2482 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 89: /* BLEND_FACTOR: "zero"  */
#line 609 "src/shaders_compiler/parser/parser.y"
           {
    (yyval.blendFactor) = gapi::BlendFactor::Zero;
  }
#line 2490 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 90: /* BLEND_FACTOR: "one"  */
#line 612 "src/shaders_compiler/parser/parser.y"
          {
    (yyval.blendFactor) = gapi::BlendFactor::One;
  }
#line 2498 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 91: /* BLEND_FACTOR: "src_color"  */
#line 615 "src/shaders_compiler/parser/parser.y"
                {
    (yyval.blendFactor) = gapi::BlendFactor::SrcColor;
  }
#line 2506 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 92: /* BLEND_FACTOR: "one_minus_src_color"  */
#line 618 "src/shaders_compiler/parser/parser.y"
                          {
    (yyval.blendFactor) = gapi::BlendFactor::OneMinusSrcColor;
  }
#line 2514 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 93: /* BLEND_FACTOR: "dst_color"  */
#line 621 "src/shaders_compiler/parser/parser.y"
                {
    (yyval.blendFactor) = gapi::BlendFactor::DstColor;
  }
#line 2522 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 94: /* BLEND_FACTOR: "one_minus_dst_color"  */
#line 624 "src/shaders_compiler/parser/parser.y"
                          {
    (yyval.blendFactor) = gapi::BlendFactor::OneMinusDstColor;
  }
#line 2530 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 95: /* BLEND_FACTOR: "src_alpha"  */
#line 627 "src/shaders_compiler/parser/parser.y"
                {
    (yyval.blendFactor) = gapi::BlendFactor::SrcAlpha;
  }
#line 2538 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 96: /* BLEND_FACTOR: "one_minus_src_alpha"  */
#line 630 "src/shaders_compiler/parser/parser.y"
                          {
    (yyval.blendFactor) = gapi::BlendFactor::OneMinusSrcAlpha;
  }
#line 2546 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 97: /* BLEND_FACTOR: "dst_alpha"  */
#line 633 "src/shaders_compiler/parser/parser.y"
                {
    (yyval.blendFactor) = gapi::BlendFactor::DstAlpha;
  }
#line 2554 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 98: /* BLEND_FACTOR: "one_minus_dst_alpha"  */
#line 636 "src/shaders_compiler/parser/parser.y"
                          {
    (yyval.blendFactor) = gapi::BlendFactor::OneMinusDstAlpha;
  }
#line 2562 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 99: /* BLEND_FACTOR: "constant_color"  */
#line 639 "src/shaders_compiler/parser/parser.y"
                     {
    (yyval.blendFactor) = gapi::BlendFactor::ConstantColor;
  }
#line 2570 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 100: /* BLEND_FACTOR: "one_minus_constant_color"  */
#line 642 "src/shaders_compiler/parser/parser.y"
                               {
    (yyval.blendFactor) = gapi::BlendFactor::OneMinusConstantColor;
  }
#line 2578 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 101: /* BLEND_FACTOR: "constant_alpha"  */
#line 645 "src/shaders_compiler/parser/parser.y"
                     {
    (yyval.blendFactor) = gapi::BlendFactor::ConstantAlpha;
  }
#line 2586 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 102: /* BLEND_FACTOR: "one_minus_constant_alpha"  */
#line 648 "src/shaders_compiler/parser/parser.y"
                               {
    (yyval.blendFactor) = gapi::BlendFactor::OneMinusConstantAlpha;
  }
#line 2594 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 103: /* BLEND_FACTOR: "src_alpha_saturate"  */
#line 651 "src/shaders_compiler/parser/parser.y"
                         {
    (yyval.blendFactor) = gapi::BlendFactor::SrcAlphaSaturate;
  }
#line 2602 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 104: /* BLEND_FACTOR: "src_one_color"  */
#line 654 "src/shaders_compiler/parser/parser.y"
                    {
    (yyval.blendFactor) = gapi::BlendFactor::SrcOneColor;
  }
#line 2610 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 105: /* BLEND_FACTOR: "one_minus_src_one_color"  */
#line 657 "src/shaders_compiler/parser/parser.y"
                              {
    (yyval.blendFactor) = gapi::BlendFactor::OneMinusSrcOneColor;
  }
#line 2618 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 106: /* BLEND_FACTOR: "src_one_alpha"  */
#line 660 "src/shaders_compiler/parser/parser.y"
                    {
    (yyval.blendFactor) = gapi::BlendFactor::SrcOneAlpha;
  }
#line 2626 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 107: /* BLEND_FACTOR: "one_minus_src_one_alpha"  */
#line 663 "src/shaders_compiler/parser/parser.y"
                              {
    (yyval.blendFactor) = gapi::BlendFactor::OneMinusSrcOneAlpha;
  }
#line 2634 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 108: /* BLEND_OP: "add"  */
#line 669 "src/shaders_compiler/parser/parser.y"
          {
    (yyval.blendOp) = gapi::BlendOp::Add;
  }
#line 2642 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 109: /* BLEND_OP: "subtract"  */
#line 672 "src/shaders_compiler/parser/parser.y"
               {
    (yyval.blendOp) = gapi::BlendOp::Subtract;
  }
#line 2650 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 110: /* BLEND_OP: "reverse_subtract"  */
#line 675 "src/shaders_compiler/parser/parser.y"
                       {
    (yyval.blendOp) = gapi::BlendOp::ReverseSubtract;
  }
#line 2658 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 111: /* BLEND_OP: "min"  */
#line 678 "src/shaders_compiler/parser/parser.y"
          {
    (yyval.blendOp) = gapi::BlendOp::Min;
  }
#line 2666 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 112: /* BLEND_OP: "max"  */
#line 681 "src/shaders_compiler/parser/parser.y"
          {
    (yyval.blendOp) = gapi::BlendOp::Max;
  }
#line 2674 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 113: /* LOGIC_OP: "clear"  */
#line 687 "src/shaders_compiler/parser/parser.y"
            {
    (yyval.logicOp) = gapi::LogicOp::Clear;
  }
#line 2682 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 114: /* LOGIC_OP: "and"  */
#line 690 "src/shaders_compiler/parser/parser.y"
          {
    (yyval.logicOp) = gapi::LogicOp::And;
  }
#line 2690 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 115: /* LOGIC_OP: "and_reverse"  */
#line 693 "src/shaders_compiler/parser/parser.y"
                  {
    (yyval.logicOp) = gapi::LogicOp::AndReverse;
  }
#line 2698 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 116: /* LOGIC_OP: "copy"  */
#line 696 "src/shaders_compiler/parser/parser.y"
           {
    (yyval.logicOp) = gapi::LogicOp::Copy;
  }
#line 2706 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 117: /* LOGIC_OP: "and_inverted"  */
#line 699 "src/shaders_compiler/parser/parser.y"
                   {
    (yyval.logicOp) = gapi::LogicOp::AndInverted;
  }
#line 2714 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 118: /* LOGIC_OP: "no_op"  */
#line 702 "src/shaders_compiler/parser/parser.y"
            {
    (yyval.logicOp) = gapi::LogicOp::NoOp;
  }
#line 2722 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 119: /* LOGIC_OP: "xor"  */
#line 705 "src/shaders_compiler/parser/parser.y"
          {
    (yyval.logicOp) = gapi::LogicOp::Xor;
  }
#line 2730 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 120: /* LOGIC_OP: "or"  */
#line 708 "src/shaders_compiler/parser/parser.y"
         {
    (yyval.logicOp) = gapi::LogicOp::Or;
  }
#line 2738 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 121: /* LOGIC_OP: "nor"  */
#line 711 "src/shaders_compiler/parser/parser.y"
          {
    (yyval.logicOp) = gapi::LogicOp::Nor;
  }
#line 2746 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 122: /* LOGIC_OP: "equivalent"  */
#line 714 "src/shaders_compiler/parser/parser.y"
                 {
    (yyval.logicOp) = gapi::LogicOp::Equivalent;
  }
#line 2754 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 123: /* LOGIC_OP: "invert"  */
#line 717 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.logicOp) = gapi::LogicOp::Invert;
  }
#line 2762 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 124: /* LOGIC_OP: "or_reverse"  */
#line 720 "src/shaders_compiler/parser/parser.y"
                 {
    (yyval.logicOp) = gapi::LogicOp::OrReverse;
  }
#line 2770 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 125: /* LOGIC_OP: "copy_inverted"  */
#line 723 "src/shaders_compiler/parser/parser.y"
                    {
    (yyval.logicOp) = gapi::LogicOp::CopyInverted;
  }
#line 2778 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 126: /* LOGIC_OP: "or_inverted"  */
#line 726 "src/shaders_compiler/parser/parser.y"
                  {
    (yyval.logicOp) = gapi::LogicOp::OrInverted;
  }
#line 2786 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 127: /* LOGIC_OP: "nand"  */
#line 729 "src/shaders_compiler/parser/parser.y"
           {
    (yyval.logicOp) = gapi::LogicOp::Nand;
  }
#line 2794 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 128: /* LOGIC_OP: "set"  */
#line 732 "src/shaders_compiler/parser/parser.y"
          {
    (yyval.logicOp) = gapi::LogicOp::Set;
  }
#line 2802 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 129: /* SCOPE_DECLARATION: "scope" TFX_TOKEN_NAME_VAL "{" SCOPE_EXP_LIST "}" ";"  */
#line 738 "src/shaders_compiler/parser/parser.y"
                                                                      {
    const bool isOk = compiler.onScopeDeclaration(new ScopeDeclarationExp{(yyvsp[-4].sval), (yyvsp[-2].scopeExp)});
    if (!isOk)
      YYABORT;
  }
#line 2812 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 130: /* SCOPE_EXP_LIST: SCOPE_EXP SCOPE_EXP_LIST  */
#line 746 "src/shaders_compiler/parser/parser.y"
                                       {
    (yyval.scopeExp) = (yyvsp[-1].scopeExp);
    (yyval.scopeExp)->next = (yyvsp[0].scopeExp);
  }
#line 2821 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 131: /* SCOPE_EXP_LIST: SCOPE_EXP  */
#line 750 "src/shaders_compiler/parser/parser.y"
                  {
    (yyval.scopeExp) = (yyvsp[0].scopeExp);
    (yyval.scopeExp)->next = nullptr;
  }
#line 2830 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 132: /* SCOPE_EXP: "reserve" ":" RESOURCE_RESERVE_EXP_LIST  */
#line 757 "src/shaders_compiler/parser/parser.y"
                                                  {
    (yyval.scopeExp) = new ShadersResourcesReserveExp((yyvsp[0].resReserveExp));
  }
#line 2838 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 133: /* SCOPE_EXP: RESOURCE_TYPE TFX_TOKEN_NAME_VAL "=" ASSIGN_EXP ";"  */
#line 760 "src/shaders_compiler/parser/parser.y"
                                                                          {
    (yyval.scopeExp) = new ResourceDeclarationExp((yyvsp[-4].resourceType), (yyvsp[-3].sval), (yyvsp[-1].resourceAssignExp));
  }
#line 2846 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 134: /* SCOPE_EXP: ATTRIBUTE_TYPE TFX_TOKEN_NAME_VAL "=" ASSIGN_EXP ";"  */
#line 763 "src/shaders_compiler/parser/parser.y"
                                                                           {
    (yyval.scopeExp) = new CbufferVarDeclarationExp((yyvsp[-4].attributeType), (yyvsp[-3].sval), (yyvsp[-1].resourceAssignExp));
  }
#line 2854 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 135: /* RESOURCE_RESERVE_EXP_LIST: RESOURCE_RESERVE_EXP ";" RESOURCE_RESERVE_EXP_LIST  */
#line 769 "src/shaders_compiler/parser/parser.y"
                                                                {
  (yyval.resReserveExp) = (yyvsp[-2].resReserveExp);
  (yyval.resReserveExp)->next = (yyvsp[0].resReserveExp);
 }
#line 2863 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 136: /* RESOURCE_RESERVE_EXP_LIST: RESOURCE_RESERVE_EXP ";"  */
#line 773 "src/shaders_compiler/parser/parser.y"
                                {
   (yyval.resReserveExp) = (yyvsp[-1].resReserveExp);
 }
#line 2871 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 137: /* RESOURCE_RESERVE_EXP: "register" "(" "texture" "," INT_VALUE "-" INT_VALUE ")"  */
#line 779 "src/shaders_compiler/parser/parser.y"
                                                                     {
    (yyval.resReserveExp) = new RegistersReserveExp(RegistersReserveExp::Register::Texture, (yyvsp[-3].ival), (yyvsp[-1].ival));
  }
#line 2879 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 138: /* RESOURCE_RESERVE_EXP: "register" "(" "texture" "," INT_VALUE ")"  */
#line 782 "src/shaders_compiler/parser/parser.y"
                                                   {
    (yyval.resReserveExp) = new RegistersReserveExp(RegistersReserveExp::Register::Texture, (yyvsp[-1].ival), (yyvsp[-1].ival));
  }
#line 2887 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 139: /* RESOURCE_RESERVE_EXP: "register" "(" "sampler" "," INT_VALUE "-" INT_VALUE ")"  */
#line 785 "src/shaders_compiler/parser/parser.y"
                                                                     {
    (yyval.resReserveExp) = new RegistersReserveExp(RegistersReserveExp::Register::Sampler, (yyvsp[-3].ival), (yyvsp[-1].ival));
  }
#line 2895 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 140: /* RESOURCE_RESERVE_EXP: "register" "(" "sampler" "," INT_VALUE ")"  */
#line 788 "src/shaders_compiler/parser/parser.y"
                                                   {
    (yyval.resReserveExp) = new RegistersReserveExp(RegistersReserveExp::Register::Sampler, (yyvsp[-1].ival), (yyvsp[-1].ival));
  }
#line 2903 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 141: /* RESOURCE_RESERVE_EXP: "register" "(" "buffer" "," INT_VALUE "-" INT_VALUE ")"  */
#line 791 "src/shaders_compiler/parser/parser.y"
                                                                    {
    (yyval.resReserveExp) = new RegistersReserveExp(RegistersReserveExp::Register::Buffer, (yyvsp[-3].ival), (yyvsp[-1].ival));
  }
#line 2911 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 142: /* RESOURCE_RESERVE_EXP: "register" "(" "buffer" "," INT_VALUE ")"  */
#line 794 "src/shaders_compiler/parser/parser.y"
                                                  {
    (yyval.resReserveExp) = new RegistersReserveExp(RegistersReserveExp::Register::Buffer, (yyvsp[-1].ival), (yyvsp[-1].ival));
  }
#line 2919 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 143: /* RESOURCE_RESERVE_EXP: "descriptor_set" "(" INT_VALUE ")"  */
#line 797 "src/shaders_compiler/parser/parser.y"
                                          {
    (yyval.resReserveExp) = new DescriptorSetReserveExp((yyvsp[-1].ival));
  }
#line 2927 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 144: /* ASSIGN_EXP: "extern" "(" TFX_TOKEN_NAME_VAL ")"  */
#line 803 "src/shaders_compiler/parser/parser.y"
                                              {
    (yyval.resourceAssignExp) = new AccessResource(ResourceAccessType::Extern, (yyvsp[-1].sval));
  }
#line 2935 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 145: /* ASSIGN_EXP: "channel" "(" TFX_TOKEN_NAME_VAL ")"  */
#line 806 "src/shaders_compiler/parser/parser.y"
                                               {
    (yyval.resourceAssignExp) = new AccessResource(ResourceAccessType::Channel, (yyvsp[-1].sval));
  }
#line 2943 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 146: /* RESOURCE_TYPE: "Texture2D"  */
#line 812 "src/shaders_compiler/parser/parser.y"
                        {
    (yyval.resourceType) = ResourceType::Texture2D;
  }
#line 2951 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 147: /* RESOURCE_TYPE: "TextureCube"  */
#line 815 "src/shaders_compiler/parser/parser.y"
                           {
    (yyval.resourceType) = ResourceType::TextureCube;
  }
#line 2959 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 148: /* RESOURCE_TYPE: "sampler"  */
#line 818 "src/shaders_compiler/parser/parser.y"
                      {
    (yyval.resourceType) = ResourceType::Sampler;
  }
#line 2967 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 149: /* ATTRIBUTE_TYPE: "int"  */
#line 824 "src/shaders_compiler/parser/parser.y"
                  {
    (yyval.attributeType) = gapi::AttributeType::Int;
  }
#line 2975 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 150: /* ATTRIBUTE_TYPE: "int2"  */
#line 827 "src/shaders_compiler/parser/parser.y"
                   {
    (yyval.attributeType) = gapi::AttributeType::Int2;
  }
#line 2983 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 151: /* ATTRIBUTE_TYPE: "int3"  */
#line 830 "src/shaders_compiler/parser/parser.y"
                   {
    (yyval.attributeType) = gapi::AttributeType::Int3;
  }
#line 2991 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 152: /* ATTRIBUTE_TYPE: "int4"  */
#line 833 "src/shaders_compiler/parser/parser.y"
                   {
    (yyval.attributeType) = gapi::AttributeType::Int4;
  }
#line 2999 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 153: /* ATTRIBUTE_TYPE: "float"  */
#line 836 "src/shaders_compiler/parser/parser.y"
                    {
    (yyval.attributeType) = gapi::AttributeType::Float;
  }
#line 3007 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 154: /* ATTRIBUTE_TYPE: "float2"  */
#line 839 "src/shaders_compiler/parser/parser.y"
                     {
    (yyval.attributeType) = gapi::AttributeType::Float2;
  }
#line 3015 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 155: /* ATTRIBUTE_TYPE: "float3"  */
#line 842 "src/shaders_compiler/parser/parser.y"
                     {
    (yyval.attributeType) = gapi::AttributeType::Float3;
  }
#line 3023 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 156: /* ATTRIBUTE_TYPE: "float4"  */
#line 845 "src/shaders_compiler/parser/parser.y"
                     {
    (yyval.attributeType) = gapi::AttributeType::Float4;
  }
#line 3031 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 157: /* ATTRIBUTE_TYPE: "float4_u8"  */
#line 848 "src/shaders_compiler/parser/parser.y"
                        {
    (yyval.attributeType) = gapi::AttributeType::Float4_u8;
  }
#line 3039 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 158: /* ATTRIBUTE_TYPE: "float4x4"  */
#line 851 "src/shaders_compiler/parser/parser.y"
                       {
    (yyval.attributeType) = gapi::AttributeType::Float4x4;
  }
#line 3047 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 159: /* FLOAT4_VALUE: "float4" "(" TFX_TOKEN_FLOAT_VAL "," TFX_TOKEN_FLOAT_VAL "," TFX_TOKEN_FLOAT_VAL "," TFX_TOKEN_FLOAT_VAL ")"  */
#line 857 "src/shaders_compiler/parser/parser.y"
                                                                                                                                 {
    (yyval.f4val) = float4((yyvsp[-7].fval), (yyvsp[-5].fval), (yyvsp[-3].fval), (yyvsp[-1].fval));
  }
#line 3055 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 160: /* INT_VALUE: TFX_TOKEN_INT_VAL  */
#line 863 "src/shaders_compiler/parser/parser.y"
                         {
    (yyval.ival) = (yyvsp[0].ival);
  }
#line 3063 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 161: /* BOOL_VALUE: TFX_TOKEN_BOOL_VAL  */
#line 869 "src/shaders_compiler/parser/parser.y"
                          {
    (yyval.bval) = (yyvsp[0].bval);
  }
#line 3071 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 162: /* TARGET_PROFILE: "vs_6_0"  */
#line 874 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::VS_6_0;
  }
#line 3079 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 163: /* TARGET_PROFILE: "vs_6_1"  */
#line 877 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::VS_6_1;
  }
#line 3087 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 164: /* TARGET_PROFILE: "vs_6_2"  */
#line 880 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::VS_6_2;
  }
#line 3095 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 165: /* TARGET_PROFILE: "vs_6_3"  */
#line 883 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::VS_6_3;
  }
#line 3103 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 166: /* TARGET_PROFILE: "vs_6_4"  */
#line 886 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::VS_6_4;
  }
#line 3111 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 167: /* TARGET_PROFILE: "vs_6_5"  */
#line 889 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::VS_6_5;
  }
#line 3119 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 168: /* TARGET_PROFILE: "vs_6_6"  */
#line 892 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::VS_6_6;
  }
#line 3127 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 169: /* TARGET_PROFILE: "vs_6_7"  */
#line 895 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::VS_6_7;
  }
#line 3135 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 170: /* TARGET_PROFILE: "ps_6_0"  */
#line 898 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::PS_6_0;
  }
#line 3143 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 171: /* TARGET_PROFILE: "ps_6_1"  */
#line 901 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::PS_6_1;
  }
#line 3151 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 172: /* TARGET_PROFILE: "ps_6_2"  */
#line 904 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::PS_6_2;
  }
#line 3159 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 173: /* TARGET_PROFILE: "ps_6_3"  */
#line 907 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::PS_6_3;
  }
#line 3167 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 174: /* TARGET_PROFILE: "ps_6_4"  */
#line 910 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::PS_6_4;
  }
#line 3175 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 175: /* TARGET_PROFILE: "ps_6_5"  */
#line 913 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::PS_6_5;
  }
#line 3183 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 176: /* TARGET_PROFILE: "ps_6_6"  */
#line 916 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::PS_6_6;
  }
#line 3191 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 177: /* TARGET_PROFILE: "ps_6_7"  */
#line 919 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::PS_6_7;
  }
#line 3199 "src/shaders_compiler/parser/parser.tab.cpp"
    break;


#line 3203 "src/shaders_compiler/parser/parser.tab.cpp"

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

#line 923 "src/shaders_compiler/parser/parser.y"


void sherror(Compiler& compiler, const char* msg) {
  logerror("parsing error: {} [{}:{}]", msg, compiler.getCurrentFileName(), compiler.getLine());
  compiler.markCompilationFailed();
}
