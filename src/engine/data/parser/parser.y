
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

  #define DECL_NODE_NAMED(type, typeName)                        \
    struct typeName ## Node                                      \
    {                                                            \
      using Ptr = std::shared_ptr<typeName ## Node>;             \
                                                                 \
      typeName ## Node() = default;                              \
      typeName ## Node(type&& v) : val(std::move(v)) {}          \
                                                                 \
      static                                                     \
      auto make(type&& v) -> Ptr                                 \
      {                                                          \
        return std::make_shared<typeName ## Node>(std::move(v)); \
      }                                                          \
                                                                 \
      type val;                                                  \
    };                                                           \
    using typeName ## NodePtr = std::shared_ptr<typeName ## Node>
  
  #define DECL_NODE(type) DECL_NODE_NAMED(type, type)

  DECL_NODE(Number2);
  DECL_NODE(Number3);
  DECL_NODE(Number4);
  DECL_NODE_NAMED(float3x3, Mat3);
  DECL_NODE_NAMED(float4x4, Mat4);
  DECL_NODE_NAMED(ed::IntArray, IntArray);
  DECL_NODE_NAMED(ed::FloatArray, FloatArray);
  DECL_NODE_NAMED(ed::TextArray, TextArray);
  DECL_NODE_NAMED(eastl::vector<Number>, NumberArray);
  DECL_NODE_NAMED(ed::Scope, Scope);
  DECL_NODE_NAMED(ed::Variable, Variable);

  using ScopeParam = std::variant<ScopeNodePtr, VariableNodePtr>;
  using ScopeParamArray = eastl::vector<ScopeParam>;
  DECL_NODE(ScopeParamArray);

  #define EDSTYPE std::variant<                                                \
    bool, Number, string,                                                      \
    Number2NodePtr, Number3NodePtr, Number4NodePtr,                            \
    Mat3NodePtr, Mat4NodePtr,                                                  \
    IntArrayNodePtr, FloatArrayNodePtr, TextArrayNodePtr, NumberArrayNodePtr,  \
    ScopeNodePtr, VariableNodePtr,                                             \
    ScopeParam, ScopeParamArrayNodePtr                                         \
  >

  static
  auto scope_params_to_scope(ed::Parser& parser, ScopeParamArray& params) -> ScopeNodePtr
  {
    ed::Scope scope = parser.makeScope();
    for (ScopeParam& param: params | std::views::reverse)
    {
      if (auto* nestedScope = std::get_if<ScopeNodePtr>(&param))
        scope.addScope(std::move((*nestedScope)->val));
      else
        parser.addVariableDefinition(std::move(std::get<VariableNodePtr>(param)->val), scope);
    }

    return ScopeNode::make(std::move(scope));
  }

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
    return number_to<float>(edstype);
  }

  static
  auto number_to_int(const EDSTYPE& edstype) -> int
  {
    return number_to<int>(edstype);
  }

  static
  auto number2_to_float2(const EDSTYPE& edstype) -> float2
  {
    const auto& n = std::get<Number2NodePtr>(edstype);
    return float2{
      number_to_float(n->val.v0),
      number_to_float(n->val.v1)
    };
  }

  static
  auto number3_to_float3(const EDSTYPE& edstype) -> float3
  {
    const auto& n = std::get<Number3NodePtr>(edstype);
    return float3{
      number_to_float(n->val.v0),
      number_to_float(n->val.v1),
      number_to_float(n->val.v2)
    };
  }

  static
  auto number4_to_float4(const EDSTYPE& edstype) -> float4
  {
    const auto& n = std::get<Number4NodePtr>(edstype);
    return float4{
      number_to_float(n->val.v0),
      number_to_float(n->val.v1),
      number_to_float(n->val.v2),
      number_to_float(n->val.v3)
    };
  }

  static
  auto number2_to_int2(const EDSTYPE& edstype) -> int2
  {
    const auto& n = std::get<Number2NodePtr>(edstype);
    return int2{
      number_to_int(n->val.v0),
      number_to_int(n->val.v1)
    };
  }

  static
  auto number3_to_int3(const EDSTYPE& edstype) -> int3
  {
    const auto& n = std::get<Number3NodePtr>(edstype);
    return int3{
      number_to_int(n->val.v0),
      number_to_int(n->val.v1),
      number_to_int(n->val.v2)
    };
  }

  static
  auto number4_to_int4(const EDSTYPE& edstype) -> int4
  {
    const auto& n = std::get<Number4NodePtr>(edstype);
    return int4{
      number_to_int(n->val.v0),
      number_to_int(n->val.v1),
      number_to_int(n->val.v2),
      number_to_int(n->val.v3)
    };
  }

  template<class T>
  static
  auto number_array_to_T_array(const EDSTYPE& edstype) -> eastl::vector<T>
  {
    const auto& from = std::get<NumberArrayNodePtr>(edstype);
    eastl::vector<T> to;

    to.reserve(from->val.size());
    for (const Number& n: from->val)
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
    auto& params = std::get<ScopeParamArrayNodePtr>($list);
    ScopeNodePtr scope = scope_params_to_scope(parser, params->val);
    parser.setMainScope(std::move(scope->val));
  }
  ;

PARAM_LIST
  : ANNOTATED_PARAM[p] PARAM_LIST[list] {
    auto l = std::get<ScopeParamArrayNodePtr>($list);
    l->val.push_back(std::get<ScopeParam>($p));
    $$ = l;
  }
  | ANNOTATED_PARAM[p] {
    auto params = ScopeParamArrayNode::make({});
    params->val.push_back((std::get<ScopeParam>($p)));
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
    auto& scope = std::get<ScopeNodePtr>($body);
    parser.setScopeName(scope->val, std::move(std::get<string>($name)));
    parser.setScopeAnnotation(scope->val, std::move(std::get<string>($a)));
    $$ = ScopeParam{scope};
  }
  | PARAM_NAME[name] SCOPE_BODY[body] {
    auto& scope = std::get<ScopeNodePtr>($body);
    parser.setScopeName(scope->val, std::move(std::get<string>($name)));
    $$ = ScopeParam{scope};
  }
  ;

SCOPE_BODY
  : "{" PARAM_LIST[list] "}" {
    auto& params = std::get<ScopeParamArrayNodePtr>($list);
    $$ = scope_params_to_scope(parser, params->val);
  }
  | "{" "}" {
    $$ = ScopeNode::make(parser.makeScope());
  }
  ;

ANNOTATED_VARIABLE
  : VARIABLE[var] ANNOTATION[a] {
    auto& var = std::get<VariableNodePtr>($var);
    var->val.annotation = std::move(std::get<string>($a));
    $$ = ScopeParam{var};
  }
  | VARIABLE[var] {
    $$ = ScopeParam{std::get<VariableNodePtr>($var)};
  }
  ;

VARIABLE
  : PARAM_NAME[name] ":" NAME_VAL[ntype] "=" VALUE[value] {
    const string& type = std::get<string>($ntype);

    ed::Variable var;
    var.name = std::move(std::get<string>($name));
    var.annotation = "";

    try
    {
      #define CASE_VAR(textType, valueType, getter)             \
        if (type == textType)                               \
        {                                                   \
          if (auto* value = std::get_if<valueType>(&$value))\
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
        if (auto* scope = std::get_if<ScopeNodePtr>(&$value))
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

    $$ = VariableNode::make(std::move(var));
  }
  | PARAM_NAME[name] ":" NAME_VAL[ntype] "[""]" "=" VALUE[value] {
    const auto& type = std::get<string>($ntype);

    ed::Variable var;
    var.name = std::move(std::get<string>($name));
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

    $$ = VariableNode::make(std::move(var));

    #undef AS_IS_DEREF
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
    rows[0] = number4_to_float4($r1);
    rows[1] = number4_to_float4($r2);
    rows[2] = number4_to_float4($r3);
    rows[3] = number4_to_float4($r4);

    float4 columns[4];
    for (int i = 0; i < 4; ++i)
      for (int j = 0; j < 4; ++j)
        columns[i][j] = rows[j][i];

    $$ = Mat4Node::make(float4x4{columns[0], columns[1], columns[2], columns[3]});
  }

MAT3_VALUE
  : "[" ROW3_VALUE[r1] "," ROW3_VALUE[r2] "," ROW3_VALUE[r3] "]" {
    float3 rows[4];
    rows[0] = number3_to_float3($r1);
    rows[1] = number3_to_float3($r2);
    rows[2] = number3_to_float3($r3);

    float3 columns[3];
    for (int i = 0; i < 3; ++i)
      for (int j = 0; j < 3; ++j)
        columns[i][j] = rows[j][i];

    $$ = Mat3Node::make(float3x3{columns[0], columns[1], columns[2]});
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
    $$ = Number4Node::make(
      Number4{
        std::get<Number>($v0),
        std::get<Number>($v1),
        std::get<Number>($v2),
        std::get<Number>($v3)
      }
    );
  }
  ;

NUMBER3_VALUE
  : NUMBER_VALUE[v0] "," NUMBER_VALUE[v1] "," NUMBER_VALUE[v2] {
    $$ = Number3Node::make(
      Number3{
        std::get<Number>($v0),
        std::get<Number>($v1),
        std::get<Number>($v2)
      }
    );
  }
  ;

NUMBER2_VALUE
  : NUMBER_VALUE[v0] "," NUMBER_VALUE[v1]{
    $$ = Number2Node::make(
      Number2{
        std::get<Number>($v0),
        std::get<Number>($v1)
      }
    );
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
  auto& l = std::get<NumberArrayNodePtr>($list);
  l->val.push_back(std::get<Number>($n));
  $$ = l;
 }
 | NUMBER_VALUE[n] {
  auto l = NumberArrayNode::make({});
  l->val.push_back(std::get<Number>($n));
  $$ = l;
 }
 ;

TEXT_LIST
 : TEXT_VAL[t] "," TEXT_LIST[list] {
  auto l = std::get<TextArrayNodePtr>($list);
  l->val.push_back(std::move(std::get<string>($t)));
  $$ = l;
 }
 | TEXT_VAL[t] {
  auto l = TextArrayNode::make({});
  l->val.push_back(std::move(std::get<string>($t)));
  $$ = l;
 }
 ;
%%

void ederror(ed::Parser& parser, const char* msg) {
  const string err = fmt::format("parsing error: {} [{}:{}]", msg, parser.getCurrentFileName(), parser.getLine());
  parser.markParsingFailed(err);
}
