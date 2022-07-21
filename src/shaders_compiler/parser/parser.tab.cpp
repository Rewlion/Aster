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

#line 98 "src/shaders_compiler/parser/parser.tab.cpp"

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
  YYSYMBOL_TOKEN_FLOAT_VAL = 3,            /* TOKEN_FLOAT_VAL  */
  YYSYMBOL_TOKEN_INT_VAL = 4,              /* TOKEN_INT_VAL  */
  YYSYMBOL_TOKEN_NAME_VAL = 5,             /* TOKEN_NAME_VAL  */
  YYSYMBOL_TOKEN_BOOL_VAL = 6,             /* TOKEN_BOOL_VAL  */
  YYSYMBOL_TOKEN_HLSL_CODE = 7,            /* TOKEN_HLSL_CODE  */
  YYSYMBOL_TOKEN_AT = 8,                   /* "@"  */
  YYSYMBOL_TOKEN_COLON = 9,                /* ":"  */
  YYSYMBOL_TOKEN_SEMICOLON = 10,           /* ";"  */
  YYSYMBOL_TOKEN_EQUAL_OP = 11,            /* "="  */
  YYSYMBOL_TOKEN_LEFT_PARENTHESIS = 12,    /* "("  */
  YYSYMBOL_TOKEN_RIGHT_PARENTHESIS = 13,   /* ")"  */
  YYSYMBOL_TOKEN_LEFT_BRACKET = 14,        /* "{"  */
  YYSYMBOL_TOKEN_RIGHT_BRACKET = 15,       /* "}"  */
  YYSYMBOL_TOKEN_COMMA = 16,               /* ","  */
  YYSYMBOL_TOKEN_MINUS = 17,               /* "-"  */
  YYSYMBOL_TOKEN_INPUT = 18,               /* "input"  */
  YYSYMBOL_TOKEN_BUFFER = 19,              /* "buffer"  */
  YYSYMBOL_TOKEN_SCOPE = 20,               /* "scope"  */
  YYSYMBOL_TOKEN_SHADER = 21,              /* "shader"  */
  YYSYMBOL_TOKEN_SHADER_VERTEX = 22,       /* "vertex"  */
  YYSYMBOL_TOKEN_SHADER_PIXEL = 23,        /* "pixel"  */
  YYSYMBOL_TOKEN_REGISTER = 24,            /* "register"  */
  YYSYMBOL_TOKEN_TEXTURE = 25,             /* "texture"  */
  YYSYMBOL_TOKEN_CBUFFER = 26,             /* "cbuffer"  */
  YYSYMBOL_TOKEN_EXTERN = 27,              /* "extern"  */
  YYSYMBOL_TOKEN_CHANNEL = 28,             /* "channel"  */
  YYSYMBOL_TOKEN_DESCRIPTOR_SET = 29,      /* "descriptor_set"  */
  YYSYMBOL_TOKEN_TECHNIQUE = 30,           /* "technique"  */
  YYSYMBOL_TOKEN_SUPPORT = 31,             /* "support"  */
  YYSYMBOL_TOKEN_ACTIVATE = 32,            /* "activate"  */
  YYSYMBOL_TOKEN_COMPILE = 33,             /* "compile"  */
  YYSYMBOL_TOKEN_RENDER_STATE = 34,        /* "render_state"  */
  YYSYMBOL_TOKEN_PRIMITIVE_TOPOLOGY = 35,  /* "primitive_topology"  */
  YYSYMBOL_TOKEN_PT_POINT_LIST = 36,       /* "point_list"  */
  YYSYMBOL_TOKEN_PT_LINE_LIST = 37,        /* "line_list"  */
  YYSYMBOL_TOKEN_PT_LINE_STRIP = 38,       /* "line_strip"  */
  YYSYMBOL_TOKEN_PT_TRIANGLE_LIST = 39,    /* "triangle_list"  */
  YYSYMBOL_TOKEN_PT_TRIANGLE_FAN = 40,     /* "triangle_fan"  */
  YYSYMBOL_TOKEN_PT_LINE_LIST_WITH_ADJACENCY = 41, /* "line_list_with_adjacency"  */
  YYSYMBOL_TOKEN_PT_LINE_STRIP_WITH_ADJACENCY = 42, /* "line_strip_with_adjacency"  */
  YYSYMBOL_TOKEN_PT_TRIANGLE_LIST_WITH_ADJACENCY = 43, /* "triangle_list_with_adjacency"  */
  YYSYMBOL_TOKEN_PT_TRIANGLE_STRIP_WITH_ADJACENCY = 44, /* "triangle_strip_with_adjacency"  */
  YYSYMBOL_TOKEN_PT_PATCH_LIST = 45,       /* "patch_list"  */
  YYSYMBOL_TOKEN_DEPTH_TEST = 46,          /* "depth_test"  */
  YYSYMBOL_TOKEN_DEPTH_WRITE = 47,         /* "depth_write"  */
  YYSYMBOL_TOKEN_DEPTH_OP = 48,            /* "depth_op"  */
  YYSYMBOL_TOKEN_STENCIL_TEST = 49,        /* "stencil_test"  */
  YYSYMBOL_TOKEN_STENCIL_FAIL_OP = 50,     /* "stencil_fail_op"  */
  YYSYMBOL_TOKEN_STENCIL_PASS_OP = 51,     /* "stencil_pass_op"  */
  YYSYMBOL_TOKEN_STENCIL_DEPTH_FAIL_OP = 52, /* "stencil_depth_fail_op"  */
  YYSYMBOL_TOKEN_STENCIL_COMPARE_OP = 53,  /* "stencil_compare_op"  */
  YYSYMBOL_TOKEN_STENCIL_REFERENCE_VALUE = 54, /* "stencil_ref_val"  */
  YYSYMBOL_TOKEN_NEVER = 55,               /* "never"  */
  YYSYMBOL_TOKEN_LESS = 56,                /* "less"  */
  YYSYMBOL_TOKEN_EQUAL = 57,               /* "equal"  */
  YYSYMBOL_TOKEN_LESS_OR_EQUAL = 58,       /* "less_or_equal"  */
  YYSYMBOL_TOKEN_GREATER = 59,             /* "greater"  */
  YYSYMBOL_TOKEN_NOT_EQUAL = 60,           /* "not_equal"  */
  YYSYMBOL_TOKEN_GREATER_OR_EQUAL = 61,    /* "greater_or_equal"  */
  YYSYMBOL_TOKEN_ALWAYS = 62,              /* "always"  */
  YYSYMBOL_TOKEN_KEEP = 63,                /* "keep"  */
  YYSYMBOL_TOKEN_ZERO = 64,                /* "zero"  */
  YYSYMBOL_TOKEN_REPLACE = 65,             /* "replace"  */
  YYSYMBOL_TOKEN_INCREMENT_AND_CLAMP = 66, /* "inc_and_clamp"  */
  YYSYMBOL_TOKEN_DECREMENT_AND_CLAMP = 67, /* "dec_and_clamp"  */
  YYSYMBOL_TOKEN_INVERT = 68,              /* "invert"  */
  YYSYMBOL_TOKEN_INCREMENT_AND_WRAP = 69,  /* "inc_and_wrap"  */
  YYSYMBOL_TOKEN_DECREMENT_AND_WRAP = 70,  /* "dec_and_wrap"  */
  YYSYMBOL_TOKEN_TARGET_VS_6_0 = 71,       /* "vs_6_0"  */
  YYSYMBOL_TOKEN_TARGET_VS_6_1 = 72,       /* "vs_6_1"  */
  YYSYMBOL_TOKEN_TARGET_VS_6_2 = 73,       /* "vs_6_2"  */
  YYSYMBOL_TOKEN_TARGET_VS_6_3 = 74,       /* "vs_6_3"  */
  YYSYMBOL_TOKEN_TARGET_VS_6_4 = 75,       /* "vs_6_4"  */
  YYSYMBOL_TOKEN_TARGET_VS_6_5 = 76,       /* "vs_6_5"  */
  YYSYMBOL_TOKEN_TARGET_VS_6_6 = 77,       /* "vs_6_6"  */
  YYSYMBOL_TOKEN_TARGET_VS_6_7 = 78,       /* "vs_6_7"  */
  YYSYMBOL_TOKEN_TARGET_PS_6_0 = 79,       /* "ps_6_0"  */
  YYSYMBOL_TOKEN_TARGET_PS_6_1 = 80,       /* "ps_6_1"  */
  YYSYMBOL_TOKEN_TARGET_PS_6_2 = 81,       /* "ps_6_2"  */
  YYSYMBOL_TOKEN_TARGET_PS_6_3 = 82,       /* "ps_6_3"  */
  YYSYMBOL_TOKEN_TARGET_PS_6_4 = 83,       /* "ps_6_4"  */
  YYSYMBOL_TOKEN_TARGET_PS_6_5 = 84,       /* "ps_6_5"  */
  YYSYMBOL_TOKEN_TARGET_PS_6_6 = 85,       /* "ps_6_6"  */
  YYSYMBOL_TOKEN_TARGET_PS_6_7 = 86,       /* "ps_6_7"  */
  YYSYMBOL_TOKEN_FLOAT = 87,               /* TOKEN_FLOAT  */
  YYSYMBOL_TOKEN_FLOAT2 = 88,              /* TOKEN_FLOAT2  */
  YYSYMBOL_TOKEN_FLOAT3 = 89,              /* TOKEN_FLOAT3  */
  YYSYMBOL_TOKEN_FLOAT4 = 90,              /* TOKEN_FLOAT4  */
  YYSYMBOL_TOKEN_INT = 91,                 /* TOKEN_INT  */
  YYSYMBOL_TOKEN_INT2 = 92,                /* TOKEN_INT2  */
  YYSYMBOL_TOKEN_INT3 = 93,                /* TOKEN_INT3  */
  YYSYMBOL_TOKEN_INT4 = 94,                /* TOKEN_INT4  */
  YYSYMBOL_TOKEN_TEXTURE2D = 95,           /* TOKEN_TEXTURE2D  */
  YYSYMBOL_YYACCEPT = 96,                  /* $accept  */
  YYSYMBOL_MODULE_EXPRESSIONS = 97,        /* MODULE_EXPRESSIONS  */
  YYSYMBOL_MODULE_EXPRESSION = 98,         /* MODULE_EXPRESSION  */
  YYSYMBOL_TECHNIQUE_DECLARATION = 99,     /* TECHNIQUE_DECLARATION  */
  YYSYMBOL_TECHNIQUE_EXP_LIST = 100,       /* TECHNIQUE_EXP_LIST  */
  YYSYMBOL_TECHNIQUE_EXP = 101,            /* TECHNIQUE_EXP  */
  YYSYMBOL_INPUT_BUFFER_LIST = 102,        /* INPUT_BUFFER_LIST  */
  YYSYMBOL_INPUT_BUFFER = 103,             /* INPUT_BUFFER  */
  YYSYMBOL_INPUT_ATTRIBUTE_LIST = 104,     /* INPUT_ATTRIBUTE_LIST  */
  YYSYMBOL_INPUT_ATTRIBUTE = 105,          /* INPUT_ATTRIBUTE  */
  YYSYMBOL_RENDER_STATE_EXP_LIST = 106,    /* RENDER_STATE_EXP_LIST  */
  YYSYMBOL_RENDER_STATE_EXP = 107,         /* RENDER_STATE_EXP  */
  YYSYMBOL_PRIMITIVE_TOPOLOGY = 108,       /* PRIMITIVE_TOPOLOGY  */
  YYSYMBOL_COMPARE_OP = 109,               /* COMPARE_OP  */
  YYSYMBOL_STENCIL_OP = 110,               /* STENCIL_OP  */
  YYSYMBOL_SCOPE_DECLARATION = 111,        /* SCOPE_DECLARATION  */
  YYSYMBOL_SCOPE_EXP_LIST = 112,           /* SCOPE_EXP_LIST  */
  YYSYMBOL_SCOPE_EXP = 113,                /* SCOPE_EXP  */
  YYSYMBOL_RESOURCE_RESERVE_EXP_LIST = 114, /* RESOURCE_RESERVE_EXP_LIST  */
  YYSYMBOL_RESOURCE_RESERVE_EXP = 115,     /* RESOURCE_RESERVE_EXP  */
  YYSYMBOL_ASSIGN_EXP = 116,               /* ASSIGN_EXP  */
  YYSYMBOL_RESOURCE_TYPE = 117,            /* RESOURCE_TYPE  */
  YYSYMBOL_ATTRIBUTE_TYPE = 118,           /* ATTRIBUTE_TYPE  */
  YYSYMBOL_INT_VALUE = 119,                /* INT_VALUE  */
  YYSYMBOL_BOOL_VALUE = 120,               /* BOOL_VALUE  */
  YYSYMBOL_SHADERS_LIST = 121,             /* SHADERS_LIST  */
  YYSYMBOL_SHADERS_ELEM = 122,             /* SHADERS_ELEM  */
  YYSYMBOL_TARGET_PROFILE = 123            /* TARGET_PROFILE  */
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
typedef yytype_uint8 yy_state_t;

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
#define YYFINAL  9
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   187

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  96
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  28
/* YYNRULES -- Number of rules.  */
#define YYNRULES  102
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  199

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   350


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
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
      95
};

#if SHDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   191,   191,   193,   198,   200,   205,   211,   215,   221,
     224,   227,   230,   233,   236,   242,   246,   253,   259,   263,
     270,   276,   280,   287,   290,   293,   296,   299,   302,   305,
     308,   311,   314,   320,   323,   326,   329,   332,   335,   338,
     341,   344,   347,   353,   356,   359,   362,   365,   368,   371,
     374,   380,   383,   386,   389,   392,   395,   398,   401,   407,
     413,   417,   424,   427,   430,   436,   440,   446,   450,   453,
     459,   462,   468,   474,   477,   480,   483,   486,   489,   492,
     495,   501,   507,   512,   516,   522,   525,   531,   534,   537,
     540,   543,   546,   549,   552,   555,   558,   561,   564,   567,
     570,   573,   576
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
  "end of file", "error", "invalid token", "TOKEN_FLOAT_VAL",
  "TOKEN_INT_VAL", "TOKEN_NAME_VAL", "TOKEN_BOOL_VAL", "TOKEN_HLSL_CODE",
  "@", ":", ";", "=", "(", ")", "{", "}", ",", "-", "input", "buffer",
  "scope", "shader", "vertex", "pixel", "register", "texture", "cbuffer",
  "extern", "channel", "descriptor_set", "technique", "support",
  "activate", "compile", "render_state", "primitive_topology",
  "point_list", "line_list", "line_strip", "triangle_list", "triangle_fan",
  "line_list_with_adjacency", "line_strip_with_adjacency",
  "triangle_list_with_adjacency", "triangle_strip_with_adjacency",
  "patch_list", "depth_test", "depth_write", "depth_op", "stencil_test",
  "stencil_fail_op", "stencil_pass_op", "stencil_depth_fail_op",
  "stencil_compare_op", "stencil_ref_val", "never", "less", "equal",
  "less_or_equal", "greater", "not_equal", "greater_or_equal", "always",
  "keep", "zero", "replace", "inc_and_clamp", "dec_and_clamp", "invert",
  "inc_and_wrap", "dec_and_wrap", "vs_6_0", "vs_6_1", "vs_6_2", "vs_6_3",
  "vs_6_4", "vs_6_5", "vs_6_6", "vs_6_7", "ps_6_0", "ps_6_1", "ps_6_2",
  "ps_6_3", "ps_6_4", "ps_6_5", "ps_6_6", "ps_6_7", "TOKEN_FLOAT",
  "TOKEN_FLOAT2", "TOKEN_FLOAT3", "TOKEN_FLOAT4", "TOKEN_INT",
  "TOKEN_INT2", "TOKEN_INT3", "TOKEN_INT4", "TOKEN_TEXTURE2D", "$accept",
  "MODULE_EXPRESSIONS", "MODULE_EXPRESSION", "TECHNIQUE_DECLARATION",
  "TECHNIQUE_EXP_LIST", "TECHNIQUE_EXP", "INPUT_BUFFER_LIST",
  "INPUT_BUFFER", "INPUT_ATTRIBUTE_LIST", "INPUT_ATTRIBUTE",
  "RENDER_STATE_EXP_LIST", "RENDER_STATE_EXP", "PRIMITIVE_TOPOLOGY",
  "COMPARE_OP", "STENCIL_OP", "SCOPE_DECLARATION", "SCOPE_EXP_LIST",
  "SCOPE_EXP", "RESOURCE_RESERVE_EXP_LIST", "RESOURCE_RESERVE_EXP",
  "ASSIGN_EXP", "RESOURCE_TYPE", "ATTRIBUTE_TYPE", "INT_VALUE",
  "BOOL_VALUE", "SHADERS_LIST", "SHADERS_ELEM", "TARGET_PROFILE", YY_NULLPTR
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
     345,   346,   347,   348,   349,   350
};
#endif

#define YYPACT_NINF (-179)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      25,    -3,     5,     7,    25,  -179,  -179,     1,    40,  -179,
    -179,    -7,    -6,    44,  -179,  -179,  -179,  -179,  -179,  -179,
    -179,  -179,  -179,    83,    -7,    97,    98,  -179,    95,    85,
      86,    96,   100,    92,    -6,  -179,  -179,   101,    99,   102,
    -179,   103,   105,    94,   106,   112,   -42,   -30,   108,  -179,
     -18,    44,  -179,    50,    50,   107,  -179,    94,   110,   111,
    -179,  -179,  -179,  -179,  -179,  -179,  -179,  -179,  -179,  -179,
    -179,  -179,  -179,  -179,  -179,  -179,   109,   113,   115,   116,
     117,   118,   119,   120,   121,   122,   123,  -179,   -30,  -179,
     124,   125,   126,  -179,   129,  -179,   128,   130,   131,   133,
     140,  -179,  -179,  -179,   141,    20,   139,   139,    -9,   139,
       6,     6,     6,    -9,   140,  -179,   127,   140,   140,   -18,
     142,   143,  -179,  -179,  -179,   136,   137,  -179,  -179,  -179,
    -179,  -179,  -179,  -179,  -179,  -179,  -179,   144,  -179,   145,
     146,  -179,  -179,  -179,  -179,  -179,  -179,  -179,  -179,   147,
     148,  -179,  -179,  -179,  -179,  -179,  -179,  -179,  -179,   149,
     150,   151,   152,   153,   135,   154,   155,  -179,   156,   157,
     114,   161,  -179,  -179,  -179,  -179,  -179,  -179,  -179,  -179,
    -179,  -179,   140,  -179,  -179,  -179,  -179,     2,  -179,   158,
    -179,     2,   159,   140,  -179,   162,   160,  -179,  -179
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     3,     5,     4,     0,     0,     1,
       2,     0,     0,     0,    77,    78,    79,    80,    73,    74,
      75,    76,    72,     0,    61,     0,     0,    12,     0,     0,
       0,     0,     0,     0,     8,    85,    86,     0,    84,     0,
      60,     0,     0,     0,     0,     0,     0,     0,     0,     7,
       0,     0,    59,     0,     0,     0,    13,    16,     0,     0,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    14,    22,     6,
       0,     0,     0,    62,     0,    83,     0,     0,     0,     0,
       0,    15,     9,    10,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    21,     0,     0,     0,    66,
       0,     0,    63,    64,    81,     0,     0,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,     0,    82,     0,
       0,    43,    44,    45,    46,    47,    48,    49,    50,     0,
       0,    51,    52,    53,    54,    55,    56,    57,    58,     0,
       0,     0,     0,     0,     0,     0,     0,    65,     0,     0,
       0,     0,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,     0,    68,    69,    70,    71,     0,    11,     0,
      17,    19,     0,     0,    18,     0,     0,    20,    67
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -179,   170,  -179,  -179,    88,  -179,    78,  -179,   -38,  -179,
      77,  -179,  -179,    53,   -11,  -179,   163,  -179,    57,  -179,
     132,  -179,  -178,  -114,   -10,   134,  -179,  -179
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,    33,    34,    56,    57,   190,   191,
      87,    88,   137,   149,   159,     6,    23,    24,    93,    94,
      98,    25,    26,   125,   139,    37,    38,    76
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
     163,    27,     7,   165,   166,    77,    90,     9,    91,   192,
       8,    92,    28,   192,    13,    11,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    29,    30,    31,    32,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,     1,   141,   142,   143,   144,
     145,   146,   147,   148,    12,     2,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,    35,    36,   189,   151,
     152,   153,   154,   155,   156,   157,   158,    96,    97,   196,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    14,
      15,    16,    17,    18,    19,    20,    21,   140,    39,   150,
     160,   161,    41,    42,    43,    44,    45,    48,    46,    47,
      50,    58,    52,    55,    53,    51,    54,    59,    89,   100,
     102,   103,    49,   187,   105,   104,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   101,   116,   117,   118,   119,
     120,   122,   121,   123,   124,   138,   126,   168,   169,   170,
     171,   182,   164,   194,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   195,   115,   162,   183,   184,   185,
     186,   188,   197,   198,    10,   193,   167,     0,     0,     0,
       0,     0,     0,     0,     0,    95,    99,    40
};

static const yytype_int16 yycheck[] =
{
     114,     7,     5,   117,   118,    35,    24,     0,    26,   187,
       5,    29,    18,   191,    21,    14,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    31,    32,    33,    34,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    20,    55,    56,    57,    58,
      59,    60,    61,    62,    14,    30,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    22,    23,   182,    63,
      64,    65,    66,    67,    68,    69,    70,    27,    28,   193,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    87,
      88,    89,    90,    91,    92,    93,    94,   107,    15,   109,
     111,   112,     5,     5,     9,    20,    20,    15,    12,     9,
       9,     5,    10,    19,    11,    16,    11,     5,    10,    12,
      10,    10,    34,     9,    11,    16,    11,    11,    11,    11,
      11,    11,    11,    11,    11,    57,    12,    12,    12,    10,
      12,    10,    12,    10,     4,     6,     5,     5,     5,    13,
      13,    16,    25,   191,    10,    10,    10,    10,    10,    10,
      10,    10,    10,    10,     5,    88,   113,    13,    13,    13,
      13,    10,    10,    13,     4,    17,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    51,    54,    24
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    20,    30,    97,    98,    99,   111,     5,     5,     0,
      97,    14,    14,    21,    87,    88,    89,    90,    91,    92,
      93,    94,    95,   112,   113,   117,   118,     7,    18,    31,
      32,    33,    34,   100,   101,    22,    23,   121,   122,    15,
     112,     5,     5,     9,    20,    20,    12,     9,    15,   100,
       9,    16,    10,    11,    11,    19,   102,   103,     5,     5,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,   123,    35,    46,    47,
      48,    49,    50,    51,    52,    53,    54,   106,   107,    10,
      24,    26,    29,   114,   115,   121,    27,    28,   116,   116,
      12,   102,    10,    10,    16,    11,    11,    11,    11,    11,
      11,    11,    11,    11,    11,   106,    12,    12,    12,    10,
      12,    12,    10,    10,     4,   119,     5,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,   108,     6,   120,
     120,    55,    56,    57,    58,    59,    60,    61,    62,   109,
     120,    63,    64,    65,    66,    67,    68,    69,    70,   110,
     110,   110,   109,   119,    25,   119,   119,   114,     5,     5,
      13,    13,    10,    10,    10,    10,    10,    10,    10,    10,
      10,    10,    16,    13,    13,    13,    13,     9,    10,   119,
     104,   105,   118,    17,   104,     5,   119,    10,    13
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    96,    97,    97,    98,    98,    99,   100,   100,   101,
     101,   101,   101,   101,   101,   102,   102,   103,   104,   104,
     105,   106,   106,   107,   107,   107,   107,   107,   107,   107,
     107,   107,   107,   108,   108,   108,   108,   108,   108,   108,
     108,   108,   108,   109,   109,   109,   109,   109,   109,   109,
     109,   110,   110,   110,   110,   110,   110,   110,   110,   111,
     112,   112,   113,   113,   113,   114,   114,   115,   115,   115,
     116,   116,   117,   118,   118,   118,   118,   118,   118,   118,
     118,   119,   120,   121,   121,   122,   122,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     6,     2,     1,     4,
       4,     7,     1,     3,     3,     2,     1,     6,     2,     1,
       3,     2,     1,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     6,
       2,     1,     4,     5,     5,     3,     2,     8,     4,     4,
       4,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1
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
#line 191 "src/shaders_compiler/parser/parser.y"
                                         {
  }
#line 1581 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 3: /* MODULE_EXPRESSIONS: MODULE_EXPRESSION  */
#line 193 "src/shaders_compiler/parser/parser.y"
                      {
  }
#line 1588 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 4: /* MODULE_EXPRESSION: SCOPE_DECLARATION  */
#line 198 "src/shaders_compiler/parser/parser.y"
                      {
  }
#line 1595 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 5: /* MODULE_EXPRESSION: TECHNIQUE_DECLARATION  */
#line 200 "src/shaders_compiler/parser/parser.y"
                          {
  }
#line 1602 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 6: /* TECHNIQUE_DECLARATION: "technique" TOKEN_NAME_VAL "{" TECHNIQUE_EXP_LIST "}" ";"  */
#line 205 "src/shaders_compiler/parser/parser.y"
                                                                          {
    compiler.onTechniqueDeclaration(new TechniqueDeclarationExp{(yyvsp[-4].sval), (yyvsp[-2].techniqueExp)});
  }
#line 1610 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 7: /* TECHNIQUE_EXP_LIST: TECHNIQUE_EXP TECHNIQUE_EXP_LIST  */
#line 211 "src/shaders_compiler/parser/parser.y"
                                                {
    (yyval.techniqueExp) = (yyvsp[-1].techniqueExp);
    (yyval.techniqueExp)->next = (yyvsp[0].techniqueExp);
  }
#line 1619 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 8: /* TECHNIQUE_EXP_LIST: TECHNIQUE_EXP  */
#line 215 "src/shaders_compiler/parser/parser.y"
                       {
    (yyval.techniqueExp) = (yyvsp[0].techniqueExp);
  }
#line 1627 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 9: /* TECHNIQUE_EXP: "support" "scope" TOKEN_NAME_VAL ";"  */
#line 221 "src/shaders_compiler/parser/parser.y"
                                               {
    (yyval.techniqueExp) = new ScopeSupportExp((yyvsp[-1].sval));
  }
#line 1635 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 10: /* TECHNIQUE_EXP: "activate" "scope" TOKEN_NAME_VAL ";"  */
#line 224 "src/shaders_compiler/parser/parser.y"
                                                {
    (yyval.techniqueExp) = new ScopeActivateExp((yyvsp[-1].sval));
  }
#line 1643 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 11: /* TECHNIQUE_EXP: "compile" "(" TARGET_PROFILE "," TOKEN_NAME_VAL ")" ";"  */
#line 227 "src/shaders_compiler/parser/parser.y"
                                                                           {
    (yyval.techniqueExp) = new CompileExp((yyvsp[-4].targetProfile), (yyvsp[-2].sval));
  }
#line 1651 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 12: /* TECHNIQUE_EXP: TOKEN_HLSL_CODE  */
#line 230 "src/shaders_compiler/parser/parser.y"
                          {
    (yyval.techniqueExp) = new HlslExp((yyvsp[0].sval));
  }
#line 1659 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 13: /* TECHNIQUE_EXP: "input" ":" INPUT_BUFFER_LIST  */
#line 233 "src/shaders_compiler/parser/parser.y"
                                           {
    (yyval.techniqueExp) = new InputExp((yyvsp[0].inputBufferExp));
  }
#line 1667 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 14: /* TECHNIQUE_EXP: "render_state" ":" RENDER_STATE_EXP_LIST  */
#line 236 "src/shaders_compiler/parser/parser.y"
                                                 {
    (yyval.techniqueExp) = (yyvsp[0].renderStateExp);
  }
#line 1675 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 15: /* INPUT_BUFFER_LIST: INPUT_BUFFER INPUT_BUFFER_LIST  */
#line 242 "src/shaders_compiler/parser/parser.y"
                                                 {
    (yyval.inputBufferExp) = (yyvsp[-1].inputBufferExp);
    (yyval.inputBufferExp)->next = (yyvsp[0].inputBufferExp);
  }
#line 1684 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 16: /* INPUT_BUFFER_LIST: INPUT_BUFFER  */
#line 246 "src/shaders_compiler/parser/parser.y"
                         {
    (yyval.inputBufferExp) = (yyvsp[0].inputBufferExp);
    (yyval.inputBufferExp)->next = nullptr;
  }
#line 1693 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 17: /* INPUT_BUFFER: "buffer" "(" INT_VALUE ")" ":" INPUT_ATTRIBUTE_LIST  */
#line 253 "src/shaders_compiler/parser/parser.y"
                                                                    {
    (yyval.inputBufferExp) = new InputBufferExp((yyvsp[-3].ival), (yyvsp[0].inputAttributeExp));
  }
#line 1701 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 18: /* INPUT_ATTRIBUTE_LIST: INPUT_ATTRIBUTE INPUT_ATTRIBUTE_LIST  */
#line 259 "src/shaders_compiler/parser/parser.y"
                                                     {
    (yyval.inputAttributeExp) = (yyvsp[-1].inputAttributeExp);
    (yyval.inputAttributeExp)->next = (yyvsp[0].inputAttributeExp);
  }
#line 1710 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 19: /* INPUT_ATTRIBUTE_LIST: INPUT_ATTRIBUTE  */
#line 263 "src/shaders_compiler/parser/parser.y"
                          {
    (yyval.inputAttributeExp) = (yyvsp[0].inputAttributeExp);
    (yyval.inputAttributeExp)->next = nullptr;
  }
#line 1719 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 20: /* INPUT_ATTRIBUTE: ATTRIBUTE_TYPE TOKEN_NAME_VAL ";"  */
#line 270 "src/shaders_compiler/parser/parser.y"
                                                  {
    (yyval.inputAttributeExp) = new InputAttributeExp((yyvsp[-2].attributeType), (yyvsp[-1].sval));
  }
#line 1727 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 21: /* RENDER_STATE_EXP_LIST: RENDER_STATE_EXP RENDER_STATE_EXP_LIST  */
#line 276 "src/shaders_compiler/parser/parser.y"
                                                        {
    (yyval.renderStateExp) = (yyvsp[-1].renderStateExp);
    (yyval.renderStateExp)->next = (yyvsp[0].renderStateExp);
  }
#line 1736 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 22: /* RENDER_STATE_EXP_LIST: RENDER_STATE_EXP  */
#line 280 "src/shaders_compiler/parser/parser.y"
                            {
    (yyval.renderStateExp) = (yyvsp[0].renderStateExp);
    (yyval.renderStateExp)->next = nullptr;
  }
#line 1745 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 23: /* RENDER_STATE_EXP: "primitive_topology" "=" PRIMITIVE_TOPOLOGY ";"  */
#line 287 "src/shaders_compiler/parser/parser.y"
                                                      {
    (yyval.renderStateExp) = new PrimitiveTopologyExp((yyvsp[-1].primitiveTopology));
  }
#line 1753 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 24: /* RENDER_STATE_EXP: "depth_test" "=" BOOL_VALUE ";"  */
#line 290 "src/shaders_compiler/parser/parser.y"
                                       {
    (yyval.renderStateExp) = new DepthTestExp((yyvsp[-1].bval));
  }
#line 1761 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 25: /* RENDER_STATE_EXP: "depth_write" "=" BOOL_VALUE ";"  */
#line 293 "src/shaders_compiler/parser/parser.y"
                                        {
    (yyval.renderStateExp) = new DepthWriteExp((yyvsp[-1].bval));
  }
#line 1769 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 26: /* RENDER_STATE_EXP: "depth_op" "=" COMPARE_OP ";"  */
#line 296 "src/shaders_compiler/parser/parser.y"
                                      {
    (yyval.renderStateExp) = new DepthOpExp((yyvsp[-1].compareOp));
  }
#line 1777 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 27: /* RENDER_STATE_EXP: "stencil_test" "=" BOOL_VALUE ";"  */
#line 299 "src/shaders_compiler/parser/parser.y"
                                         {
    (yyval.renderStateExp) = new StencilTestExp((yyvsp[-1].bval));
  }
#line 1785 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 28: /* RENDER_STATE_EXP: "stencil_fail_op" "=" STENCIL_OP ";"  */
#line 302 "src/shaders_compiler/parser/parser.y"
                                             {
    (yyval.renderStateExp) = new StencilFailOpExp((yyvsp[-1].stencilOp));
  }
#line 1793 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 29: /* RENDER_STATE_EXP: "stencil_pass_op" "=" STENCIL_OP ";"  */
#line 305 "src/shaders_compiler/parser/parser.y"
                                             {
    (yyval.renderStateExp) = new StencilPassOpExp((yyvsp[-1].stencilOp));
  }
#line 1801 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 30: /* RENDER_STATE_EXP: "stencil_depth_fail_op" "=" STENCIL_OP ";"  */
#line 308 "src/shaders_compiler/parser/parser.y"
                                                   {
    (yyval.renderStateExp) = new StencilDepthFailOpExp((yyvsp[-1].stencilOp));
  }
#line 1809 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 31: /* RENDER_STATE_EXP: "stencil_compare_op" "=" COMPARE_OP ";"  */
#line 311 "src/shaders_compiler/parser/parser.y"
                                                {
    (yyval.renderStateExp) = new StencilCompareOpExp((yyvsp[-1].compareOp));
  }
#line 1817 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 32: /* RENDER_STATE_EXP: "stencil_ref_val" "=" INT_VALUE ";"  */
#line 314 "src/shaders_compiler/parser/parser.y"
                                           {
    (yyval.renderStateExp) = new StencilReferenceValueExp((yyvsp[-1].ival));
  }
#line 1825 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 33: /* PRIMITIVE_TOPOLOGY: "point_list"  */
#line 320 "src/shaders_compiler/parser/parser.y"
                 {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::PointList;
  }
#line 1833 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 34: /* PRIMITIVE_TOPOLOGY: "line_list"  */
#line 323 "src/shaders_compiler/parser/parser.y"
                {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::LineList;
  }
#line 1841 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 35: /* PRIMITIVE_TOPOLOGY: "line_strip"  */
#line 326 "src/shaders_compiler/parser/parser.y"
                 {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::LineStrip;
  }
#line 1849 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 36: /* PRIMITIVE_TOPOLOGY: "triangle_list"  */
#line 329 "src/shaders_compiler/parser/parser.y"
                    {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::TriangleList;
  }
#line 1857 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 37: /* PRIMITIVE_TOPOLOGY: "triangle_fan"  */
#line 332 "src/shaders_compiler/parser/parser.y"
                   {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::TriangleFan;
  }
#line 1865 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 38: /* PRIMITIVE_TOPOLOGY: "line_list_with_adjacency"  */
#line 335 "src/shaders_compiler/parser/parser.y"
                               {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::LineListWithAdjacency;
  }
#line 1873 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 39: /* PRIMITIVE_TOPOLOGY: "line_strip_with_adjacency"  */
#line 338 "src/shaders_compiler/parser/parser.y"
                                {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::LineStripWithAdjacency;
  }
#line 1881 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 40: /* PRIMITIVE_TOPOLOGY: "triangle_list_with_adjacency"  */
#line 341 "src/shaders_compiler/parser/parser.y"
                                   {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::TriangleListWithAdjacency;
  }
#line 1889 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 41: /* PRIMITIVE_TOPOLOGY: "triangle_strip_with_adjacency"  */
#line 344 "src/shaders_compiler/parser/parser.y"
                                    {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::TriangleStripWithAdjacency;
  }
#line 1897 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 42: /* PRIMITIVE_TOPOLOGY: "patch_list"  */
#line 347 "src/shaders_compiler/parser/parser.y"
                 {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::PatchList;
  }
#line 1905 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 43: /* COMPARE_OP: "never"  */
#line 353 "src/shaders_compiler/parser/parser.y"
            {
   (yyval.compareOp) = gapi::CompareOp::Never;
  }
#line 1913 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 44: /* COMPARE_OP: "less"  */
#line 356 "src/shaders_compiler/parser/parser.y"
           {
   (yyval.compareOp) = gapi::CompareOp::Less;
  }
#line 1921 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 45: /* COMPARE_OP: "equal"  */
#line 359 "src/shaders_compiler/parser/parser.y"
            {
   (yyval.compareOp) = gapi::CompareOp::Never;
  }
#line 1929 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 46: /* COMPARE_OP: "less_or_equal"  */
#line 362 "src/shaders_compiler/parser/parser.y"
                    {
   (yyval.compareOp) = gapi::CompareOp::LessOrEqual;
  }
#line 1937 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 47: /* COMPARE_OP: "greater"  */
#line 365 "src/shaders_compiler/parser/parser.y"
              {
   (yyval.compareOp) = gapi::CompareOp::Greater;
  }
#line 1945 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 48: /* COMPARE_OP: "not_equal"  */
#line 368 "src/shaders_compiler/parser/parser.y"
                {
   (yyval.compareOp) = gapi::CompareOp::NotEqual;
  }
#line 1953 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 49: /* COMPARE_OP: "greater_or_equal"  */
#line 371 "src/shaders_compiler/parser/parser.y"
                       {
   (yyval.compareOp) = gapi::CompareOp::GreaterOrEqual;
  }
#line 1961 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 50: /* COMPARE_OP: "always"  */
#line 374 "src/shaders_compiler/parser/parser.y"
             {
   (yyval.compareOp) = gapi::CompareOp::Always;
  }
#line 1969 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 51: /* STENCIL_OP: "keep"  */
#line 380 "src/shaders_compiler/parser/parser.y"
           {
    (yyval.stencilOp) = gapi::StencilOp::Keep;
  }
#line 1977 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 52: /* STENCIL_OP: "zero"  */
#line 383 "src/shaders_compiler/parser/parser.y"
           {
    (yyval.stencilOp) = gapi::StencilOp::Zero;
  }
#line 1985 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 53: /* STENCIL_OP: "replace"  */
#line 386 "src/shaders_compiler/parser/parser.y"
              {
    (yyval.stencilOp) = gapi::StencilOp::Replace;
  }
#line 1993 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 54: /* STENCIL_OP: "inc_and_clamp"  */
#line 389 "src/shaders_compiler/parser/parser.y"
                    {
    (yyval.stencilOp) = gapi::StencilOp::IncrementAndClamp;
  }
#line 2001 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 55: /* STENCIL_OP: "dec_and_clamp"  */
#line 392 "src/shaders_compiler/parser/parser.y"
                    {
    (yyval.stencilOp) = gapi::StencilOp::DecrementAndClamp;
  }
#line 2009 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 56: /* STENCIL_OP: "invert"  */
#line 395 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.stencilOp) = gapi::StencilOp::Invert;
  }
#line 2017 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 57: /* STENCIL_OP: "inc_and_wrap"  */
#line 398 "src/shaders_compiler/parser/parser.y"
                   {
    (yyval.stencilOp) = gapi::StencilOp::IncrementAndWrap;
  }
#line 2025 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 58: /* STENCIL_OP: "dec_and_wrap"  */
#line 401 "src/shaders_compiler/parser/parser.y"
                   {
    (yyval.stencilOp) = gapi::StencilOp::DecrementAndWrap;
  }
#line 2033 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 59: /* SCOPE_DECLARATION: "scope" TOKEN_NAME_VAL "{" SCOPE_EXP_LIST "}" ";"  */
#line 407 "src/shaders_compiler/parser/parser.y"
                                                                  {
    compiler.onScopeDeclaration(new ScopeDeclarationExp{(yyvsp[-4].sval), (yyvsp[-2].scopeExp)});
  }
#line 2041 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 60: /* SCOPE_EXP_LIST: SCOPE_EXP SCOPE_EXP_LIST  */
#line 413 "src/shaders_compiler/parser/parser.y"
                                       {
    (yyval.scopeExp) = (yyvsp[-1].scopeExp);
    (yyval.scopeExp)->next = (yyvsp[0].scopeExp);
  }
#line 2050 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 61: /* SCOPE_EXP_LIST: SCOPE_EXP  */
#line 417 "src/shaders_compiler/parser/parser.y"
                  {
    (yyval.scopeExp) = (yyvsp[0].scopeExp);
    (yyval.scopeExp)->next = nullptr;
  }
#line 2059 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 62: /* SCOPE_EXP: "shader" SHADERS_LIST ":" RESOURCE_RESERVE_EXP_LIST  */
#line 424 "src/shaders_compiler/parser/parser.y"
                                                                           {
    (yyval.scopeExp) = new ShadersResourcesReserveExp((yyvsp[-2].shaderExp), (yyvsp[0].resReserveExp));
  }
#line 2067 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 63: /* SCOPE_EXP: RESOURCE_TYPE TOKEN_NAME_VAL "=" ASSIGN_EXP ";"  */
#line 427 "src/shaders_compiler/parser/parser.y"
                                                                      {
    (yyval.scopeExp) = new ResourceDeclarationExp((yyvsp[-4].resourceType), (yyvsp[-3].sval), (yyvsp[-1].resourceAssignExp));
  }
#line 2075 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 64: /* SCOPE_EXP: ATTRIBUTE_TYPE TOKEN_NAME_VAL "=" ASSIGN_EXP ";"  */
#line 430 "src/shaders_compiler/parser/parser.y"
                                                                       {
    (yyval.scopeExp) = new CbufferVarDeclarationExp((yyvsp[-4].attributeType), (yyvsp[-3].sval), (yyvsp[-1].resourceAssignExp));
  }
#line 2083 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 65: /* RESOURCE_RESERVE_EXP_LIST: RESOURCE_RESERVE_EXP ";" RESOURCE_RESERVE_EXP_LIST  */
#line 436 "src/shaders_compiler/parser/parser.y"
                                                                {
  (yyval.resReserveExp) = (yyvsp[-2].resReserveExp);
  (yyval.resReserveExp)->next = (yyvsp[0].resReserveExp);
 }
#line 2092 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 66: /* RESOURCE_RESERVE_EXP_LIST: RESOURCE_RESERVE_EXP ";"  */
#line 440 "src/shaders_compiler/parser/parser.y"
                                {
   (yyval.resReserveExp) = (yyvsp[-1].resReserveExp);
 }
#line 2100 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 67: /* RESOURCE_RESERVE_EXP: "register" "(" "texture" "," INT_VALUE "-" INT_VALUE ")"  */
#line 446 "src/shaders_compiler/parser/parser.y"
                                                                     {
    (yyval.resReserveExp) = new RegistersReserveExp(RegistersReserveExp::Register::Texture, (yyvsp[-3].ival), (yyvsp[-1].ival));

  }
#line 2109 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 68: /* RESOURCE_RESERVE_EXP: "cbuffer" "(" INT_VALUE ")"  */
#line 450 "src/shaders_compiler/parser/parser.y"
                                          {
    (yyval.resReserveExp) = new CBufferReserveExp((yyvsp[-1].ival));
  }
#line 2117 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 69: /* RESOURCE_RESERVE_EXP: "descriptor_set" "(" INT_VALUE ")"  */
#line 453 "src/shaders_compiler/parser/parser.y"
                                          {
    (yyval.resReserveExp) = new DescriptorSetReserveExp((yyvsp[-1].ival));
  }
#line 2125 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 70: /* ASSIGN_EXP: "extern" "(" TOKEN_NAME_VAL ")"  */
#line 459 "src/shaders_compiler/parser/parser.y"
                                          {
    (yyval.resourceAssignExp) = new AccessResource(ResourceAccessType::Extern, (yyvsp[-1].sval));
  }
#line 2133 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 71: /* ASSIGN_EXP: "channel" "(" TOKEN_NAME_VAL ")"  */
#line 462 "src/shaders_compiler/parser/parser.y"
                                           {
    (yyval.resourceAssignExp) = new AccessResource(ResourceAccessType::Channel, (yyvsp[-1].sval));
  }
#line 2141 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 72: /* RESOURCE_TYPE: TOKEN_TEXTURE2D  */
#line 468 "src/shaders_compiler/parser/parser.y"
                    {
    (yyval.resourceType) = ResourceType::Texture2D;
  }
#line 2149 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 73: /* ATTRIBUTE_TYPE: TOKEN_INT  */
#line 474 "src/shaders_compiler/parser/parser.y"
              {
    (yyval.attributeType) = AttributeType::Int;
  }
#line 2157 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 74: /* ATTRIBUTE_TYPE: TOKEN_INT2  */
#line 477 "src/shaders_compiler/parser/parser.y"
               {
    (yyval.attributeType) = AttributeType::Int2;
  }
#line 2165 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 75: /* ATTRIBUTE_TYPE: TOKEN_INT3  */
#line 480 "src/shaders_compiler/parser/parser.y"
               {
    (yyval.attributeType) = AttributeType::Int3;
  }
#line 2173 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 76: /* ATTRIBUTE_TYPE: TOKEN_INT4  */
#line 483 "src/shaders_compiler/parser/parser.y"
               {
    (yyval.attributeType) = AttributeType::Int4;
  }
#line 2181 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 77: /* ATTRIBUTE_TYPE: TOKEN_FLOAT  */
#line 486 "src/shaders_compiler/parser/parser.y"
                {
    (yyval.attributeType) = AttributeType::Float;
  }
#line 2189 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 78: /* ATTRIBUTE_TYPE: TOKEN_FLOAT2  */
#line 489 "src/shaders_compiler/parser/parser.y"
                 {
    (yyval.attributeType) = AttributeType::Float2;
  }
#line 2197 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 79: /* ATTRIBUTE_TYPE: TOKEN_FLOAT3  */
#line 492 "src/shaders_compiler/parser/parser.y"
                 {
    (yyval.attributeType) = AttributeType::Float3;
  }
#line 2205 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 80: /* ATTRIBUTE_TYPE: TOKEN_FLOAT4  */
#line 495 "src/shaders_compiler/parser/parser.y"
                 {
    (yyval.attributeType) = AttributeType::Float4;
  }
#line 2213 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 81: /* INT_VALUE: TOKEN_INT_VAL  */
#line 501 "src/shaders_compiler/parser/parser.y"
                     {
    (yyval.ival) = (yyvsp[0].ival);
  }
#line 2221 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 82: /* BOOL_VALUE: TOKEN_BOOL_VAL  */
#line 507 "src/shaders_compiler/parser/parser.y"
                      {
    (yyval.bval) = (yyvsp[0].bval);
  }
#line 2229 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 83: /* SHADERS_LIST: SHADERS_ELEM "," SHADERS_LIST  */
#line 512 "src/shaders_compiler/parser/parser.y"
                                                   {
    (yyval.shaderExp) = (yyvsp[0].shaderExp);
    (yyval.shaderExp)->shaders = (gapi::ShaderStage)((yyval.shaderExp)->shaders | (yyvsp[-2].shader));
  }
#line 2238 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 84: /* SHADERS_LIST: SHADERS_ELEM  */
#line 516 "src/shaders_compiler/parser/parser.y"
                         {
    (yyval.shaderExp) = new ShaderExp((yyvsp[0].shader));
  }
#line 2246 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 85: /* SHADERS_ELEM: "vertex"  */
#line 522 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.shader) = gapi::ShaderStage::Vertex;
  }
#line 2254 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 86: /* SHADERS_ELEM: "pixel"  */
#line 525 "src/shaders_compiler/parser/parser.y"
            {
    (yyval.shader) = gapi::ShaderStage::Fragment;
  }
#line 2262 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 87: /* TARGET_PROFILE: "vs_6_0"  */
#line 531 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::VS_6_0;
  }
#line 2270 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 88: /* TARGET_PROFILE: "vs_6_1"  */
#line 534 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::VS_6_1;
  }
#line 2278 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 89: /* TARGET_PROFILE: "vs_6_2"  */
#line 537 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::VS_6_2;
  }
#line 2286 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 90: /* TARGET_PROFILE: "vs_6_3"  */
#line 540 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::VS_6_3;
  }
#line 2294 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 91: /* TARGET_PROFILE: "vs_6_4"  */
#line 543 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::VS_6_4;
  }
#line 2302 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 92: /* TARGET_PROFILE: "vs_6_5"  */
#line 546 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::VS_6_5;
  }
#line 2310 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 93: /* TARGET_PROFILE: "vs_6_6"  */
#line 549 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::VS_6_6;
  }
#line 2318 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 94: /* TARGET_PROFILE: "vs_6_7"  */
#line 552 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::VS_6_7;
  }
#line 2326 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 95: /* TARGET_PROFILE: "ps_6_0"  */
#line 555 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::PS_6_0;
  }
#line 2334 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 96: /* TARGET_PROFILE: "ps_6_1"  */
#line 558 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::PS_6_1;
  }
#line 2342 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 97: /* TARGET_PROFILE: "ps_6_2"  */
#line 561 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::PS_6_2;
  }
#line 2350 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 98: /* TARGET_PROFILE: "ps_6_3"  */
#line 564 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::PS_6_3;
  }
#line 2358 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 99: /* TARGET_PROFILE: "ps_6_4"  */
#line 567 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::PS_6_4;
  }
#line 2366 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 100: /* TARGET_PROFILE: "ps_6_5"  */
#line 570 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::PS_6_5;
  }
#line 2374 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 101: /* TARGET_PROFILE: "ps_6_6"  */
#line 573 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::PS_6_6;
  }
#line 2382 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 102: /* TARGET_PROFILE: "ps_6_7"  */
#line 576 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::PS_6_7;
  }
#line 2390 "src/shaders_compiler/parser/parser.tab.cpp"
    break;


#line 2394 "src/shaders_compiler/parser/parser.tab.cpp"

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

#line 580 "src/shaders_compiler/parser/parser.y"


void sherror(Compiler& compiler, const char* msg) {
  logerror("[{}] shader technique parsing error: {} {}", "<todo>", shlineno, msg);
}
