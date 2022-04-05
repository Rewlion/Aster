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




/* First part of user prologue.  */
#line 2 "src/engine/datablock/parser/parser.y"

  #include "ast.h"
  #include <glm/glm.hpp>
  #include <engine/log.h>

// Declare stuff from Flex that Bison needs to know about:
  extern int yylex();
  extern int yyparse(Ast::Config* rootAst);
  extern void yyerror(Ast::Config* rootAst, const char* msg);
  extern FILE *yyin;
  extern int yylineno;
  extern int columno;
  extern char* yytext;

#line 86 "src/engine/datablock/parser/parser.tab.cpp"

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
  YYSYMBOL_EQUAL_OP = 10,                  /* "="  */
  YYSYMBOL_LEFT_PARENTHESIS = 11,          /* "("  */
  YYSYMBOL_RIGHT_PARENTHESIS = 12,         /* ")"  */
  YYSYMBOL_LEFT_BRACKET = 13,              /* "{"  */
  YYSYMBOL_RIGHT_BRACKET = 14,             /* "}"  */
  YYSYMBOL_LEFT_SQUARE_BRACKET = 15,       /* "["  */
  YYSYMBOL_RIGHT_SQUARE_BRACKET = 16,      /* "]"  */
  YYSYMBOL_COMMA = 17,                     /* ","  */
  YYSYMBOL_INT_TYPE = 18,                  /* INT_TYPE  */
  YYSYMBOL_FLOAT_TYPE = 19,                /* FLOAT_TYPE  */
  YYSYMBOL_TEXT_TYPE = 20,                 /* TEXT_TYPE  */
  YYSYMBOL_POINT_2D_TYPE = 21,             /* POINT_2D_TYPE  */
  YYSYMBOL_POINT_3D_TYPE = 22,             /* POINT_3D_TYPE  */
  YYSYMBOL_POINT_4D_TYPE = 23,             /* POINT_4D_TYPE  */
  YYSYMBOL_INT_POINT_2D_TYPE = 24,         /* INT_POINT_2D_TYPE  */
  YYSYMBOL_INT_POINT_3D_TYPE = 25,         /* INT_POINT_3D_TYPE  */
  YYSYMBOL_INT_POINT_4D_TYPE = 26,         /* INT_POINT_4D_TYPE  */
  YYSYMBOL_MAT4_TYPE = 27,                 /* MAT4_TYPE  */
  YYSYMBOL_MAT3_TYPE = 28,                 /* MAT3_TYPE  */
  YYSYMBOL_BOOL_TYPE = 29,                 /* BOOL_TYPE  */
  YYSYMBOL_YYACCEPT = 30,                  /* $accept  */
  YYSYMBOL_CONFIG = 31,                    /* CONFIG  */
  YYSYMBOL_PARAM_LIST = 32,                /* PARAM_LIST  */
  YYSYMBOL_ANNOTATED_PARAM = 33,           /* ANNOTATED_PARAM  */
  YYSYMBOL_BLOCK = 34,                     /* BLOCK  */
  YYSYMBOL_ATTRIBUTE = 35,                 /* ATTRIBUTE  */
  YYSYMBOL_PARAM_NAME = 36,                /* PARAM_NAME  */
  YYSYMBOL_ATTRIBUTE_TYPE = 37,            /* ATTRIBUTE_TYPE  */
  YYSYMBOL_ATTRIBUTE_VALUE = 38,           /* ATTRIBUTE_VALUE  */
  YYSYMBOL_ROW3 = 39,                      /* ROW3  */
  YYSYMBOL_ROW4 = 40                       /* ROW4  */
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
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

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
#define YYFINAL  21
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   107

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  30
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  11
/* YYNRULES -- Number of rules.  */
#define YYNRULES  39
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  93

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   284


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
      25,    26,    27,    28,    29
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    95,    95,   101,   105,   111,   116,   120,   124,   130,
     134,   140,   167,   168,   172,   175,   178,   181,   184,   187,
     190,   193,   196,   199,   202,   205,   211,   215,   219,   223,
     227,   231,   235,   239,   243,   247,   251,   255,   262,   268
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
  "FLOAT_VAL", "TEXT_VAL", "NAME_VAL", "@", ":", "=", "(", ")", "{", "}",
  "[", "]", ",", "INT_TYPE", "FLOAT_TYPE", "TEXT_TYPE", "POINT_2D_TYPE",
  "POINT_3D_TYPE", "POINT_4D_TYPE", "INT_POINT_2D_TYPE",
  "INT_POINT_3D_TYPE", "INT_POINT_4D_TYPE", "MAT4_TYPE", "MAT3_TYPE",
  "BOOL_TYPE", "$accept", "CONFIG", "PARAM_LIST", "ANNOTATED_PARAM",
  "BLOCK", "ATTRIBUTE", "PARAM_NAME", "ATTRIBUTE_TYPE", "ATTRIBUTE_VALUE",
  "ROW3", "ROW4", YY_NULLPTR
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
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284
};
#endif

#define YYPACT_NINF (-55)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      23,   -55,   -55,   -55,   -55,   -55,   -55,   -55,   -55,   -55,
     -55,   -55,   -55,   -55,   -55,     8,   -55,    23,     2,    -6,
      -1,   -55,   -55,    20,    21,    -3,   -55,    24,    27,    29,
     -55,    22,    50,    25,    26,   -55,   -55,    40,    41,   -55,
      44,   -55,   -55,    47,    35,    56,    57,    46,    49,   -55,
      51,    52,    53,    58,    59,    60,    62,    66,    67,    61,
      70,    63,    64,    65,    68,    69,    58,    71,    59,    72,
      74,    78,    79,    73,    82,    75,   -55,   -55,    -4,    76,
     -55,    77,    59,   -55,    85,    86,    90,    80,    81,    83,
      84,   -55,   -55
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    13,    12,    14,    18,    25,    19,    20,    21,    15,
      16,    17,    23,    22,    24,     0,     2,     4,     8,     0,
       0,     1,     3,     0,     0,     0,     6,     0,     0,     0,
      10,     0,     0,     0,     0,     9,    35,    31,    27,    26,
       0,    11,     7,     0,     0,     0,     0,     0,     0,     5,
      32,    28,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    33,    29,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    34,    30,     0,     0,
      36,     0,     0,    38,     0,     0,     0,     0,     0,     0,
       0,    37,    39
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -55,   -55,   -16,   -55,    34,   -55,    87,   -55,   -55,   -26,
     -54
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    15,    16,    17,    26,    18,    19,    20,    41,    47,
      48
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      61,    22,    24,     1,     2,     1,     2,    25,    21,    31,
      23,    30,    83,    84,    75,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    59,    87,     1,
       2,    28,    29,    33,    32,    34,    35,    42,    43,    50,
      73,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    36,    37,    38,    39,    44,    45,    46,
      25,    51,    52,    53,    62,    40,    54,    63,    55,    56,
      57,    64,    65,    58,    60,    67,    76,    49,    66,    77,
      68,    69,    70,    78,    79,    71,    72,    81,    74,    80,
      88,    89,    82,    85,    86,    90,    91,    92,     0,    83,
       0,    84,     0,     0,     0,     0,     0,    27
};

static const yytype_int8 yycheck[] =
{
      54,    17,     8,     6,     7,     6,     7,    13,     0,    25,
       8,    14,    16,    17,    68,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    53,    82,     6,
       7,    11,    11,     6,    10,     6,    14,    12,    12,     4,
      66,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,     3,     4,     5,     6,    17,    17,    15,
      13,     5,     5,    17,     4,    15,    17,     5,    17,    17,
      17,     5,     5,    15,    15,     5,     4,    43,    17,     5,
      17,    17,    17,     5,     5,    17,    17,     5,    17,    16,
       5,     5,    17,    17,    17,     5,    16,    16,    -1,    16,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    20
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     6,     7,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    31,    32,    33,    35,    36,
      37,     0,    32,     8,     8,    13,    34,    36,    11,    11,
      14,    32,    10,     6,     6,    14,     3,     4,     5,     6,
      15,    38,    12,    12,    17,    17,    15,    39,    40,    34,
       4,     5,     5,    17,    17,    17,    17,    17,    15,    39,
      15,    40,     4,     5,     5,     5,    17,     5,    17,    17,
      17,    17,    17,    39,    17,    40,     4,     5,     5,     5,
      16,     5,    17,    16,    17,    17,    17,    40,     5,     5,
       5,    16,    16
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    30,    31,    32,    32,    33,    33,    33,    33,    34,
      34,    35,    36,    36,    37,    37,    37,    37,    37,    37,
      37,    37,    37,    37,    37,    37,    38,    38,    38,    38,
      38,    38,    38,    38,    38,    38,    38,    38,    39,    40
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     6,     2,     5,     1,     3,
       2,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     5,
       7,     1,     3,     5,     7,     1,     7,     9,     7,     9
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (rootAst, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

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
                  Kind, Value, rootAst); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, Ast::Config* rootAst)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (rootAst);
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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, Ast::Config* rootAst)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep, rootAst);
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
                 int yyrule, Ast::Config* rootAst)
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
                       &yyvsp[(yyi + 1) - (yynrhs)], rootAst);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, rootAst); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


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
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, Ast::Config* rootAst)
{
  YYUSE (yyvaluep);
  YYUSE (rootAst);
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
yyparse (Ast::Config* rootAst)
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

  yychar = YYEMPTY; /* Cause a token to be read.  */
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
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
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
  yychar = YYEMPTY;
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
  case 2: /* CONFIG: PARAM_LIST  */
#line 95 "src/engine/datablock/parser/parser.y"
                       {
    rootAst->paramList = (yyvsp[0].paramListNode);
  }
#line 1385 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 3: /* PARAM_LIST: ANNOTATED_PARAM PARAM_LIST  */
#line 101 "src/engine/datablock/parser/parser.y"
                                        {
    (yyval.paramListNode) = (yyvsp[-1].paramListNode);
    (yyval.paramListNode)->next = (yyvsp[0].paramListNode);
  }
#line 1394 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 4: /* PARAM_LIST: ANNOTATED_PARAM  */
#line 105 "src/engine/datablock/parser/parser.y"
                       {
    (yyval.paramListNode) = (yyvsp[0].paramListNode);
  }
#line 1402 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 5: /* ANNOTATED_PARAM: PARAM_NAME "@" "(" TEXT_VAL ")" BLOCK  */
#line 111 "src/engine/datablock/parser/parser.y"
                                                          {
    (yyval.paramListNode) = (yyvsp[0].blockNode);
    (yyval.paramListNode)->name = (yyvsp[-5].sval); free((yyvsp[-5].sval));
    (yyval.paramListNode)->annotation = (yyvsp[-2].sval);
  }
#line 1412 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 6: /* ANNOTATED_PARAM: PARAM_NAME BLOCK  */
#line 116 "src/engine/datablock/parser/parser.y"
                                  {
    (yyval.paramListNode) = (yyvsp[0].blockNode);
    (yyval.paramListNode)->name = (yyvsp[-1].sval); free((yyvsp[-1].sval));
  }
#line 1421 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 7: /* ANNOTATED_PARAM: ATTRIBUTE "@" "(" TEXT_VAL ")"  */
#line 120 "src/engine/datablock/parser/parser.y"
                                            {
    (yyval.paramListNode) = (yyvsp[-4].attributeNode);
    (yyval.paramListNode)->annotation = (yyvsp[-1].sval);
  }
#line 1430 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 8: /* ANNOTATED_PARAM: ATTRIBUTE  */
#line 124 "src/engine/datablock/parser/parser.y"
                    {
    (yyval.paramListNode) = (yyvsp[0].attributeNode);
  }
#line 1438 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 9: /* BLOCK: "{" PARAM_LIST "}"  */
#line 130 "src/engine/datablock/parser/parser.y"
                               {
    (yyval.blockNode) = new Ast::Block;
    (yyval.blockNode)->paramList = (yyvsp[-1].paramListNode);
  }
#line 1447 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 10: /* BLOCK: "{" "}"  */
#line 134 "src/engine/datablock/parser/parser.y"
            {
    (yyval.blockNode) = new Ast::Block;
  }
#line 1455 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 11: /* ATTRIBUTE: ATTRIBUTE_TYPE PARAM_NAME "=" ATTRIBUTE_VALUE  */
#line 140 "src/engine/datablock/parser/parser.y"
                                                                     {
    auto valueType = (yyvsp[0].attributeValue)->GetType();
    if ( (yyvsp[-3].attributeType) == valueType )
    {
      (yyval.attributeNode) = new Ast::Attribute;
      (yyval.attributeNode)->valueNode = (yyvsp[0].attributeValue);
      (yyval.attributeNode)->name = (yyvsp[-2].sval); free((yyvsp[-2].sval));
      (yyval.attributeNode)->type = (yyvsp[-3].attributeType);
    }
    else
    {
      auto expectedTypeStr = Ast::GetAttributeTypeStr((yyvsp[-3].attributeType));
      auto valueTypeStr = Ast::GetAttributeTypeStr(valueType);
      auto valueStr = (yyvsp[0].attributeValue)->ToStr();

      char buffer [100];
      std::snprintf(buffer, 100,"%s=%s  error: value has wrong type `%s`, `%s` expected",
      expectedTypeStr,  valueStr.c_str(),
      valueTypeStr, expectedTypeStr
      );

      yyerror(nullptr, buffer);
    }
  }
#line 1484 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 12: /* PARAM_NAME: NAME_VAL  */
#line 167 "src/engine/datablock/parser/parser.y"
                   { (yyval.sval) = (yyvsp[0].sval); }
#line 1490 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 13: /* PARAM_NAME: TEXT_VAL  */
#line 168 "src/engine/datablock/parser/parser.y"
                   { (yyval.sval) = (yyvsp[0].sval); }
#line 1496 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 14: /* ATTRIBUTE_TYPE: INT_TYPE  */
#line 172 "src/engine/datablock/parser/parser.y"
             {
    (yyval.attributeType) = Ast::AttributeType::Int;
  }
#line 1504 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 15: /* ATTRIBUTE_TYPE: INT_POINT_2D_TYPE  */
#line 175 "src/engine/datablock/parser/parser.y"
                      {
    (yyval.attributeType) = Ast::AttributeType::Int2;
  }
#line 1512 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 16: /* ATTRIBUTE_TYPE: INT_POINT_3D_TYPE  */
#line 178 "src/engine/datablock/parser/parser.y"
                      {
    (yyval.attributeType) = Ast::AttributeType::Int3;
  }
#line 1520 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 17: /* ATTRIBUTE_TYPE: INT_POINT_4D_TYPE  */
#line 181 "src/engine/datablock/parser/parser.y"
                      {
    (yyval.attributeType) = Ast::AttributeType::Int4;
  }
#line 1528 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 18: /* ATTRIBUTE_TYPE: FLOAT_TYPE  */
#line 184 "src/engine/datablock/parser/parser.y"
               {
    (yyval.attributeType) = Ast::AttributeType::Float;
  }
#line 1536 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 19: /* ATTRIBUTE_TYPE: POINT_2D_TYPE  */
#line 187 "src/engine/datablock/parser/parser.y"
                  {
    (yyval.attributeType) = Ast::AttributeType::Float2;
  }
#line 1544 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 20: /* ATTRIBUTE_TYPE: POINT_3D_TYPE  */
#line 190 "src/engine/datablock/parser/parser.y"
                  {
    (yyval.attributeType) = Ast::AttributeType::Float3;
  }
#line 1552 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 21: /* ATTRIBUTE_TYPE: POINT_4D_TYPE  */
#line 193 "src/engine/datablock/parser/parser.y"
                  {
    (yyval.attributeType) = Ast::AttributeType::Float4;
  }
#line 1560 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 22: /* ATTRIBUTE_TYPE: MAT3_TYPE  */
#line 196 "src/engine/datablock/parser/parser.y"
              {
    (yyval.attributeType) = Ast::AttributeType::Mat3;
  }
#line 1568 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 23: /* ATTRIBUTE_TYPE: MAT4_TYPE  */
#line 199 "src/engine/datablock/parser/parser.y"
              {
    (yyval.attributeType) = Ast::AttributeType::Mat4;
  }
#line 1576 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 24: /* ATTRIBUTE_TYPE: BOOL_TYPE  */
#line 202 "src/engine/datablock/parser/parser.y"
              {
    (yyval.attributeType) = Ast::AttributeType::Bool;
  }
#line 1584 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 25: /* ATTRIBUTE_TYPE: TEXT_TYPE  */
#line 205 "src/engine/datablock/parser/parser.y"
              {
    (yyval.attributeType) = Ast::AttributeType::Text;
  }
#line 1592 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 26: /* ATTRIBUTE_VALUE: TEXT_VAL  */
#line 211 "src/engine/datablock/parser/parser.y"
                {
    auto v = new Ast::TextValue; v->value = std::string((yyvsp[0].sval)); free((yyvsp[0].sval));
    (yyval.attributeValue) = v;
  }
#line 1601 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 27: /* ATTRIBUTE_VALUE: FLOAT_VAL  */
#line 215 "src/engine/datablock/parser/parser.y"
                 {
    auto v = new Ast::FloatValue; v->value = (yyvsp[0].fval);
    (yyval.attributeValue) = v;
  }
#line 1610 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 28: /* ATTRIBUTE_VALUE: FLOAT_VAL "," FLOAT_VAL  */
#line 219 "src/engine/datablock/parser/parser.y"
                                    {
    auto v = new Ast::Float2Value; v->value = vec2((yyvsp[-2].fval), (yyvsp[0].fval));
    (yyval.attributeValue) = v;
  }
#line 1619 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 29: /* ATTRIBUTE_VALUE: FLOAT_VAL "," FLOAT_VAL "," FLOAT_VAL  */
#line 223 "src/engine/datablock/parser/parser.y"
                                                      {
    auto v = new Ast::Float3Value; v->value = vec3((yyvsp[-4].fval), (yyvsp[-2].fval), (yyvsp[0].fval));
    (yyval.attributeValue) = v;
  }
#line 1628 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 30: /* ATTRIBUTE_VALUE: FLOAT_VAL "," FLOAT_VAL "," FLOAT_VAL "," FLOAT_VAL  */
#line 227 "src/engine/datablock/parser/parser.y"
                                                                        {
    auto v = new Ast::Float4Value; v->value = vec4((yyvsp[-6].fval), (yyvsp[-4].fval), (yyvsp[-2].fval), (yyvsp[0].fval));
    (yyval.attributeValue) = v;
  }
#line 1637 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 31: /* ATTRIBUTE_VALUE: INT_VAL  */
#line 231 "src/engine/datablock/parser/parser.y"
               {
    auto v = new Ast::IntValue; v->value = (yyvsp[0].ival);
    (yyval.attributeValue) = v;
  }
#line 1646 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 32: /* ATTRIBUTE_VALUE: INT_VAL "," INT_VAL  */
#line 235 "src/engine/datablock/parser/parser.y"
                                {
    auto v = new Ast::Int2Value; v->value = ivec2((yyvsp[-2].ival), (yyvsp[0].ival));
    (yyval.attributeValue) = v;
  }
#line 1655 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 33: /* ATTRIBUTE_VALUE: INT_VAL "," INT_VAL "," INT_VAL  */
#line 239 "src/engine/datablock/parser/parser.y"
                                                {
    auto v = new Ast::Int3Value; v->value = ivec3((yyvsp[-4].ival), (yyvsp[-2].ival), (yyvsp[0].ival));
    (yyval.attributeValue) = v;
  }
#line 1664 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 34: /* ATTRIBUTE_VALUE: INT_VAL "," INT_VAL "," INT_VAL "," INT_VAL  */
#line 243 "src/engine/datablock/parser/parser.y"
                                                                {
    auto v = new Ast::Int4Value; v->value = ivec4((yyvsp[-6].ival), (yyvsp[-4].ival), (yyvsp[-2].ival), (yyvsp[0].ival));
    (yyval.attributeValue) = v;
  }
#line 1673 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 35: /* ATTRIBUTE_VALUE: BOOL_VAL  */
#line 247 "src/engine/datablock/parser/parser.y"
                {
    auto v = new Ast::BoolValue; v->value = (yyvsp[0].bval);
    (yyval.attributeValue) = v;
  }
#line 1682 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 36: /* ATTRIBUTE_VALUE: "[" ROW3 "," ROW3 "," ROW3 "]"  */
#line 251 "src/engine/datablock/parser/parser.y"
                                               {
    auto v = new Ast::Mat3Value; v->value = mat3( (yyvsp[-5].f3val), (yyvsp[-3].f3val), (yyvsp[-1].f3val));
    (yyval.attributeValue) = v;
  }
#line 1691 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 37: /* ATTRIBUTE_VALUE: "[" ROW4 "," ROW4 "," ROW4 "," ROW4 "]"  */
#line 255 "src/engine/datablock/parser/parser.y"
                                                            {
    auto v = new Ast::Mat4Value; v->value = mat4( (yyvsp[-7].f4val), (yyvsp[-5].f4val), (yyvsp[-3].f4val), (yyvsp[-1].f4val));
    (yyval.attributeValue) = v;
  }
#line 1700 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 38: /* ROW3: "[" FLOAT_VAL "," FLOAT_VAL "," FLOAT_VAL "]"  */
#line 262 "src/engine/datablock/parser/parser.y"
                                                              {
    (yyval.f3val) = vec3{ (yyvsp[-5].fval), (yyvsp[-3].fval), (yyvsp[-1].fval) };
  }
#line 1708 "src/engine/datablock/parser/parser.tab.cpp"
    break;

  case 39: /* ROW4: "[" FLOAT_VAL "," FLOAT_VAL "," FLOAT_VAL "," FLOAT_VAL "]"  */
#line 268 "src/engine/datablock/parser/parser.y"
                                                                                {
    (yyval.f4val) = vec4{ (yyvsp[-7].fval), (yyvsp[-5].fval), (yyvsp[-3].fval), (yyvsp[-1].fval) };
  }
#line 1716 "src/engine/datablock/parser/parser.tab.cpp"
    break;


#line 1720 "src/engine/datablock/parser/parser.tab.cpp"

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
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
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
        yyerror (rootAst, yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          goto yyexhaustedlab;
      }
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, rootAst);
          yychar = YYEMPTY;
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, rootAst);
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
  yyerror (rootAst, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, rootAst);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, rootAst);
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

#line 273 "src/engine/datablock/parser/parser.y"


void yyerror(Ast::Config* rootAst, const char* msg) {
  logerror("[{}] blk parsing error: {} {}", rootAst->blkFile, yylineno, msg);
  rootAst->isValid = false;
}
