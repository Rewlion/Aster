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
  YYSYMBOL_TOKEN_HLSL_CODE = 6,            /* TOKEN_HLSL_CODE  */
  YYSYMBOL_TOKEN_AT = 7,                   /* "@"  */
  YYSYMBOL_TOKEN_COLON = 8,                /* ":"  */
  YYSYMBOL_TOKEN_SEMICOLON = 9,            /* ";"  */
  YYSYMBOL_TOKEN_EQUAL_OP = 10,            /* "="  */
  YYSYMBOL_TOKEN_LEFT_PARENTHESIS = 11,    /* "("  */
  YYSYMBOL_TOKEN_RIGHT_PARENTHESIS = 12,   /* ")"  */
  YYSYMBOL_TOKEN_LEFT_BRACKET = 13,        /* "{"  */
  YYSYMBOL_TOKEN_RIGHT_BRACKET = 14,       /* "}"  */
  YYSYMBOL_TOKEN_COMMA = 15,               /* ","  */
  YYSYMBOL_TOKEN_MINUS = 16,               /* "-"  */
  YYSYMBOL_TOKEN_INPUT = 17,               /* "input"  */
  YYSYMBOL_TOKEN_BUFFER = 18,              /* "buffer"  */
  YYSYMBOL_TOKEN_SCOPE = 19,               /* "scope"  */
  YYSYMBOL_TOKEN_SHADER = 20,              /* "shader"  */
  YYSYMBOL_TOKEN_SHADER_VERTEX = 21,       /* "vertex"  */
  YYSYMBOL_TOKEN_SHADER_PIXEL = 22,        /* "pixel"  */
  YYSYMBOL_TOKEN_REGISTER = 23,            /* "register"  */
  YYSYMBOL_TOKEN_TEXTURE = 24,             /* "texture"  */
  YYSYMBOL_TOKEN_CBUFFER = 25,             /* "cbuffer"  */
  YYSYMBOL_TOKEN_EXTERN = 26,              /* "extern"  */
  YYSYMBOL_TOKEN_CHANNEL = 27,             /* "channel"  */
  YYSYMBOL_TOKEN_DESCRIPTOR_SET = 28,      /* "descriptor_set"  */
  YYSYMBOL_TOKEN_TECHNIQUE = 29,           /* "technique"  */
  YYSYMBOL_TOKEN_SUPPORT = 30,             /* "support"  */
  YYSYMBOL_TOKEN_ACTIVATE = 31,            /* "activate"  */
  YYSYMBOL_TOKEN_COMPILE = 32,             /* "compile"  */
  YYSYMBOL_TOKEN_PRIMITIVE_TOPOLOGY = 33,  /* "primitive_topology"  */
  YYSYMBOL_TOKEN_PT_POINT_LIST = 34,       /* "point_list"  */
  YYSYMBOL_TOKEN_PT_LINE_LIST = 35,        /* "line_list"  */
  YYSYMBOL_TOKEN_PT_LINE_STRIP = 36,       /* "line_strip"  */
  YYSYMBOL_TOKEN_PT_TRIANGLE_LIST = 37,    /* "triangle_list"  */
  YYSYMBOL_TOKEN_PT_TRIANGLE_FAN = 38,     /* "triangle_fan"  */
  YYSYMBOL_TOKEN_PT_LINE_LIST_WITH_ADJACENCY = 39, /* "line_list_with_adjacency"  */
  YYSYMBOL_TOKEN_PT_LINE_STRIP_WITH_ADJACENCY = 40, /* "line_strip_with_adjacency"  */
  YYSYMBOL_TOKEN_PT_TRIANGLE_LIST_WITH_ADJACENCY = 41, /* "triangle_list_with_adjacency"  */
  YYSYMBOL_TOKEN_PT_TRIANGLE_STRIP_WITH_ADJACENCY = 42, /* "triangle_strip_with_adjacency"  */
  YYSYMBOL_TOKEN_PT_PATCH_LIST = 43,       /* "patch_list"  */
  YYSYMBOL_TOKEN_TARGET_VS_6_0 = 44,       /* "vs_6_0"  */
  YYSYMBOL_TOKEN_TARGET_VS_6_1 = 45,       /* "vs_6_1"  */
  YYSYMBOL_TOKEN_TARGET_VS_6_2 = 46,       /* "vs_6_2"  */
  YYSYMBOL_TOKEN_TARGET_VS_6_3 = 47,       /* "vs_6_3"  */
  YYSYMBOL_TOKEN_TARGET_VS_6_4 = 48,       /* "vs_6_4"  */
  YYSYMBOL_TOKEN_TARGET_VS_6_5 = 49,       /* "vs_6_5"  */
  YYSYMBOL_TOKEN_TARGET_VS_6_6 = 50,       /* "vs_6_6"  */
  YYSYMBOL_TOKEN_TARGET_VS_6_7 = 51,       /* "vs_6_7"  */
  YYSYMBOL_TOKEN_TARGET_PS_6_0 = 52,       /* "ps_6_0"  */
  YYSYMBOL_TOKEN_TARGET_PS_6_1 = 53,       /* "ps_6_1"  */
  YYSYMBOL_TOKEN_TARGET_PS_6_2 = 54,       /* "ps_6_2"  */
  YYSYMBOL_TOKEN_TARGET_PS_6_3 = 55,       /* "ps_6_3"  */
  YYSYMBOL_TOKEN_TARGET_PS_6_4 = 56,       /* "ps_6_4"  */
  YYSYMBOL_TOKEN_TARGET_PS_6_5 = 57,       /* "ps_6_5"  */
  YYSYMBOL_TOKEN_TARGET_PS_6_6 = 58,       /* "ps_6_6"  */
  YYSYMBOL_TOKEN_TARGET_PS_6_7 = 59,       /* "ps_6_7"  */
  YYSYMBOL_TOKEN_FLOAT = 60,               /* TOKEN_FLOAT  */
  YYSYMBOL_TOKEN_FLOAT2 = 61,              /* TOKEN_FLOAT2  */
  YYSYMBOL_TOKEN_FLOAT3 = 62,              /* TOKEN_FLOAT3  */
  YYSYMBOL_TOKEN_FLOAT4 = 63,              /* TOKEN_FLOAT4  */
  YYSYMBOL_TOKEN_INT = 64,                 /* TOKEN_INT  */
  YYSYMBOL_TOKEN_INT2 = 65,                /* TOKEN_INT2  */
  YYSYMBOL_TOKEN_INT3 = 66,                /* TOKEN_INT3  */
  YYSYMBOL_TOKEN_INT4 = 67,                /* TOKEN_INT4  */
  YYSYMBOL_TOKEN_TEXTURE2D = 68,           /* TOKEN_TEXTURE2D  */
  YYSYMBOL_YYACCEPT = 69,                  /* $accept  */
  YYSYMBOL_MODULE_EXPRESSIONS = 70,        /* MODULE_EXPRESSIONS  */
  YYSYMBOL_MODULE_EXPRESSION = 71,         /* MODULE_EXPRESSION  */
  YYSYMBOL_TECHNIQUE_DECLARATION = 72,     /* TECHNIQUE_DECLARATION  */
  YYSYMBOL_TECHNIQUE_EXP_LIST = 73,        /* TECHNIQUE_EXP_LIST  */
  YYSYMBOL_TECHNIQUE_EXP = 74,             /* TECHNIQUE_EXP  */
  YYSYMBOL_INPUT_BUFFER_LIST = 75,         /* INPUT_BUFFER_LIST  */
  YYSYMBOL_INPUT_BUFFER = 76,              /* INPUT_BUFFER  */
  YYSYMBOL_INPUT_ATTRIBUTE_LIST = 77,      /* INPUT_ATTRIBUTE_LIST  */
  YYSYMBOL_INPUT_ATTRIBUTE = 78,           /* INPUT_ATTRIBUTE  */
  YYSYMBOL_PRIMITIVE_TOPOLOGY = 79,        /* PRIMITIVE_TOPOLOGY  */
  YYSYMBOL_SCOPE_DECLARATION = 80,         /* SCOPE_DECLARATION  */
  YYSYMBOL_SCOPE_EXP_LIST = 81,            /* SCOPE_EXP_LIST  */
  YYSYMBOL_SCOPE_EXP = 82,                 /* SCOPE_EXP  */
  YYSYMBOL_RESOURCE_RESERVE_EXP_LIST = 83, /* RESOURCE_RESERVE_EXP_LIST  */
  YYSYMBOL_RESOURCE_RESERVE_EXP = 84,      /* RESOURCE_RESERVE_EXP  */
  YYSYMBOL_ASSIGN_EXP = 85,                /* ASSIGN_EXP  */
  YYSYMBOL_RESOURCE_TYPE = 86,             /* RESOURCE_TYPE  */
  YYSYMBOL_ATTRIBUTE_TYPE = 87,            /* ATTRIBUTE_TYPE  */
  YYSYMBOL_INT_VALUE = 88,                 /* INT_VALUE  */
  YYSYMBOL_SHADERS_LIST = 89,              /* SHADERS_LIST  */
  YYSYMBOL_SHADERS_ELEM = 90,              /* SHADERS_ELEM  */
  YYSYMBOL_TARGET_PROFILE = 91             /* TARGET_PROFILE  */
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
#define YYLAST   135

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  69
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  23
/* YYNRULES -- Number of rules.  */
#define YYNRULES  73
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  141

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   323


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
      65,    66,    67,    68
};

#if SHDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   155,   155,   157,   162,   164,   169,   175,   179,   185,
     188,   191,   194,   197,   200,   206,   210,   217,   223,   227,
     234,   240,   243,   246,   249,   252,   255,   258,   261,   264,
     267,   273,   279,   283,   290,   293,   296,   302,   306,   312,
     316,   319,   325,   328,   334,   340,   343,   346,   349,   352,
     355,   358,   361,   367,   373,   377,   383,   386,   392,   395,
     398,   401,   404,   407,   410,   413,   416,   419,   422,   425,
     428,   431,   434,   437
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
  "TOKEN_INT_VAL", "TOKEN_NAME_VAL", "TOKEN_HLSL_CODE", "@", ":", ";", "=",
  "(", ")", "{", "}", ",", "-", "input", "buffer", "scope", "shader",
  "vertex", "pixel", "register", "texture", "cbuffer", "extern", "channel",
  "descriptor_set", "technique", "support", "activate", "compile",
  "primitive_topology", "point_list", "line_list", "line_strip",
  "triangle_list", "triangle_fan", "line_list_with_adjacency",
  "line_strip_with_adjacency", "triangle_list_with_adjacency",
  "triangle_strip_with_adjacency", "patch_list", "vs_6_0", "vs_6_1",
  "vs_6_2", "vs_6_3", "vs_6_4", "vs_6_5", "vs_6_6", "vs_6_7", "ps_6_0",
  "ps_6_1", "ps_6_2", "ps_6_3", "ps_6_4", "ps_6_5", "ps_6_6", "ps_6_7",
  "TOKEN_FLOAT", "TOKEN_FLOAT2", "TOKEN_FLOAT3", "TOKEN_FLOAT4",
  "TOKEN_INT", "TOKEN_INT2", "TOKEN_INT3", "TOKEN_INT4", "TOKEN_TEXTURE2D",
  "$accept", "MODULE_EXPRESSIONS", "MODULE_EXPRESSION",
  "TECHNIQUE_DECLARATION", "TECHNIQUE_EXP_LIST", "TECHNIQUE_EXP",
  "INPUT_BUFFER_LIST", "INPUT_BUFFER", "INPUT_ATTRIBUTE_LIST",
  "INPUT_ATTRIBUTE", "PRIMITIVE_TOPOLOGY", "SCOPE_DECLARATION",
  "SCOPE_EXP_LIST", "SCOPE_EXP", "RESOURCE_RESERVE_EXP_LIST",
  "RESOURCE_RESERVE_EXP", "ASSIGN_EXP", "RESOURCE_TYPE", "ATTRIBUTE_TYPE",
  "INT_VALUE", "SHADERS_LIST", "SHADERS_ELEM", "TARGET_PROFILE", YY_NULLPTR
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
     315,   316,   317,   318,   319,   320,   321,   322,   323
};
#endif

#define YYPACT_NINF (-122)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
     -16,     0,    11,    17,   -16,  -122,  -122,    22,    23,  -122,
    -122,   -20,     1,   -11,  -122,  -122,  -122,  -122,  -122,  -122,
    -122,  -122,  -122,    24,   -20,    32,    34,  -122,    57,    55,
      56,    65,    67,    64,     1,  -122,  -122,    71,    66,    73,
    -122,    70,    74,    68,    78,    80,     5,   -14,    79,  -122,
     -19,   -11,  -122,   -12,   -12,    76,  -122,    68,    81,    82,
    -122,  -122,  -122,  -122,  -122,  -122,  -122,  -122,  -122,  -122,
    -122,  -122,  -122,  -122,  -122,  -122,    77,  -122,  -122,  -122,
    -122,  -122,  -122,  -122,  -122,  -122,  -122,    84,  -122,    83,
      85,    86,  -122,    89,  -122,    88,    90,    91,    93,    99,
    -122,  -122,  -122,   100,  -122,    87,    99,    99,   -19,   101,
     102,  -122,  -122,  -122,    92,    96,    94,    98,   103,  -122,
     104,   105,   106,   109,    99,  -122,  -122,  -122,  -122,     6,
    -122,    97,  -122,     6,   107,    99,  -122,   110,   108,  -122,
    -122
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     3,     5,     4,     0,     0,     1,
       2,     0,     0,     0,    49,    50,    51,    52,    45,    46,
      47,    48,    44,     0,    33,     0,     0,    12,     0,     0,
       0,     0,     0,     0,     8,    56,    57,     0,    55,     0,
      32,     0,     0,     0,     0,     0,     0,     0,     0,     7,
       0,     0,    31,     0,     0,     0,    13,    16,     0,     0,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,     0,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,     0,     6,     0,
       0,     0,    34,     0,    54,     0,     0,     0,     0,     0,
      15,     9,    10,     0,    14,     0,     0,     0,    38,     0,
       0,    35,    36,    53,     0,     0,     0,     0,     0,    37,
       0,     0,     0,     0,     0,    40,    41,    42,    43,     0,
      11,     0,    17,    19,     0,     0,    18,     0,     0,    20,
      39
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -122,   117,  -122,  -122,    61,  -122,    69,  -122,   -44,  -122,
    -122,  -122,   111,  -122,    14,  -122,    75,  -122,  -121,  -105,
      72,  -122,  -122
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,    33,    34,    56,    57,   132,   133,
      87,     6,    23,    24,    92,    93,    97,    25,    26,   114,
      37,    38,    76
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      13,   117,   118,     1,    89,     7,    90,    27,   134,    91,
      35,    36,   134,     2,    95,    96,     8,     9,    28,   131,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
     138,    29,    30,    31,    32,    11,    12,    41,    39,    42,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    43,    14,    15,    16,    17,
      18,    19,    20,    21,    44,    45,    46,    47,    48,    50,
      53,    51,    52,    58,    54,    59,    55,    99,    88,   136,
     101,   102,   103,   104,   105,    49,   106,   107,   108,   109,
     111,   110,   112,   113,   122,   115,   120,   121,   123,   124,
     125,   116,   137,   135,   129,   126,   127,   128,   130,   139,
     140,    10,   119,    94,     0,     0,   100,     0,     0,    98,
       0,     0,     0,     0,     0,    40
};

static const yytype_int16 yycheck[] =
{
      20,   106,   107,    19,    23,     5,    25,     6,   129,    28,
      21,    22,   133,    29,    26,    27,     5,     0,    17,   124,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
     135,    30,    31,    32,    33,    13,    13,     5,    14,     5,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,     8,    60,    61,    62,    63,
      64,    65,    66,    67,    19,    19,    11,    10,    14,     8,
      10,    15,     9,     5,    10,     5,    18,    11,     9,   133,
       9,     9,    15,     9,    11,    34,    11,    11,     9,    11,
       9,    11,     9,     4,    12,     5,     5,     5,    12,    15,
      12,    24,     5,    16,     8,    12,    12,    12,     9,     9,
      12,     4,   108,    51,    -1,    -1,    57,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    24
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    19,    29,    70,    71,    72,    80,     5,     5,     0,
      70,    13,    13,    20,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    81,    82,    86,    87,     6,    17,    30,
      31,    32,    33,    73,    74,    21,    22,    89,    90,    14,
      81,     5,     5,     8,    19,    19,    11,    10,    14,    73,
       8,    15,     9,    10,    10,    18,    75,    76,     5,     5,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    91,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    79,     9,    23,
      25,    28,    83,    84,    89,    26,    27,    85,    85,    11,
      75,     9,     9,    15,     9,    11,    11,    11,     9,    11,
      11,     9,     9,     4,    88,     5,    24,    88,    88,    83,
       5,     5,    12,    12,    15,    12,    12,    12,    12,     8,
       9,    88,    77,    78,    87,    16,    77,     5,    88,     9,
      12
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    69,    70,    70,    71,    71,    72,    73,    73,    74,
      74,    74,    74,    74,    74,    75,    75,    76,    77,    77,
      78,    79,    79,    79,    79,    79,    79,    79,    79,    79,
      79,    80,    81,    81,    82,    82,    82,    83,    83,    84,
      84,    84,    85,    85,    86,    87,    87,    87,    87,    87,
      87,    87,    87,    88,    89,    89,    90,    90,    91,    91,
      91,    91,    91,    91,    91,    91,    91,    91,    91,    91,
      91,    91,    91,    91
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     6,     2,     1,     4,
       4,     7,     1,     3,     4,     2,     1,     6,     2,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     6,     2,     1,     4,     5,     5,     3,     2,     8,
       4,     4,     4,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1
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
#line 155 "src/shaders_compiler/parser/parser.y"
                                         {
  }
#line 1502 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 3: /* MODULE_EXPRESSIONS: MODULE_EXPRESSION  */
#line 157 "src/shaders_compiler/parser/parser.y"
                      {
  }
#line 1509 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 4: /* MODULE_EXPRESSION: SCOPE_DECLARATION  */
#line 162 "src/shaders_compiler/parser/parser.y"
                      {
  }
#line 1516 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 5: /* MODULE_EXPRESSION: TECHNIQUE_DECLARATION  */
#line 164 "src/shaders_compiler/parser/parser.y"
                          {
  }
#line 1523 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 6: /* TECHNIQUE_DECLARATION: "technique" TOKEN_NAME_VAL "{" TECHNIQUE_EXP_LIST "}" ";"  */
#line 169 "src/shaders_compiler/parser/parser.y"
                                                                          {
    compiler.onTechniqueDeclaration(new TechniqueDeclarationExp{(yyvsp[-4].sval), (yyvsp[-2].techniqueExp)});
  }
#line 1531 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 7: /* TECHNIQUE_EXP_LIST: TECHNIQUE_EXP TECHNIQUE_EXP_LIST  */
#line 175 "src/shaders_compiler/parser/parser.y"
                                                {
    (yyval.techniqueExp) = (yyvsp[-1].techniqueExp);
    (yyval.techniqueExp)->next = (yyvsp[0].techniqueExp);
  }
#line 1540 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 8: /* TECHNIQUE_EXP_LIST: TECHNIQUE_EXP  */
#line 179 "src/shaders_compiler/parser/parser.y"
                       {
    (yyval.techniqueExp) = (yyvsp[0].techniqueExp);
  }
#line 1548 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 9: /* TECHNIQUE_EXP: "support" "scope" TOKEN_NAME_VAL ";"  */
#line 185 "src/shaders_compiler/parser/parser.y"
                                               {
    (yyval.techniqueExp) = new ScopeSupportExp((yyvsp[-1].sval));
  }
#line 1556 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 10: /* TECHNIQUE_EXP: "activate" "scope" TOKEN_NAME_VAL ";"  */
#line 188 "src/shaders_compiler/parser/parser.y"
                                                {
    (yyval.techniqueExp) = new ScopeActivateExp((yyvsp[-1].sval));
  }
#line 1564 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 11: /* TECHNIQUE_EXP: "compile" "(" TARGET_PROFILE "," TOKEN_NAME_VAL ")" ";"  */
#line 191 "src/shaders_compiler/parser/parser.y"
                                                                           {
    (yyval.techniqueExp) = new CompileExp((yyvsp[-4].targetProfile), (yyvsp[-2].sval));
  }
#line 1572 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 12: /* TECHNIQUE_EXP: TOKEN_HLSL_CODE  */
#line 194 "src/shaders_compiler/parser/parser.y"
                          {
    (yyval.techniqueExp) = new HlslExp((yyvsp[0].sval));
  }
#line 1580 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 13: /* TECHNIQUE_EXP: "input" ":" INPUT_BUFFER_LIST  */
#line 197 "src/shaders_compiler/parser/parser.y"
                                           {
    (yyval.techniqueExp) = new InputExp((yyvsp[0].inputBufferExp));
  }
#line 1588 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 14: /* TECHNIQUE_EXP: "primitive_topology" "=" PRIMITIVE_TOPOLOGY ";"  */
#line 200 "src/shaders_compiler/parser/parser.y"
                                                      {
    (yyval.techniqueExp) = new PrimitiveTopologyExp((yyvsp[-1].primitiveTopology));
  }
#line 1596 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 15: /* INPUT_BUFFER_LIST: INPUT_BUFFER INPUT_BUFFER_LIST  */
#line 206 "src/shaders_compiler/parser/parser.y"
                                                 {
    (yyval.inputBufferExp) = (yyvsp[-1].inputBufferExp);
    (yyval.inputBufferExp)->next = (yyvsp[0].inputBufferExp);
  }
#line 1605 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 16: /* INPUT_BUFFER_LIST: INPUT_BUFFER  */
#line 210 "src/shaders_compiler/parser/parser.y"
                         {
    (yyval.inputBufferExp) = (yyvsp[0].inputBufferExp);
    (yyval.inputBufferExp)->next = nullptr;
  }
#line 1614 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 17: /* INPUT_BUFFER: "buffer" "(" INT_VALUE ")" ":" INPUT_ATTRIBUTE_LIST  */
#line 217 "src/shaders_compiler/parser/parser.y"
                                                                    {
    (yyval.inputBufferExp) = new InputBufferExp((yyvsp[-3].ival), (yyvsp[0].inputAttributeExp));
  }
#line 1622 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 18: /* INPUT_ATTRIBUTE_LIST: INPUT_ATTRIBUTE INPUT_ATTRIBUTE_LIST  */
#line 223 "src/shaders_compiler/parser/parser.y"
                                                     {
    (yyval.inputAttributeExp) = (yyvsp[-1].inputAttributeExp);
    (yyval.inputAttributeExp)->next = (yyvsp[0].inputAttributeExp);
  }
#line 1631 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 19: /* INPUT_ATTRIBUTE_LIST: INPUT_ATTRIBUTE  */
#line 227 "src/shaders_compiler/parser/parser.y"
                          {
    (yyval.inputAttributeExp) = (yyvsp[0].inputAttributeExp);
    (yyval.inputAttributeExp)->next = nullptr;
  }
#line 1640 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 20: /* INPUT_ATTRIBUTE: ATTRIBUTE_TYPE TOKEN_NAME_VAL ";"  */
#line 234 "src/shaders_compiler/parser/parser.y"
                                                  {
    (yyval.inputAttributeExp) = new InputAttributeExp((yyvsp[-2].attributeType), (yyvsp[-1].sval));
  }
#line 1648 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 21: /* PRIMITIVE_TOPOLOGY: "point_list"  */
#line 240 "src/shaders_compiler/parser/parser.y"
                 {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::PointList;
  }
#line 1656 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 22: /* PRIMITIVE_TOPOLOGY: "line_list"  */
#line 243 "src/shaders_compiler/parser/parser.y"
                {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::LineList;
  }
#line 1664 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 23: /* PRIMITIVE_TOPOLOGY: "line_strip"  */
#line 246 "src/shaders_compiler/parser/parser.y"
                 {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::LineStrip;
  }
#line 1672 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 24: /* PRIMITIVE_TOPOLOGY: "triangle_list"  */
#line 249 "src/shaders_compiler/parser/parser.y"
                    {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::TriangleList;
  }
#line 1680 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 25: /* PRIMITIVE_TOPOLOGY: "triangle_fan"  */
#line 252 "src/shaders_compiler/parser/parser.y"
                   {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::TriangleFan;
  }
#line 1688 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 26: /* PRIMITIVE_TOPOLOGY: "line_list_with_adjacency"  */
#line 255 "src/shaders_compiler/parser/parser.y"
                               {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::LineListWithAdjacency;
  }
#line 1696 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 27: /* PRIMITIVE_TOPOLOGY: "line_strip_with_adjacency"  */
#line 258 "src/shaders_compiler/parser/parser.y"
                                {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::LineStripWithAdjacency;
  }
#line 1704 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 28: /* PRIMITIVE_TOPOLOGY: "triangle_list_with_adjacency"  */
#line 261 "src/shaders_compiler/parser/parser.y"
                                   {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::TriangleListWithAdjacency;
  }
#line 1712 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 29: /* PRIMITIVE_TOPOLOGY: "triangle_strip_with_adjacency"  */
#line 264 "src/shaders_compiler/parser/parser.y"
                                    {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::TriangleStripWithAdjacency;
  }
#line 1720 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 30: /* PRIMITIVE_TOPOLOGY: "patch_list"  */
#line 267 "src/shaders_compiler/parser/parser.y"
                 {
    (yyval.primitiveTopology) = gapi::PrimitiveTopology::PatchList;
  }
#line 1728 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 31: /* SCOPE_DECLARATION: "scope" TOKEN_NAME_VAL "{" SCOPE_EXP_LIST "}" ";"  */
#line 273 "src/shaders_compiler/parser/parser.y"
                                                                  {
    compiler.onScopeDeclaration(new ScopeDeclarationExp{(yyvsp[-4].sval), (yyvsp[-2].scopeExp)});
  }
#line 1736 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 32: /* SCOPE_EXP_LIST: SCOPE_EXP SCOPE_EXP_LIST  */
#line 279 "src/shaders_compiler/parser/parser.y"
                                       {
    (yyval.scopeExp) = (yyvsp[-1].scopeExp);
    (yyval.scopeExp)->next = (yyvsp[0].scopeExp);
  }
#line 1745 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 33: /* SCOPE_EXP_LIST: SCOPE_EXP  */
#line 283 "src/shaders_compiler/parser/parser.y"
                  {
    (yyval.scopeExp) = (yyvsp[0].scopeExp);
    (yyval.scopeExp)->next = nullptr;
  }
#line 1754 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 34: /* SCOPE_EXP: "shader" SHADERS_LIST ":" RESOURCE_RESERVE_EXP_LIST  */
#line 290 "src/shaders_compiler/parser/parser.y"
                                                                           {
    (yyval.scopeExp) = new ShadersResourcesReserveExp((yyvsp[-2].shaderExp), (yyvsp[0].resReserveExp));
  }
#line 1762 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 35: /* SCOPE_EXP: RESOURCE_TYPE TOKEN_NAME_VAL "=" ASSIGN_EXP ";"  */
#line 293 "src/shaders_compiler/parser/parser.y"
                                                                      {
    (yyval.scopeExp) = new ResourceDeclarationExp((yyvsp[-4].resourceType), (yyvsp[-3].sval), (yyvsp[-1].resourceAssignExp));
  }
#line 1770 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 36: /* SCOPE_EXP: ATTRIBUTE_TYPE TOKEN_NAME_VAL "=" ASSIGN_EXP ";"  */
#line 296 "src/shaders_compiler/parser/parser.y"
                                                                       {
    (yyval.scopeExp) = new CbufferVarDeclarationExp((yyvsp[-4].attributeType), (yyvsp[-3].sval), (yyvsp[-1].resourceAssignExp));
  }
#line 1778 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 37: /* RESOURCE_RESERVE_EXP_LIST: RESOURCE_RESERVE_EXP ";" RESOURCE_RESERVE_EXP_LIST  */
#line 302 "src/shaders_compiler/parser/parser.y"
                                                                {
  (yyval.resReserveExp) = (yyvsp[-2].resReserveExp);
  (yyval.resReserveExp)->next = (yyvsp[0].resReserveExp);
 }
#line 1787 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 38: /* RESOURCE_RESERVE_EXP_LIST: RESOURCE_RESERVE_EXP ";"  */
#line 306 "src/shaders_compiler/parser/parser.y"
                                {
   (yyval.resReserveExp) = (yyvsp[-1].resReserveExp);
 }
#line 1795 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 39: /* RESOURCE_RESERVE_EXP: "register" "(" "texture" "," INT_VALUE "-" INT_VALUE ")"  */
#line 312 "src/shaders_compiler/parser/parser.y"
                                                                     {
    (yyval.resReserveExp) = new RegistersReserveExp(RegistersReserveExp::Register::Texture, (yyvsp[-3].ival), (yyvsp[-1].ival));

  }
#line 1804 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 40: /* RESOURCE_RESERVE_EXP: "cbuffer" "(" INT_VALUE ")"  */
#line 316 "src/shaders_compiler/parser/parser.y"
                                          {
    (yyval.resReserveExp) = new CBufferReserveExp((yyvsp[-1].ival));
  }
#line 1812 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 41: /* RESOURCE_RESERVE_EXP: "descriptor_set" "(" INT_VALUE ")"  */
#line 319 "src/shaders_compiler/parser/parser.y"
                                          {
    (yyval.resReserveExp) = new DescriptorSetReserveExp((yyvsp[-1].ival));
  }
#line 1820 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 42: /* ASSIGN_EXP: "extern" "(" TOKEN_NAME_VAL ")"  */
#line 325 "src/shaders_compiler/parser/parser.y"
                                          {
    (yyval.resourceAssignExp) = new AccessResource(ResourceAccessType::Extern, (yyvsp[-1].sval));
  }
#line 1828 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 43: /* ASSIGN_EXP: "channel" "(" TOKEN_NAME_VAL ")"  */
#line 328 "src/shaders_compiler/parser/parser.y"
                                           {
    (yyval.resourceAssignExp) = new AccessResource(ResourceAccessType::Channel, (yyvsp[-1].sval));
  }
#line 1836 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 44: /* RESOURCE_TYPE: TOKEN_TEXTURE2D  */
#line 334 "src/shaders_compiler/parser/parser.y"
                    {
    (yyval.resourceType) = ResourceType::Texture2D;
  }
#line 1844 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 45: /* ATTRIBUTE_TYPE: TOKEN_INT  */
#line 340 "src/shaders_compiler/parser/parser.y"
              {
    (yyval.attributeType) = AttributeType::Int;
  }
#line 1852 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 46: /* ATTRIBUTE_TYPE: TOKEN_INT2  */
#line 343 "src/shaders_compiler/parser/parser.y"
               {
    (yyval.attributeType) = AttributeType::Int2;
  }
#line 1860 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 47: /* ATTRIBUTE_TYPE: TOKEN_INT3  */
#line 346 "src/shaders_compiler/parser/parser.y"
               {
    (yyval.attributeType) = AttributeType::Int3;
  }
#line 1868 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 48: /* ATTRIBUTE_TYPE: TOKEN_INT4  */
#line 349 "src/shaders_compiler/parser/parser.y"
               {
    (yyval.attributeType) = AttributeType::Int4;
  }
#line 1876 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 49: /* ATTRIBUTE_TYPE: TOKEN_FLOAT  */
#line 352 "src/shaders_compiler/parser/parser.y"
                {
    (yyval.attributeType) = AttributeType::Float;
  }
#line 1884 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 50: /* ATTRIBUTE_TYPE: TOKEN_FLOAT2  */
#line 355 "src/shaders_compiler/parser/parser.y"
                 {
    (yyval.attributeType) = AttributeType::Float2;
  }
#line 1892 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 51: /* ATTRIBUTE_TYPE: TOKEN_FLOAT3  */
#line 358 "src/shaders_compiler/parser/parser.y"
                 {
    (yyval.attributeType) = AttributeType::Float3;
  }
#line 1900 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 52: /* ATTRIBUTE_TYPE: TOKEN_FLOAT4  */
#line 361 "src/shaders_compiler/parser/parser.y"
                 {
    (yyval.attributeType) = AttributeType::Float4;
  }
#line 1908 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 53: /* INT_VALUE: TOKEN_INT_VAL  */
#line 367 "src/shaders_compiler/parser/parser.y"
                     {
    (yyval.ival) = (yyvsp[0].ival);
  }
#line 1916 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 54: /* SHADERS_LIST: SHADERS_ELEM "," SHADERS_LIST  */
#line 373 "src/shaders_compiler/parser/parser.y"
                                                   {
    (yyval.shaderExp) = (yyvsp[0].shaderExp);
    (yyval.shaderExp)->shaders = (gapi::ShaderStage)((yyval.shaderExp)->shaders | (yyvsp[-2].shader));
  }
#line 1925 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 55: /* SHADERS_LIST: SHADERS_ELEM  */
#line 377 "src/shaders_compiler/parser/parser.y"
                         {
    (yyval.shaderExp) = new ShaderExp((yyvsp[0].shader));
  }
#line 1933 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 56: /* SHADERS_ELEM: "vertex"  */
#line 383 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.shader) = gapi::ShaderStage::Vertex;
  }
#line 1941 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 57: /* SHADERS_ELEM: "pixel"  */
#line 386 "src/shaders_compiler/parser/parser.y"
            {
    (yyval.shader) = gapi::ShaderStage::Fragment;
  }
#line 1949 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 58: /* TARGET_PROFILE: "vs_6_0"  */
#line 392 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::VS_6_0;
  }
#line 1957 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 59: /* TARGET_PROFILE: "vs_6_1"  */
#line 395 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::VS_6_1;
  }
#line 1965 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 60: /* TARGET_PROFILE: "vs_6_2"  */
#line 398 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::VS_6_2;
  }
#line 1973 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 61: /* TARGET_PROFILE: "vs_6_3"  */
#line 401 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::VS_6_3;
  }
#line 1981 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 62: /* TARGET_PROFILE: "vs_6_4"  */
#line 404 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::VS_6_4;
  }
#line 1989 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 63: /* TARGET_PROFILE: "vs_6_5"  */
#line 407 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::VS_6_5;
  }
#line 1997 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 64: /* TARGET_PROFILE: "vs_6_6"  */
#line 410 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::VS_6_6;
  }
#line 2005 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 65: /* TARGET_PROFILE: "vs_6_7"  */
#line 413 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::VS_6_7;
  }
#line 2013 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 66: /* TARGET_PROFILE: "ps_6_0"  */
#line 416 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::PS_6_0;
  }
#line 2021 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 67: /* TARGET_PROFILE: "ps_6_1"  */
#line 419 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::PS_6_1;
  }
#line 2029 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 68: /* TARGET_PROFILE: "ps_6_2"  */
#line 422 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::PS_6_2;
  }
#line 2037 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 69: /* TARGET_PROFILE: "ps_6_3"  */
#line 425 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::PS_6_3;
  }
#line 2045 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 70: /* TARGET_PROFILE: "ps_6_4"  */
#line 428 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::PS_6_4;
  }
#line 2053 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 71: /* TARGET_PROFILE: "ps_6_5"  */
#line 431 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::PS_6_5;
  }
#line 2061 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 72: /* TARGET_PROFILE: "ps_6_6"  */
#line 434 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::PS_6_6;
  }
#line 2069 "src/shaders_compiler/parser/parser.tab.cpp"
    break;

  case 73: /* TARGET_PROFILE: "ps_6_7"  */
#line 437 "src/shaders_compiler/parser/parser.y"
             {
    (yyval.targetProfile) = TargetProfile::PS_6_7;
  }
#line 2077 "src/shaders_compiler/parser/parser.tab.cpp"
    break;


#line 2081 "src/shaders_compiler/parser/parser.tab.cpp"

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

#line 441 "src/shaders_compiler/parser/parser.y"


void sherror(Compiler& compiler, const char* msg) {
  logerror("[{}] shader technique parsing error: {} {}", "<todo>", shlineno, msg);
}
