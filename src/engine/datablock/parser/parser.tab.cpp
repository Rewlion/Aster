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
#define YYSTYPE         BKSTYPE
#define YYLTYPE         BKLTYPE
/* Substitute the variable and function names.  */
#define yyparse         bkparse
#define yylex           bklex
#define yyerror         bkerror
#define yydebug         bkdebug
#define yynerrs         bknerrs
#define yylval          bklval
#define yychar          bkchar
#define yylloc          bklloc

/* First part of user prologue.  */
#line 2 "src/engine/datablock/parser/parser.y"

  #include "parser.h"
  
  #include <engine/log.h>
  #include <engine/types.h>

  #include <glm/glm.hpp>
  
  #include <variant>

// Declare stuff from Flex that Bison needs to know about:
  extern int bklex(BlkParser& parser);
  extern int bkparse(BlkParser& parser);
  extern void bkerror(BlkParser& parser, const char* msg);
  extern FILE *bkin;
  extern int bklineno;
  extern char* bktext;

#line 100 "src/engine/datablock/parser/parser.tab.cpp"

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
  YYSYMBOL_BOOL_VAL = 3,                   /* BOOL_VAL  */
  YYSYMBOL_INT_VAL = 4,                    /* INT_VAL  */
  YYSYMBOL_FLOAT_VAL = 5,                  /* FLOAT_VAL  */
  YYSYMBOL_TEXT_VAL = 6,                   /* TEXT_VAL  */
  YYSYMBOL_NAME_VAL = 7,                   /* NAME_VAL  */
  YYSYMBOL_AT = 8,                         /* "@"  */
  YYSYMBOL_COLON = 9,                      /* ":"  */
  YYSYMBOL_SEMICOLON = 10,                 /* ";"  */
  YYSYMBOL_EQUAL_OP = 11,                  /* "="  */
  YYSYMBOL_LEFT_PARENTHESIS = 12,          /* "("  */
  YYSYMBOL_RIGHT_PARENTHESIS = 13,         /* ")"  */
  YYSYMBOL_LEFT_BRACKET = 14,              /* "{"  */
  YYSYMBOL_RIGHT_BRACKET = 15,             /* "}"  */
  YYSYMBOL_LEFT_SQUARE_BRACKET = 16,       /* "["  */
  YYSYMBOL_RIGHT_SQUARE_BRACKET = 17,      /* "]"  */
  YYSYMBOL_COMMA = 18,                     /* ","  */
  YYSYMBOL_TEXT_TYPE = 19,                 /* "text"  */
  YYSYMBOL_FLOAT_TYPE = 20,                /* "float"  */
  YYSYMBOL_POINT_2D_TYPE = 21,             /* "float2"  */
  YYSYMBOL_POINT_3D_TYPE = 22,             /* "float3"  */
  YYSYMBOL_POINT_4D_TYPE = 23,             /* "float4"  */
  YYSYMBOL_INT_TYPE = 24,                  /* "int"  */
  YYSYMBOL_INT_POINT_2D_TYPE = 25,         /* "int2"  */
  YYSYMBOL_INT_POINT_3D_TYPE = 26,         /* "int3"  */
  YYSYMBOL_INT_POINT_4D_TYPE = 27,         /* "int4"  */
  YYSYMBOL_MAT3_TYPE = 28,                 /* "mat3"  */
  YYSYMBOL_MAT4_TYPE = 29,                 /* "mat4"  */
  YYSYMBOL_BOOL_TYPE = 30,                 /* "bool"  */
  YYSYMBOL_YYACCEPT = 31,                  /* $accept  */
  YYSYMBOL_CONFIG = 32,                    /* CONFIG  */
  YYSYMBOL_PARAM_LIST = 33,                /* PARAM_LIST  */
  YYSYMBOL_ANNOTATED_PARAM = 34,           /* ANNOTATED_PARAM  */
  YYSYMBOL_BLOCK = 35,                     /* BLOCK  */
  YYSYMBOL_ATTRIBUTE = 36,                 /* ATTRIBUTE  */
  YYSYMBOL_PARAM_NAME = 37,                /* PARAM_NAME  */
  YYSYMBOL_ATTRIBUTE_TYPE = 38,            /* ATTRIBUTE_TYPE  */
  YYSYMBOL_ATTRIBUTE_VALUE = 39,           /* ATTRIBUTE_VALUE  */
  YYSYMBOL_MAT4_VALUE = 40,                /* MAT4_VALUE  */
  YYSYMBOL_MAT3_VALUE = 41,                /* MAT3_VALUE  */
  YYSYMBOL_NUMBER4_VALUE = 42,             /* NUMBER4_VALUE  */
  YYSYMBOL_NUMBER3_VALUE = 43,             /* NUMBER3_VALUE  */
  YYSYMBOL_NUMBER2_VALUE = 44,             /* NUMBER2_VALUE  */
  YYSYMBOL_NUMBER_VALUE = 45               /* NUMBER_VALUE  */
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
typedef yytype_int8 yy_state_t;

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
         || (defined BKLTYPE_IS_TRIVIAL && BKLTYPE_IS_TRIVIAL \
             && defined BKSTYPE_IS_TRIVIAL && BKSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  21
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   100

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  31
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  15
/* YYNRULES -- Number of rules.  */
#define YYNRULES  40
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  94

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   285


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
      25,    26,    27,    28,    29,    30
};

#if BKDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   229,   229,   237,   241,   249,   255,   260,   265,   271,
     275,   281,   327,   328,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   343,   347,   348,   349,   350,
     351,   352,   353,   354,   358,   370,   381,   391,   400,   408,
     409
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
  "end of file", "error", "invalid token", "BOOL_VAL", "INT_VAL",
  "FLOAT_VAL", "TEXT_VAL", "NAME_VAL", "@", ":", ";", "=", "(", ")", "{",
  "}", "[", "]", ",", "text", "float", "float2", "float3", "float4", "int",
  "int2", "int3", "int4", "mat3", "mat4", "bool", "$accept", "CONFIG",
  "PARAM_LIST", "ANNOTATED_PARAM", "BLOCK", "ATTRIBUTE", "PARAM_NAME",
  "ATTRIBUTE_TYPE", "ATTRIBUTE_VALUE", "MAT4_VALUE", "MAT3_VALUE",
  "NUMBER4_VALUE", "NUMBER3_VALUE", "NUMBER2_VALUE", "NUMBER_VALUE", YY_NULLPTR
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
     285
};
#endif

#define YYPACT_NINF (-40)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      64,   -40,   -40,   -40,   -40,   -40,   -40,   -40,   -40,   -40,
     -40,   -40,   -40,   -40,   -40,    14,   -40,    64,    10,     3,
       6,   -40,   -40,     7,     8,    39,   -40,    11,    17,    21,
     -40,    13,     0,    16,    18,   -40,   -40,   -40,   -40,   -40,
       5,   -40,   -40,   -40,   -40,   -40,   -40,    15,   -40,    20,
      19,    22,    25,     5,   -40,    29,    30,     5,    35,    40,
      41,    35,     5,     5,     5,    37,    55,    56,    58,    59,
       5,    60,     5,    61,     5,   -40,    57,    62,    65,    77,
       5,     5,     5,     5,    79,    37,    80,   -40,    81,   -40,
      66,     5,    83,   -40
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    13,    12,    25,    18,    19,    20,    21,    14,    15,
      16,    17,    22,    23,    24,     0,     2,     4,     8,     0,
       0,     1,     3,     0,     0,     0,     6,     0,     0,     0,
      10,     0,     0,     0,     0,     9,    27,    40,    39,    26,
       0,    11,    33,    32,    31,    30,    29,    28,     7,     0,
       0,     0,     0,     0,     5,     0,     0,     0,    38,     0,
       0,     0,     0,     0,     0,    37,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    36,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    37,     0,    35,
       0,     0,     0,    34
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -40,   -40,   -10,   -40,   -14,   -40,    78,   -40,   -40,   -40,
     -40,   -39,   -38,   -40,   -32
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    15,    16,    17,    26,    18,    19,    20,    41,    42,
      43,    44,    45,    46,    67
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      47,    50,    51,    36,    37,    38,    39,    22,    52,    37,
      38,    24,     1,     2,    21,    31,    40,    25,    23,    28,
      29,    58,    32,    33,    66,    61,    68,    34,    35,    48,
      65,    49,    69,    53,    25,    54,    55,     0,    75,    56,
      77,    84,    79,    57,    86,     1,     2,    59,    60,    85,
      69,    87,    92,    62,    30,    70,    63,    64,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
       1,     2,    71,    80,    72,    73,     0,    74,    76,    78,
      81,    82,    91,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    83,    88,    89,    27,    90,
      93
};

static const yytype_int8 yycheck[] =
{
      32,    40,    40,     3,     4,     5,     6,    17,    40,     4,
       5,     8,     6,     7,     0,    25,    16,    14,     8,    12,
      12,    53,    11,     6,    63,    57,    64,     6,    15,    13,
      62,    13,    64,    18,    14,    49,    17,    -1,    70,    17,
      72,    80,    74,    18,    82,     6,     7,    18,    18,    81,
      82,    83,    91,    18,    15,    18,    16,    16,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
       6,     7,    17,    16,    18,    17,    -1,    18,    18,    18,
      18,    16,    16,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    18,    17,    17,    20,    18,
      17
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     6,     7,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    32,    33,    34,    36,    37,
      38,     0,    33,     8,     8,    14,    35,    37,    12,    12,
      15,    33,    11,     6,     6,    15,     3,     4,     5,     6,
      16,    39,    40,    41,    42,    43,    44,    45,    13,    13,
      42,    43,    45,    18,    35,    17,    17,    18,    45,    18,
      18,    45,    18,    16,    16,    45,    42,    45,    43,    45,
      18,    17,    18,    17,    18,    45,    18,    45,    18,    45,
      16,    18,    16,    18,    42,    45,    43,    45,    17,    17,
      18,    16,    42,    17
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    31,    32,    33,    33,    34,    34,    34,    34,    35,
      35,    36,    37,    37,    38,    38,    38,    38,    38,    38,
      38,    38,    38,    38,    38,    38,    39,    39,    39,    39,
      39,    39,    39,    39,    40,    41,    42,    43,    44,    45,
      45
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     6,     2,     5,     1,     3,
       2,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,    15,    11,     7,     5,     3,     1,
       1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = BKEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == BKEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (parser, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use BKerror or BKUNDEF. */
#define YYERRCODE BKUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if BKDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YY_LOCATION_PRINT
#  if defined BKLTYPE_IS_TRIVIAL && BKLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#   define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

#  else
#   define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#  endif
# endif /* !defined YY_LOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location, parser); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, BlkParser& parser)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (yylocationp);
  YYUSE (parser);
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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, BlkParser& parser)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YY_LOCATION_PRINT (yyo, *yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp, parser);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule, BlkParser& parser)
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
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]), parser);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, parser); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !BKDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !BKDEBUG */


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
  YYLTYPE *yylloc;
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
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, BlkParser& parser)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (parser);
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
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined BKLTYPE_IS_TRIVIAL && BKLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (BlkParser& parser)
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

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = BKEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
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
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
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
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
  if (yychar == BKEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (parser);
    }

  if (yychar <= BKEOF)
    {
      yychar = BKEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == BKerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = BKUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
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
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = BKEMPTY;
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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* CONFIG: PARAM_LIST  */
#line 229 "src/engine/datablock/parser/parser.y"
                     {
    auto& params = std::get<BlockParams>(yyvsp[0]);
    DataBlock bk = block_params_to_bk(std::move(params));
    parser.setBlock(std::move(bk));
  }
#line 1508 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 3: /* PARAM_LIST: ANNOTATED_PARAM PARAM_LIST  */
#line 237 "src/engine/datablock/parser/parser.y"
                                        {
    std::get<BlockParams>(yyvsp[0]).push_back() = std::move(std::get<BlockParam>(yyvsp[-1]));
    yyval = std::move(yyvsp[0]);
  }
#line 1517 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 4: /* PARAM_LIST: ANNOTATED_PARAM  */
#line 241 "src/engine/datablock/parser/parser.y"
                       {
    BlockParams params;
    params.push_back() = std::move(std::get<BlockParam>(yyvsp[0]));
    yyval = params;
  }
#line 1527 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 5: /* ANNOTATED_PARAM: PARAM_NAME "@" "(" TEXT_VAL ")" BLOCK  */
#line 249 "src/engine/datablock/parser/parser.y"
                                                          {
    auto& bk = std::get<DataBlock>(yyvsp[0]);
    parser.setBkName(bk, std::move(std::get<string>(yyvsp[-5])));
    parser.setBkAnnotation(bk, std::move(std::get<string>(yyvsp[-2])));
    yyval = BlockParam{bk};
  }
#line 1538 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 6: /* ANNOTATED_PARAM: PARAM_NAME BLOCK  */
#line 255 "src/engine/datablock/parser/parser.y"
                                  {
    auto& bk = std::get<DataBlock>(yyvsp[0]);
    parser.setBkName(bk, std::move(std::get<string>(yyvsp[-1])));
    yyval = BlockParam{bk};
  }
#line 1548 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 7: /* ANNOTATED_PARAM: ATTRIBUTE "@" "(" TEXT_VAL ")"  */
#line 260 "src/engine/datablock/parser/parser.y"
                                            {
    auto& attribute = std::get<DataBlock::Attribute>(yyvsp[-4]);
    attribute.annotation = std::move(std::get<string>(yyvsp[-1]));
    yyval = BlockParam{std::move(attribute)};
  }
#line 1558 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 8: /* ANNOTATED_PARAM: ATTRIBUTE  */
#line 265 "src/engine/datablock/parser/parser.y"
                    {
    yyval = BlockParam{std::move(std::get<DataBlock::Attribute>(yyvsp[0]))};
  }
#line 1566 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 9: /* BLOCK: "{" PARAM_LIST "}"  */
#line 271 "src/engine/datablock/parser/parser.y"
                             {
    auto& params = std::get<BlockParams>(yyvsp[-1]);
    yyval = block_params_to_bk(std::move(params));
  }
#line 1575 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 10: /* BLOCK: "{" "}"  */
#line 275 "src/engine/datablock/parser/parser.y"
            {
    yyval = DataBlock{};
  }
#line 1583 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 11: /* ATTRIBUTE: ATTRIBUTE_TYPE PARAM_NAME "=" ATTRIBUTE_VALUE  */
#line 281 "src/engine/datablock/parser/parser.y"
                                                                 {
    DataBlock::Attribute attr{
      .name = std::move(std::get<string>(yyvsp[-2])),
      .annotation = "",
      .type = std::get<DataBlock::Attribute::Type>(yyvsp[-3])
    };

    #define AS_IS(v) std::move(v)
    #define ATTRIBUTE_CASE(AttrType, ValueType, Getter)                                        \
      case DataBlock::Attribute::Type::AttrType:                                               \
      {                                                                                        \
        if (auto* value = std::get_if<ValueType>(&yyvsp[0]))                                         \
          attr.as = Getter(*value);                                                            \
        else                                                                                   \
        {                                                                                      \
          auto err = fmt::format("invalid type of attribute value. Expected: {} Provided: {}", \
            #ValueType, bkstype_to_string(yyvsp[0]));                                                \
          bkerror(parser, err.c_str());                                                        \
        }                                                                                      \
        break;                                                                                 \
      }

    auto attrType = std::get<DataBlock::Attribute::Type>(yyvsp[-3]);
    switch (attrType)
    {
      ATTRIBUTE_CASE(Bool,   bool,    AS_IS)
      ATTRIBUTE_CASE(Int,    Number,  number_to_int)
      ATTRIBUTE_CASE(Int2,   Number2, number2_to_int2)
      ATTRIBUTE_CASE(Int3,   Number3, number3_to_int3)
      ATTRIBUTE_CASE(Int4,   Number4, number4_to_int4)
      ATTRIBUTE_CASE(Float,  Number,  number_to_float)
      ATTRIBUTE_CASE(Float2, Number2, number2_to_float2)
      ATTRIBUTE_CASE(Float3, Number3, number3_to_float3)
      ATTRIBUTE_CASE(Float4, Number4, number4_to_float4)
      ATTRIBUTE_CASE(Text,   string,  AS_IS)
      ATTRIBUTE_CASE(Mat3,   mat3,    AS_IS)
      ATTRIBUTE_CASE(Mat4,   mat4,    AS_IS)
    }
    #undef AS_IS
    #undef ATTRIBUTE_CASE

    yyval = attr;
  }
#line 1631 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 12: /* PARAM_NAME: NAME_VAL  */
#line 327 "src/engine/datablock/parser/parser.y"
                   { yyval = std::move(yyvsp[0]); }
#line 1637 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 13: /* PARAM_NAME: TEXT_VAL  */
#line 328 "src/engine/datablock/parser/parser.y"
                   { yyval = std::move(yyvsp[0]); }
#line 1643 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 14: /* ATTRIBUTE_TYPE: "int"  */
#line 332 "src/engine/datablock/parser/parser.y"
             { yyval = DataBlock::Attribute::Type::Int; }
#line 1649 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 15: /* ATTRIBUTE_TYPE: "int2"  */
#line 333 "src/engine/datablock/parser/parser.y"
             { yyval = DataBlock::Attribute::Type::Int2; }
#line 1655 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 16: /* ATTRIBUTE_TYPE: "int3"  */
#line 334 "src/engine/datablock/parser/parser.y"
             { yyval = DataBlock::Attribute::Type::Int3; }
#line 1661 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 17: /* ATTRIBUTE_TYPE: "int4"  */
#line 335 "src/engine/datablock/parser/parser.y"
             { yyval = DataBlock::Attribute::Type::Int4; }
#line 1667 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 18: /* ATTRIBUTE_TYPE: "float"  */
#line 336 "src/engine/datablock/parser/parser.y"
             { yyval = DataBlock::Attribute::Type::Float; }
#line 1673 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 19: /* ATTRIBUTE_TYPE: "float2"  */
#line 337 "src/engine/datablock/parser/parser.y"
             { yyval = DataBlock::Attribute::Type::Float2; }
#line 1679 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 20: /* ATTRIBUTE_TYPE: "float3"  */
#line 338 "src/engine/datablock/parser/parser.y"
             { yyval = DataBlock::Attribute::Type::Float3; }
#line 1685 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 21: /* ATTRIBUTE_TYPE: "float4"  */
#line 339 "src/engine/datablock/parser/parser.y"
             { yyval = DataBlock::Attribute::Type::Float4; }
#line 1691 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 22: /* ATTRIBUTE_TYPE: "mat3"  */
#line 340 "src/engine/datablock/parser/parser.y"
             { yyval = DataBlock::Attribute::Type::Mat3; }
#line 1697 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 23: /* ATTRIBUTE_TYPE: "mat4"  */
#line 341 "src/engine/datablock/parser/parser.y"
             { yyval = DataBlock::Attribute::Type::Mat4; }
#line 1703 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 24: /* ATTRIBUTE_TYPE: "bool"  */
#line 342 "src/engine/datablock/parser/parser.y"
             { yyval = DataBlock::Attribute::Type::Bool; }
#line 1709 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 25: /* ATTRIBUTE_TYPE: "text"  */
#line 343 "src/engine/datablock/parser/parser.y"
             { yyval = DataBlock::Attribute::Type::Text; }
#line 1715 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 26: /* ATTRIBUTE_VALUE: TEXT_VAL  */
#line 347 "src/engine/datablock/parser/parser.y"
                     { yyval = yyvsp[0]; }
#line 1721 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 27: /* ATTRIBUTE_VALUE: BOOL_VAL  */
#line 348 "src/engine/datablock/parser/parser.y"
                     { yyval = yyvsp[0]; }
#line 1727 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 28: /* ATTRIBUTE_VALUE: NUMBER_VALUE  */
#line 349 "src/engine/datablock/parser/parser.y"
                     { yyval = yyvsp[0]; }
#line 1733 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 29: /* ATTRIBUTE_VALUE: NUMBER2_VALUE  */
#line 350 "src/engine/datablock/parser/parser.y"
                     { yyval = yyvsp[0]; }
#line 1739 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 30: /* ATTRIBUTE_VALUE: NUMBER3_VALUE  */
#line 351 "src/engine/datablock/parser/parser.y"
                     { yyval = yyvsp[0]; }
#line 1745 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 31: /* ATTRIBUTE_VALUE: NUMBER4_VALUE  */
#line 352 "src/engine/datablock/parser/parser.y"
                     { yyval = yyvsp[0]; }
#line 1751 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 32: /* ATTRIBUTE_VALUE: MAT3_VALUE  */
#line 353 "src/engine/datablock/parser/parser.y"
                     { yyval = yyvsp[0]; }
#line 1757 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 33: /* ATTRIBUTE_VALUE: MAT4_VALUE  */
#line 354 "src/engine/datablock/parser/parser.y"
                     { yyval = yyvsp[0]; }
#line 1763 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 34: /* MAT4_VALUE: "[" NUMBER4_VALUE "]" "," "[" NUMBER4_VALUE "]" "," "[" NUMBER4_VALUE "]" "," "[" NUMBER4_VALUE "]"  */
#line 358 "src/engine/datablock/parser/parser.y"
                                                                                                                        {
    mat4 m;
    m[0] = number4_to_float4(yyvsp[-13]);
    m[1] = number4_to_float4(yyvsp[-9]);
    m[2] = number4_to_float4(yyvsp[-5]);
    m[3] = number4_to_float4(yyvsp[-1]);

    yyval = m;
  }
#line 1777 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 35: /* MAT3_VALUE: "[" NUMBER3_VALUE "]" "," "[" NUMBER3_VALUE "]" "," "[" NUMBER3_VALUE "]"  */
#line 370 "src/engine/datablock/parser/parser.y"
                                                                                          {
    mat3 m;
    m[0] = number3_to_float3(yyvsp[-9]);
    m[1] = number3_to_float3(yyvsp[-5]);
    m[2] = number3_to_float3(yyvsp[-1]);

    yyval = m;
  }
#line 1790 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 36: /* NUMBER4_VALUE: NUMBER_VALUE "," NUMBER_VALUE "," NUMBER_VALUE "," NUMBER_VALUE  */
#line 381 "src/engine/datablock/parser/parser.y"
                                                                                    {
    yyval = Number4{
      std::get<Number>(yyvsp[-6]),
      std::get<Number>(yyvsp[-4]),
      std::get<Number>(yyvsp[-2]),
      std::get<Number>(yyvsp[0])};
  }
#line 1802 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 37: /* NUMBER3_VALUE: NUMBER_VALUE "," NUMBER_VALUE "," NUMBER_VALUE  */
#line 391 "src/engine/datablock/parser/parser.y"
                                                               {
    yyval = Number3{
      std::get<Number>(yyvsp[-4]),
      std::get<Number>(yyvsp[-2]),
      std::get<Number>(yyvsp[0])};
  }
#line 1813 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 38: /* NUMBER2_VALUE: NUMBER_VALUE "," NUMBER_VALUE  */
#line 400 "src/engine/datablock/parser/parser.y"
                                         {
    yyval = Number2{
      std::get<Number>(yyvsp[-2]),
      std::get<Number>(yyvsp[0])};
  }
#line 1823 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 39: /* NUMBER_VALUE: FLOAT_VAL  */
#line 408 "src/engine/datablock/parser/parser.y"
                 { yyval = yyvsp[0]; }
#line 1829 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 40: /* NUMBER_VALUE: INT_VAL  */
#line 409 "src/engine/datablock/parser/parser.y"
                 { yyval = yyvsp[0]; }
#line 1835 "src/engine/datablock/parser/parser.tab.cpp"
    break;


#line 1839 "src/engine/datablock/parser/parser.tab.cpp"

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
  *++yylsp = yyloc;

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
  yytoken = yychar == BKEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken, &yylloc};
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
        yyerror (parser, yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          goto yyexhaustedlab;
      }
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= BKEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == BKEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, parser);
          yychar = BKEMPTY;
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp, parser);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

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
  yyerror (parser, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
  if (yychar != BKEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, parser);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp, parser);
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

#line 411 "src/engine/datablock/parser/parser.y"


void bkerror(BlkParser& parser, const char* msg) {
  logerror("parsing error: {} [{}:{}]", msg, parser.getCurrentFileName(), bklloc.first_line);
  parser.markParsingFailed();
}
