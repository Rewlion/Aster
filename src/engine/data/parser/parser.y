
%{
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
%}

%code requires  {
  #include "parser.h"

  #include <ranges>
  #include <variant>

  using ScopeParam = std::variant<std::shared_ptr<ed::Scope>, std::shared_ptr<ed::Variable>>;
  using ScopeParamArray = eastl::vector<ScopeParam>;

  static
  auto scope_params_to_scope(ed::Parser& parser, ScopeParamArray& params) -> std::shared_ptr<ed::Scope>
  {
    std::shared_ptr<ed::Scope> scope = parser.makeScope();
    for (ScopeParam& param: params | std::views::reverse)
    {
      if (auto* nestedScope = std::get_if<std::shared_ptr<ed::Scope>>(&param))
        scope->addScope(std::move(**nestedScope));
      else
        parser.addVariableDefinition(std::move(*std::get<std::shared_ptr<ed::Variable>>(param)), *scope);
    }

    return scope;
  }

  using Number = std::variant<int, float>;
  struct Number2
  {
    Number v0, v1;
  };

  struct Number3
  {
    Number v0, v1, v2;
  };

  struct Number4
  {
    Number v0, v1, v2, v3;
  };

  #define EDSTYPE std::variant<                                                                    \
    bool, string,                                                                                  \
    Number, Number2, Number3, Number4,                                                             \
    float3x3, float4x4,                                                                            \
    std::shared_ptr<ed::IntArray>, std::shared_ptr<ed::FloatArray>, std::shared_ptr<ed::TextArray>,\
    std::shared_ptr<eastl::vector<Number>>                                        ,                \
    std::shared_ptr<ed::Scope>, std::shared_ptr<ed::Variable>,                                     \
    ScopeParam, std::shared_ptr<ScopeParamArray>                                                   \
  >

  template<class T>
  static
  auto number_to(const EDSTYPE& edstype) -> T
  {
    const Number& n = std::get<Number>(edstype);
    if (auto* fl = std::get_if<T>(&n))
      return *fl;
    else
    {
      using FromType = typename std::conditional<std::is_same_v<T,int>, float, int>::type;
      return static_cast<T>(std::get<FromType>(n));
    }
  }

  static
  auto number_to_float(const EDSTYPE& edstype) -> float
  {
    const Number& n = std::get<Number>(edstype);
    if (auto* fl = std::get_if<float>(&n))
      return *fl;
    else
      return static_cast<float>(std::get<int>(n));
  }

  static
  auto number_to_int(const EDSTYPE& edstype) -> int
  {
    const Number& n = std::get<Number>(edstype);
    if (auto* v = std::get_if<int>(&n))
      return *v;
    else
      return static_cast<int>(std::get<float>(n));
  }

  static
  auto number2_to_float2(const EDSTYPE& edstype) -> float2
  {
    const Number2& n = std::get<Number2>(edstype);
    return float2{
      number_to_float(n.v0),
      number_to_float(n.v1)
    };
  }

  static
  auto number3_to_float3(const EDSTYPE& edstype) -> float3
  {
    const Number3& n = std::get<Number3>(edstype);
    return float3{
      number_to_float(n.v0),
      number_to_float(n.v1),
      number_to_float(n.v2)
    };
  }

  static
  auto number4_to_float4(const EDSTYPE& edstype) -> float4
  {
    const Number4& n = std::get<Number4>(edstype);
    return float4{
      number_to_float(n.v0),
      number_to_float(n.v1),
      number_to_float(n.v2),
      number_to_float(n.v3)
    };
  }

  static
  auto number2_to_int2(const EDSTYPE& edstype) -> int2
  {
    const Number2& n = std::get<Number2>(edstype);
    return int2{
      number_to_int(n.v0),
      number_to_int(n.v1)
    };
  }

  static
  auto number3_to_int3(const EDSTYPE& edstype) -> int3
  {
    const Number3& n = std::get<Number3>(edstype);
    return int3{
      number_to_int(n.v0),
      number_to_int(n.v1),
      number_to_int(n.v2)
    };
  }

  static
  auto number4_to_int4(const EDSTYPE& edstype) -> int4
  {
    const Number4& n = std::get<Number4>(edstype);
    return int4{
      number_to_int(n.v0),
      number_to_int(n.v1),
      number_to_int(n.v2),
      number_to_int(n.v3)
    };
  }

  template<class T>
  static
  auto number_array_to_T_array(const EDSTYPE& edstype) -> eastl::vector<T>
  {
    const auto& from = std::get<std::shared_ptr<eastl::vector<Number>>>(edstype);
    eastl::vector<T> to;

    to.reserve(from->size());
    for (const Number& n: *from)
      to.push_back(number_to<T>(n));

    return to;
  }

  static
  auto number_array_to_int_array(const EDSTYPE& edstype) -> ed::IntArray
  {
    return number_array_to_T_array<int>(edstype);
  }

  static
  auto number_array_to_float_array(const EDSTYPE& edstype) -> ed::FloatArray
  {
    return number_array_to_T_array<float>(edstype);
  }
}

%locations

%define parse.error detailed
%define api.prefix {ed}

%lex-param {ed::Parser& parser}
%parse-param {ed::Parser& parser}

%token BOOL_VAL
%token INT_VAL
%token FLOAT_VAL
%token TEXT_VAL
%token NAME_VAL
%token AT "@"
%token COLON ":"
%token EQUAL_OP "="
%token LEFT_PARENTHESIS "("
%token RIGHT_PARENTHESIS ")"
%token LEFT_BRACKET "{"
%token RIGHT_BRACKET "}"
%token LEFT_SQUARE_BRACKET "["
%token RIGHT_SQUARE_BRACKET "]"
%token COMMA ","

%%

ENGINE_DATA
  : PARAM_LIST[list] {
    auto& params = std::get<std::shared_ptr<ScopeParamArray>>($list);
    std::shared_ptr<ed::Scope> scope = scope_params_to_scope(parser, *params);
    parser.setMainScope(std::move(*scope));
  }
  ;

PARAM_LIST
  : ANNOTATED_PARAM[p] PARAM_LIST[list] {
    std::get<std::shared_ptr<ScopeParamArray>>($list)->push_back(std::get<ScopeParam>($p));
    $$ = $list;
  }
  | ANNOTATED_PARAM[p] {
    std::shared_ptr<ScopeParamArray> params = std::make_shared<ScopeParamArray>();
    params->push_back((std::get<ScopeParam>($p)));
    $$ = params;
  }
  ;

ANNOTATED_PARAM
  : ANNOTATED_SCOPE[scope] {
    $$ = $scope;
  }
  | ANNOTATED_VARIABLE[variable] {
    $$ = $variable;
  }
  ;

ANNOTATED_SCOPE
  : PARAM_NAME[name] ANNOTATION[a] SCOPE_BODY[body] {
    auto& scope = std::get<std::shared_ptr<ed::Scope>>($body);
    parser.setScopeName(*scope, std::move(std::get<string>($name)));
    parser.setScopeAnnotation(*scope, std::move(std::get<string>($a)));
    $$ = ScopeParam{scope};
  }
  | PARAM_NAME[name] SCOPE_BODY[body] {
    auto& scope = std::get<std::shared_ptr<ed::Scope>>($body);
    parser.setScopeName(*scope, std::move(std::get<string>($name)));
    $$ = ScopeParam{scope};
  }
  ;

SCOPE_BODY
  : "{" PARAM_LIST[list] "}" {
    auto& params = std::get<std::shared_ptr<ScopeParamArray>>($list);
    $$ = scope_params_to_scope(parser, *params);
  }
  | "{" "}" {
    $$ = parser.makeScope();
  }
  ;

ANNOTATED_VARIABLE
  : VARIABLE[var] ANNOTATION[a] {
    auto& var = std::get<std::shared_ptr<ed::Variable>>($var);
    var->annotation = std::move(std::get<string>($a));
    $$ = ScopeParam{var};
  }
  | VARIABLE[var] {
    $$ = ScopeParam{std::get<std::shared_ptr<ed::Variable>>($var)};
  }
  ;

VARIABLE
  : PARAM_NAME[name] ":" NAME_VAL[ntype] "=" VALUE[value] {
    const auto& type = std::get<string>($ntype);

    std::shared_ptr<ed::Variable> var{new ed::Variable};
    var->name = std::move(std::get<string>($name));
    var->annotation = "";

    try
    {
      #define CASE_VAR(textType, valueType, getter)             \
        if (type == textType)                               \
        {                                                   \
          if (auto* value = std::get_if<valueType>(&$value))\
            var->value = getter(*value);                    \
          else                                              \
            throw std::runtime_error("invalid value type"); \
        }
      #define NEXT else
      #define AS_IS(var) var
      #define AS_IS_UNREF(var) *var

           CASE_VAR("b",  bool,     AS_IS)
      NEXT CASE_VAR("i",  Number,   number_to_int)
      NEXT CASE_VAR("i2", Number2,  number2_to_int2)
      NEXT CASE_VAR("i3", Number3,  number3_to_int3)
      NEXT CASE_VAR("i4", Number4,  number4_to_int4)
      NEXT CASE_VAR("f",  Number,   number_to_float)
      NEXT CASE_VAR("f2", Number2,  number2_to_float2)
      NEXT CASE_VAR("f3", Number3,  number3_to_float3)
      NEXT CASE_VAR("f4", Number4,  number4_to_float4)
      NEXT CASE_VAR("m3", float3x3, AS_IS)
      NEXT CASE_VAR("m4", float4x4, AS_IS)
      NEXT CASE_VAR("t",  string,   AS_IS)
      NEXT {
        if (auto* scope = std::get_if<std::shared_ptr<ed::Scope>>(&$value))
        {
          ed::TypeConstructor tc = parser.buildTypeConstructor(type, std::move(**scope));
          if (tc.typeId != INVALID_ED_TYPE_ID)
            var->value = std::move(tc);
          else
            throw std::runtime_error(fmt::format("unknown custom type", var->name, type));
        }
        else
          throw std::runtime_error(fmt::format("invalid value type for the custom type. Custom type has to be initialized with scope i.e. '{}:{} = {{ }}'", var->name, type));
      }
    }
    catch(const std::runtime_error& e)
    {
      ederror(parser, fmt::format(" error in {}:{}, {} ", var->name, type, e.what()).c_str());
    }

    $$ = var;
  }
  | PARAM_NAME[name] ":" NAME_VAL[ntype] "[""]" "=" VALUE[value] {
    const auto& type = std::get<string>($ntype);

    std::shared_ptr<ed::Variable> var{new ed::Variable};
    var->name = std::move(std::get<string>($name));
    var->annotation = "";
    try
    {
           CASE_VAR("i", std::shared_ptr<eastl::vector<Number>>, number_array_to_int_array)
      NEXT CASE_VAR("f", std::shared_ptr<eastl::vector<Number>>, number_array_to_float_array)
      NEXT CASE_VAR("t", std::shared_ptr<eastl::vector<string>>, AS_IS_UNREF)
      NEXT {
        throw std::runtime_error(string{"unknown array type"});
      }
    }
    catch(const std::runtime_error& e)
    {
      ederror(parser, fmt::format(" error in {}:{}, {} ", var->name, type, e.what()).c_str());
    }

    $$ = var;

    #undef AS_IS_UNREF
    #undef AS_IS
    #undef NEXT
    #undef CASE_VAR
  }
  ;

ANNOTATION
  : "@" "(" TEXT_VAL[a] ")" {
    $$ = std::move($a);
  }
  ;

PARAM_NAME
  : NAME_VAL[name] { $$ = std::move($name); }
  | TEXT_VAL[name] { $$ = std::move($name); }
  ;

VALUE
  : TEXT_VAL[v]       { $$ = $v; }
  | BOOL_VAL[v]       { $$ = $v; }
  | NUMBER_VALUE[v]   { $$ = $v; }
  | NUMBER2_VALUE[v]  { $$ = $v; }
  | NUMBER3_VALUE[v]  { $$ = $v; }
  | NUMBER4_VALUE[v]  { $$ = $v; }
  | MAT4_VALUE[v]     { $$ = $v; }
  | MAT3_VALUE[v]     { $$ = $v; }
  | ARRAY_VALUE[v]    { $$ = $v; }
  | SCOPE_BODY[v]     { $$ = $v; }
  ;

MAT4_VALUE
  : "[" ROW4_VALUE[r1] "," ROW4_VALUE[r2] "," ROW4_VALUE[r3] "," ROW4_VALUE[r4] "]" {
    float4 rows[4];
    rows[0] = number4_to_float4(std::get<Number4>($r1));
    rows[1] = number4_to_float4(std::get<Number4>($r2));
    rows[2] = number4_to_float4(std::get<Number4>($r3));
    rows[3] = number4_to_float4(std::get<Number4>($r4));

    float4 columns[4];
    for (int i = 0; i < 4; ++i)
      for (int j = 0; j < 4; ++j)
        columns[i][j] = rows[j][i];

    $$ = float4x4{columns[0], columns[1], columns[2], columns[3]};
  }

MAT3_VALUE
  : "[" ROW3_VALUE[r1] "," ROW3_VALUE[r2] "," ROW3_VALUE[r3] "]" {
    float3 rows[4];
    rows[0] = number3_to_float3(std::get<Number3>($r1));
    rows[1] = number3_to_float3(std::get<Number3>($r2));
    rows[2] = number3_to_float3(std::get<Number3>($r3));

    float3 columns[3];
    for (int i = 0; i < 3; ++i)
      for (int j = 0; j < 3; ++j)
        columns[i][j] = rows[j][i];

    $$ = float3x3{columns[0], columns[1], columns[2]};
  }

ROW4_VALUE
  : "[" NUMBER4_VALUE[v] "]" {
    $$ = $v;
  }
  ;

ROW3_VALUE
  : "[" NUMBER3_VALUE[v] "]" {
    $$ = $v;
  }
  ;

NUMBER4_VALUE
  : NUMBER_VALUE[v0] "," NUMBER_VALUE[v1] "," NUMBER_VALUE[v2] "," NUMBER_VALUE[v3] {
    $$ = Number4{
      std::get<Number>($v0),
      std::get<Number>($v1),
      std::get<Number>($v2),
      std::get<Number>($v3)};
  }
  ;

NUMBER3_VALUE
  : NUMBER_VALUE[v0] "," NUMBER_VALUE[v1] "," NUMBER_VALUE[v2] {
    $$ = Number3{
      std::get<Number>($v0),
      std::get<Number>($v1),
      std::get<Number>($v2)};
  }
  ;

NUMBER2_VALUE
  : NUMBER_VALUE[v0] "," NUMBER_VALUE[v1]{
    $$ = Number2{
      std::get<Number>($v0),
      std::get<Number>($v1)};
  }
  ;

NUMBER_VALUE
  : FLOAT_VAL[v] { $$ = $v; }
  | INT_VAL[v]   { $$ = $v; }
  ;

ARRAY_VALUE
  : "[" NUMBER_LIST[l] "]" {
    $$ = $l;
  }
  | "[" TEXT_LIST[l] "]" {
    $$ = $l;
  }
  ;

NUMBER_LIST
 : NUMBER_VALUE[n] "," NUMBER_LIST[list] {
  auto& l = std::get<std::shared_ptr<eastl::vector<Number>>>($list);
  l->push_back(std::get<Number>($n));
  $$ = l;
 }
 | NUMBER_VALUE[n] {
  auto l = std::make_shared<eastl::vector<Number>>();
  l->push_back(std::get<Number>($n));
  $$ = l;
 }
 ;

TEXT_LIST
 : TEXT_VAL[t] "," TEXT_LIST[list] {
  auto& l = std::get<std::shared_ptr<eastl::vector<string>>>($list);
  l->push_back(std::move(std::get<string>($t)));
  $$ = l;
 }
 | TEXT_VAL[t] {
  auto l = std::make_shared<eastl::vector<string>>();
  l->push_back(std::move(std::get<string>($t)));
  $$ = l;
 }
 ;
%%

void ederror(ed::Parser& parser, const char* msg) {
  const string err = fmt::format("parsing error: {} [{}:{}]", msg, parser.getCurrentFileName(), parser.getLine());
  parser.markParsingFailed(err);
}
