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
#define YYSTYPE         EDSTYPE
#define YYLTYPE         EDLTYPE
/* Substitute the variable and function names.  */
#define yyparse         edparse
#define yylex           edlex
#define yyerror         ederror
#define yydebug         eddebug
#define yynerrs         ednerrs
#define yylval          edlval
#define yychar          edchar
#define yylloc          edlloc

/* First part of user prologue.  */
#line 2 "src/engine/data/parser/parser.y"

  #include "parser.h"
  
  #include <engine/log.h>
  #include <engine/types.h>

  #include <glm/glm.hpp>
  
  #include <variant>

// Declare stuff from Flex that Bison needs to know about:
  extern int edlex(ed::Parser& parser);
  extern int edparse(ed::Parser& parser);
  extern void ederror(ed::Parser& parser, const char* msg);
  extern FILE *edin;
  extern char* edtext;

#line 99 "src/engine/data/parser/parser.tab.cpp"

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
  YYSYMBOL_YYACCEPT = 18,                  /* $accept  */
  YYSYMBOL_ENGINE_DATA = 19,               /* ENGINE_DATA  */
  YYSYMBOL_PARAM_LIST = 20,                /* PARAM_LIST  */
  YYSYMBOL_ANNOTATED_PARAM = 21,           /* ANNOTATED_PARAM  */
  YYSYMBOL_ANNOTATED_SCOPE = 22,           /* ANNOTATED_SCOPE  */
  YYSYMBOL_SCOPE_BODY = 23,                /* SCOPE_BODY  */
  YYSYMBOL_ANNOTATED_VARIABLE = 24,        /* ANNOTATED_VARIABLE  */
  YYSYMBOL_VARIABLE = 25,                  /* VARIABLE  */
  YYSYMBOL_ANNOTATION = 26,                /* ANNOTATION  */
  YYSYMBOL_PARAM_NAME = 27,                /* PARAM_NAME  */
  YYSYMBOL_VALUE = 28,                     /* VALUE  */
  YYSYMBOL_MAT4_VALUE = 29,                /* MAT4_VALUE  */
  YYSYMBOL_MAT3_VALUE = 30,                /* MAT3_VALUE  */
  YYSYMBOL_ROW4_VALUE = 31,                /* ROW4_VALUE  */
  YYSYMBOL_ROW3_VALUE = 32,                /* ROW3_VALUE  */
  YYSYMBOL_NUMBER4_VALUE = 33,             /* NUMBER4_VALUE  */
  YYSYMBOL_NUMBER3_VALUE = 34,             /* NUMBER3_VALUE  */
  YYSYMBOL_NUMBER2_VALUE = 35,             /* NUMBER2_VALUE  */
  YYSYMBOL_NUMBER_VALUE = 36,              /* NUMBER_VALUE  */
  YYSYMBOL_ARRAY_VALUE = 37,               /* ARRAY_VALUE  */
  YYSYMBOL_NUMBER_LIST = 38,               /* NUMBER_LIST  */
  YYSYMBOL_TEXT_LIST = 39                  /* TEXT_LIST  */
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
         || (defined EDLTYPE_IS_TRIVIAL && EDLTYPE_IS_TRIVIAL \
             && defined EDSTYPE_IS_TRIVIAL && EDSTYPE_IS_TRIVIAL)))

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
#define YYFINAL  10
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   88

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  18
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  22
/* YYNRULES -- Number of rules.  */
#define YYNRULES  42
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  95

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   272


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
      15,    16,    17
};

#if EDDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   248,   248,   256,   261,   269,   272,   278,   284,   292,
     296,   302,   307,   313,   366,   396,   402,   403,   407,   408,
     409,   410,   411,   412,   413,   414,   415,   416,   420,   436,
     451,   457,   463,   476,   488,   499,   500,   504,   507,   513,
     518,   526,   531
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
  "[", "]", ",", "$accept", "ENGINE_DATA", "PARAM_LIST", "ANNOTATED_PARAM",
  "ANNOTATED_SCOPE", "SCOPE_BODY", "ANNOTATED_VARIABLE", "VARIABLE",
  "ANNOTATION", "PARAM_NAME", "VALUE", "MAT4_VALUE", "MAT3_VALUE",
  "ROW4_VALUE", "ROW3_VALUE", "NUMBER4_VALUE", "NUMBER3_VALUE",
  "NUMBER2_VALUE", "NUMBER_VALUE", "ARRAY_VALUE", "NUMBER_LIST",
  "TEXT_LIST", YY_NULLPTR
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
     265,   266,   267,   268,   269,   270,   271,   272
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
      37,   -55,   -55,    11,    37,   -55,   -55,   -55,     5,    33,
     -55,   -55,    21,   -55,    13,     9,   -55,    22,    52,    38,
     -55,    31,   -55,    40,     4,    34,   -55,   -55,   -55,   -55,
     -55,   -55,    24,   -55,   -55,   -55,   -55,   -55,   -55,   -55,
      39,   -55,    54,   -55,    50,    48,    49,   -55,    44,    46,
      50,     4,    51,    53,    55,    56,    58,   -55,    50,   -55,
      62,    57,   -55,   -55,   -55,    50,    50,    59,    50,    60,
     -55,   -55,    50,    57,    61,    56,    63,    58,    64,    50,
      65,    50,    67,    50,    68,    56,    69,   -55,   -55,    50,
      71,    50,    64,   -55,   -55
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    17,    16,     0,     2,     4,     5,     6,    12,     0,
       1,     3,     0,    11,     0,     0,     8,     0,     0,     0,
      10,     0,     7,     0,     0,     0,     9,    15,    19,    36,
      35,    18,     0,    27,    13,    24,    25,    23,    22,    21,
      20,    26,     0,    42,     0,     0,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    37,     0,    38,
       0,    34,    14,    30,    31,     0,     0,     0,     0,     0,
      39,    41,     0,     0,     0,     0,     0,     0,    33,     0,
       0,     0,     0,     0,     0,     0,     0,    29,    32,     0,
       0,     0,     0,    28,    33
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -55,   -55,    73,     1,   -55,    -3,   -55,   -55,    66,   -55,
      19,   -55,   -55,   -54,   -52,   -42,   -41,   -55,   -32,   -55,
     -55,   -55
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    33,     7,     8,    13,     9,
      34,    35,    36,    45,    46,    37,    38,    39,    40,    41,
      48,    49
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      47,    67,    52,    53,    69,    11,    16,    28,    29,    30,
      31,    10,    54,    12,    22,     1,     2,    15,    61,    32,
      19,    80,    11,    20,    52,    82,    70,    53,    29,    30,
      43,    90,    18,    73,    74,    15,    76,     1,     2,    44,
      78,    12,    14,     1,     2,    26,    15,    84,    24,    86,
      42,    88,    27,    25,    29,    30,    50,    92,    23,    94,
      57,    58,    59,    60,    51,    55,    56,    63,    71,    64,
      62,    66,    65,    68,    72,    17,    75,    77,    79,     0,
      81,    83,    85,    87,     0,    89,    91,    93,    21
};

static const yytype_int8 yycheck[] =
{
      32,    55,    44,    44,    56,     4,     9,     3,     4,     5,
       6,     0,    44,     8,    17,     6,     7,    13,    50,    15,
       7,    75,    21,    14,    66,    77,    58,    68,     4,     5,
       6,    85,    11,    65,    66,    13,    68,     6,     7,    15,
      72,     8,     9,     6,     7,    14,    13,    79,    10,    81,
      16,    83,    12,    15,     4,     5,    17,    89,     6,    91,
      16,    17,    16,    17,    10,    17,    17,    16,     6,    16,
      51,    15,    17,    15,    17,     9,    17,    17,    17,    -1,
      17,    17,    17,    16,    -1,    17,    17,    16,    15
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     6,     7,    19,    20,    21,    22,    24,    25,    27,
       0,    21,     8,    26,     9,    13,    23,    26,    11,     7,
      14,    20,    23,     6,    10,    15,    14,    12,     3,     4,
       5,     6,    15,    23,    28,    29,    30,    33,    34,    35,
      36,    37,    16,     6,    15,    31,    32,    36,    38,    39,
      17,    10,    33,    34,    36,    17,    17,    16,    17,    16,
      17,    36,    28,    16,    16,    17,    15,    31,    15,    32,
      36,     6,    17,    36,    36,    17,    36,    17,    36,    17,
      31,    17,    32,    17,    36,    17,    36,    16,    36,    17,
      31,    17,    36,    16,    36
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    18,    19,    20,    20,    21,    21,    22,    22,    23,
      23,    24,    24,    25,    25,    26,    27,    27,    28,    28,
      28,    28,    28,    28,    28,    28,    28,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    36,    37,    37,    38,
      38,    39,    39
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     3,     2,     3,
       2,     2,     1,     5,     7,     4,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     9,     7,
       3,     3,     7,     5,     3,     1,     1,     3,     3,     3,
       1,     3,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = EDEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == EDEMPTY)                                        \
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
   Use EDerror or EDUNDEF. */
#define YYERRCODE EDUNDEF

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
#if EDDEBUG

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
#  if defined EDLTYPE_IS_TRIVIAL && EDLTYPE_IS_TRIVIAL

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
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, ed::Parser& parser)
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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, ed::Parser& parser)
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
                 int yyrule, ed::Parser& parser)
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
#else /* !EDDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !EDDEBUG */


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
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, ed::Parser& parser)
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
# if defined EDLTYPE_IS_TRIVIAL && EDLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (ed::Parser& parser)
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

  yychar = EDEMPTY; /* Cause a token to be read.  */
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
  if (yychar == EDEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (parser);
    }

  if (yychar <= EDEOF)
    {
      yychar = EDEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == EDerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = EDUNDEF;
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
  yychar = EDEMPTY;
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
  case 2: /* ENGINE_DATA: PARAM_LIST  */
#line 248 "src/engine/data/parser/parser.y"
                     {
    auto& params = std::get<ScopeParamArrayNodePtr>(yyvsp[0]);
    ScopeNodePtr scope = scope_params_to_scope(parser, params->val);
    parser.setMainScope(std::move(scope->val));
  }
#line 1497 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 3: /* PARAM_LIST: PARAM_LIST ANNOTATED_PARAM  */
#line 256 "src/engine/data/parser/parser.y"
                                        {
    auto l = std::get<ScopeParamArrayNodePtr>(yyvsp[-1]);
    l->val.push_back(std::get<ScopeParam>(yyvsp[0]));
    yyval = l;
  }
#line 1507 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 4: /* PARAM_LIST: ANNOTATED_PARAM  */
#line 261 "src/engine/data/parser/parser.y"
                       {
    auto params = ScopeParamArrayNode::make({});
    params->val.push_back((std::get<ScopeParam>(yyvsp[0])));
    yyval = params;
  }
#line 1517 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 5: /* ANNOTATED_PARAM: ANNOTATED_SCOPE  */
#line 269 "src/engine/data/parser/parser.y"
                           {
    yyval = yyvsp[0];
  }
#line 1525 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 6: /* ANNOTATED_PARAM: ANNOTATED_VARIABLE  */
#line 272 "src/engine/data/parser/parser.y"
                                 {
    yyval = yyvsp[0];
  }
#line 1533 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 7: /* ANNOTATED_SCOPE: PARAM_NAME ANNOTATION SCOPE_BODY  */
#line 278 "src/engine/data/parser/parser.y"
                                                    {
    auto& scope = std::get<ScopeNodePtr>(yyvsp[0]);
    parser.setScopeName(scope->val, std::move(std::get<string>(yyvsp[-2])));
    parser.setScopeAnnotation(scope->val, std::move(std::get<string>(yyvsp[-1])));
    yyval = ScopeParam{scope};
  }
#line 1544 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 8: /* ANNOTATED_SCOPE: PARAM_NAME SCOPE_BODY  */
#line 284 "src/engine/data/parser/parser.y"
                                      {
    auto& scope = std::get<ScopeNodePtr>(yyvsp[0]);
    parser.setScopeName(scope->val, std::move(std::get<string>(yyvsp[-1])));
    yyval = ScopeParam{scope};
  }
#line 1554 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 9: /* SCOPE_BODY: "{" PARAM_LIST "}"  */
#line 292 "src/engine/data/parser/parser.y"
                             {
    auto& params = std::get<ScopeParamArrayNodePtr>(yyvsp[-1]);
    yyval = scope_params_to_scope(parser, params->val);
  }
#line 1563 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 10: /* SCOPE_BODY: "{" "}"  */
#line 296 "src/engine/data/parser/parser.y"
            {
    yyval = ScopeNode::make(parser.makeScope());
  }
#line 1571 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 11: /* ANNOTATED_VARIABLE: VARIABLE ANNOTATION  */
#line 302 "src/engine/data/parser/parser.y"
                                {
    auto& var = std::get<VariableNodePtr>(yyvsp[-1]);
    var->val.annotation = std::move(std::get<string>(yyvsp[0]));
    yyval = ScopeParam{var};
  }
#line 1581 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 12: /* ANNOTATED_VARIABLE: VARIABLE  */
#line 307 "src/engine/data/parser/parser.y"
                  {
    yyval = ScopeParam{std::get<VariableNodePtr>(yyvsp[0])};
  }
#line 1589 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 13: /* VARIABLE: PARAM_NAME ":" NAME_VAL "=" VALUE  */
#line 313 "src/engine/data/parser/parser.y"
                                                          {
    const string& type = std::get<string>(yyvsp[-2]);

    ed::Variable var;
    var.name = std::move(std::get<string>(yyvsp[-4]));
    var.annotation = "";

    try
    {
      #define CASE_VAR(textType, valueType, getter)             \
        if (type == textType)                               \
        {                                                   \
          if (auto* value = std::get_if<valueType>(&yyvsp[0]))\
            var.value = getter(*value);                    \
          else                                              \
            throw std::runtime_error("invalid value type"); \
        }
      #define NEXT else
      #define AS_IS(var) var
      #define AS_IS_DEREF(ptr) (*ptr).val

           CASE_VAR("b",  bool,            AS_IS)
      NEXT CASE_VAR("i",  Number,          number_to_int)
      NEXT CASE_VAR("i2", Number2NodePtr,  number2_to_int2)
      NEXT CASE_VAR("i3", Number3NodePtr,  number3_to_int3)
      NEXT CASE_VAR("i4", Number4NodePtr,  number4_to_int4)
      NEXT CASE_VAR("f",  Number,          number_to_float)
      NEXT CASE_VAR("f2", Number2NodePtr,  number2_to_float2)
      NEXT CASE_VAR("f3", Number3NodePtr,  number3_to_float3)
      NEXT CASE_VAR("f4", Number4NodePtr,  number4_to_float4)
      NEXT CASE_VAR("m3", Mat3NodePtr,     AS_IS_DEREF)
      NEXT CASE_VAR("m4", Mat4NodePtr,     AS_IS_DEREF)
      NEXT CASE_VAR("t",  string,          AS_IS)
      NEXT {
        if (auto* scope = std::get_if<ScopeNodePtr>(&yyvsp[0]))
        {
          ed::TypeConstructor tc = parser.buildTypeConstructor(type, std::move((*scope)->val));
          if (tc.typeId != INVALID_ED_TYPE_ID)
            var.value = std::move(tc);
          else
            throw std::runtime_error(fmt::format("unknown custom type", var.name, type));
        }
        else
          throw std::runtime_error(fmt::format("invalid value type for the custom type. Custom type has to be initialized with scope i.e. '{}:{} = {{ }}'", var.name, type));
      }
    }
    catch(const std::runtime_error& e)
    {
      ederror(parser, fmt::format(" error in {}:{}, {} ", var.name, type, e.what()).c_str());
    }

    yyval = VariableNode::make(std::move(var));
  }
#line 1647 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 14: /* VARIABLE: PARAM_NAME ":" NAME_VAL "[" "]" "=" VALUE  */
#line 366 "src/engine/data/parser/parser.y"
                                                                 {
    const auto& type = std::get<string>(yyvsp[-4]);

    ed::Variable var;
    var.name = std::move(std::get<string>(yyvsp[-6]));
    var.annotation = "";
    try
    {
           CASE_VAR("i", NumberArrayNodePtr, number_array_to_int_array)
      NEXT CASE_VAR("f", NumberArrayNodePtr, number_array_to_float_array)
      NEXT CASE_VAR("t", TextArrayNodePtr,   AS_IS_DEREF)
      NEXT {
        throw std::runtime_error(string{"unknown array type"});
      }
    }
    catch(const std::runtime_error& e)
    {
      ederror(parser, fmt::format(" error in {}:{}, {} ", var.name, type, e.what()).c_str());
    }

    yyval = VariableNode::make(std::move(var));

    #undef AS_IS_DEREF
    #undef AS_IS
    #undef NEXT
    #undef CASE_VAR
  }
#line 1679 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 15: /* ANNOTATION: "@" "(" TEXT_VAL ")"  */
#line 396 "src/engine/data/parser/parser.y"
                            {
    yyval = std::move(yyvsp[-1]);
  }
#line 1687 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 16: /* PARAM_NAME: NAME_VAL  */
#line 402 "src/engine/data/parser/parser.y"
                   { yyval = std::move(yyvsp[0]); }
#line 1693 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 17: /* PARAM_NAME: TEXT_VAL  */
#line 403 "src/engine/data/parser/parser.y"
                   { yyval = std::move(yyvsp[0]); }
#line 1699 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 18: /* VALUE: TEXT_VAL  */
#line 407 "src/engine/data/parser/parser.y"
                      { yyval = yyvsp[0]; }
#line 1705 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 19: /* VALUE: BOOL_VAL  */
#line 408 "src/engine/data/parser/parser.y"
                      { yyval = yyvsp[0]; }
#line 1711 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 20: /* VALUE: NUMBER_VALUE  */
#line 409 "src/engine/data/parser/parser.y"
                      { yyval = yyvsp[0]; }
#line 1717 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 21: /* VALUE: NUMBER2_VALUE  */
#line 410 "src/engine/data/parser/parser.y"
                      { yyval = yyvsp[0]; }
#line 1723 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 22: /* VALUE: NUMBER3_VALUE  */
#line 411 "src/engine/data/parser/parser.y"
                      { yyval = yyvsp[0]; }
#line 1729 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 23: /* VALUE: NUMBER4_VALUE  */
#line 412 "src/engine/data/parser/parser.y"
                      { yyval = yyvsp[0]; }
#line 1735 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 24: /* VALUE: MAT4_VALUE  */
#line 413 "src/engine/data/parser/parser.y"
                      { yyval = yyvsp[0]; }
#line 1741 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 25: /* VALUE: MAT3_VALUE  */
#line 414 "src/engine/data/parser/parser.y"
                      { yyval = yyvsp[0]; }
#line 1747 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 26: /* VALUE: ARRAY_VALUE  */
#line 415 "src/engine/data/parser/parser.y"
                      { yyval = yyvsp[0]; }
#line 1753 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 27: /* VALUE: SCOPE_BODY  */
#line 416 "src/engine/data/parser/parser.y"
                      { yyval = yyvsp[0]; }
#line 1759 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 28: /* MAT4_VALUE: "[" ROW4_VALUE "," ROW4_VALUE "," ROW4_VALUE "," ROW4_VALUE "]"  */
#line 420 "src/engine/data/parser/parser.y"
                                                                                    {
    float4 rows[4];
    rows[0] = number4_to_float4(yyvsp[-7]);
    rows[1] = number4_to_float4(yyvsp[-5]);
    rows[2] = number4_to_float4(yyvsp[-3]);
    rows[3] = number4_to_float4(yyvsp[-1]);

    float4 columns[4];
    for (int i = 0; i < 4; ++i)
      for (int j = 0; j < 4; ++j)
        columns[i][j] = rows[j][i];

    yyval = Mat4Node::make(float4x4{columns[0], columns[1], columns[2], columns[3]});
  }
#line 1778 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 29: /* MAT3_VALUE: "[" ROW3_VALUE "," ROW3_VALUE "," ROW3_VALUE "]"  */
#line 436 "src/engine/data/parser/parser.y"
                                                                 {
    float3 rows[4];
    rows[0] = number3_to_float3(yyvsp[-5]);
    rows[1] = number3_to_float3(yyvsp[-3]);
    rows[2] = number3_to_float3(yyvsp[-1]);

    float3 columns[3];
    for (int i = 0; i < 3; ++i)
      for (int j = 0; j < 3; ++j)
        columns[i][j] = rows[j][i];

    yyval = Mat3Node::make(float3x3{columns[0], columns[1], columns[2]});
  }
#line 1796 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 30: /* ROW4_VALUE: "[" NUMBER4_VALUE "]"  */
#line 451 "src/engine/data/parser/parser.y"
                             {
    yyval = yyvsp[-1];
  }
#line 1804 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 31: /* ROW3_VALUE: "[" NUMBER3_VALUE "]"  */
#line 457 "src/engine/data/parser/parser.y"
                             {
    yyval = yyvsp[-1];
  }
#line 1812 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 32: /* NUMBER4_VALUE: NUMBER_VALUE "," NUMBER_VALUE "," NUMBER_VALUE "," NUMBER_VALUE  */
#line 463 "src/engine/data/parser/parser.y"
                                                                                    {
    yyval = Number4Node::make(
      Number4{
        std::get<Number>(yyvsp[-6]),
        std::get<Number>(yyvsp[-4]),
        std::get<Number>(yyvsp[-2]),
        std::get<Number>(yyvsp[0])
      }
    );
  }
#line 1827 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 33: /* NUMBER3_VALUE: NUMBER_VALUE "," NUMBER_VALUE "," NUMBER_VALUE  */
#line 476 "src/engine/data/parser/parser.y"
                                                               {
    yyval = Number3Node::make(
      Number3{
        std::get<Number>(yyvsp[-4]),
        std::get<Number>(yyvsp[-2]),
        std::get<Number>(yyvsp[0])
      }
    );
  }
#line 1841 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 34: /* NUMBER2_VALUE: NUMBER_VALUE "," NUMBER_VALUE  */
#line 488 "src/engine/data/parser/parser.y"
                                         {
    yyval = Number2Node::make(
      Number2{
        std::get<Number>(yyvsp[-2]),
        std::get<Number>(yyvsp[0])
      }
    );
  }
#line 1854 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 35: /* NUMBER_VALUE: FLOAT_VAL  */
#line 499 "src/engine/data/parser/parser.y"
                 { yyval = yyvsp[0]; }
#line 1860 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 36: /* NUMBER_VALUE: INT_VAL  */
#line 500 "src/engine/data/parser/parser.y"
                 { yyval = yyvsp[0]; }
#line 1866 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 37: /* ARRAY_VALUE: "[" NUMBER_LIST "]"  */
#line 504 "src/engine/data/parser/parser.y"
                           {
    yyval = yyvsp[-1];
  }
#line 1874 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 38: /* ARRAY_VALUE: "[" TEXT_LIST "]"  */
#line 507 "src/engine/data/parser/parser.y"
                         {
    yyval = yyvsp[-1];
  }
#line 1882 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 39: /* NUMBER_LIST: NUMBER_LIST "," NUMBER_VALUE  */
#line 513 "src/engine/data/parser/parser.y"
                                         {
  auto& l = std::get<NumberArrayNodePtr>(yyvsp[-2]);
  l->val.push_back(std::get<Number>(yyvsp[0]));
  yyval = l;
 }
#line 1892 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 40: /* NUMBER_LIST: NUMBER_VALUE  */
#line 518 "src/engine/data/parser/parser.y"
                   {
  auto l = NumberArrayNode::make({});
  l->val.push_back(std::get<Number>(yyvsp[0]));
  yyval = l;
 }
#line 1902 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 41: /* TEXT_LIST: TEXT_LIST "," TEXT_VAL  */
#line 526 "src/engine/data/parser/parser.y"
                                   {
  auto l = std::get<TextArrayNodePtr>(yyvsp[-2]);
  l->val.push_back(std::move(std::get<string>(yyvsp[0])));
  yyval = l;
 }
#line 1912 "src/engine/data/parser/parser.tab.cpp"
    break;

  case 42: /* TEXT_LIST: TEXT_VAL  */
#line 531 "src/engine/data/parser/parser.y"
               {
  auto l = TextArrayNode::make({});
  l->val.push_back(std::move(std::get<string>(yyvsp[0])));
  yyval = l;
 }
#line 1922 "src/engine/data/parser/parser.tab.cpp"
    break;


#line 1926 "src/engine/data/parser/parser.tab.cpp"

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
  yytoken = yychar == EDEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
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

      if (yychar <= EDEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == EDEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, parser);
          yychar = EDEMPTY;
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
  if (yychar != EDEMPTY)
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

#line 537 "src/engine/data/parser/parser.y"


void ederror(ed::Parser& parser, const char* msg) {
  const string err = fmt::format("parsing error: {} [{}:{}]", msg, parser.getCurrentFileName(), parser.getLine());
  parser.markParsingFailed(err);
}
