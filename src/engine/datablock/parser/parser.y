
%{
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
  extern char* bktext;
%}

%code requires  {
  #include "parser.h"

  #include <ranges>
  #include <variant>

  using BlockParam = std::variant<DataBlock, DataBlock::Attribute>;
  using BlockParams = eastl::vector<BlockParam>;

  static
  auto block_params_to_bk(BlockParams&& params) -> DataBlock
  {
    DataBlock bk;
    for (auto& param: params | std::views::reverse)
    {
      if (auto* bkChild = std::get_if<DataBlock>(&param))
        bk.insertBlock(*bkChild);
      else
       bk.insertAttribute(std::get<DataBlock::Attribute>(param));
    }

    return bk;
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

  #define BKSTYPE std::variant<         \
    DataBlock::Attribute::Type,         \
    bool, string,                       \
    Number, Number2, Number3, Number4,  \
    mat3, mat4,                         \
    DataBlock, DataBlock::Attribute,    \
    BlockParam, BlockParams>

  static
  const char* bkstype_to_string(const BKSTYPE& v)
  {
    if (auto* t = std::get_if<DataBlock::Attribute::Type>(&v))
      return "attribute type";
    if (auto* t = std::get_if<bool>(&v))
      return "bool";
    if (auto* t = std::get_if<string>(&v))
      return "text";
    if (auto* t = std::get_if<Number>(&v))
      return "number";
    if (auto* t = std::get_if<Number2>(&v))
      return "number2";
    if (auto* t = std::get_if<Number3>(&v))
      return "number3";
    if (auto* t = std::get_if<Number4>(&v))
      return "number4";
    if (auto* t = std::get_if<mat3>(&v))
      return "mat3";
    if (auto* t = std::get_if<mat4>(&v))
      return "mat4";
    if (auto* t = std::get_if<DataBlock>(&v))
      return "block";
    if (auto* t = std::get_if<DataBlock::Attribute>(&v))
      return "attribute";
    if (auto* t = std::get_if<BlockParam>(&v))
      return "block param";
    if (auto* t = std::get_if<BlockParams>(&v))
      return "block params";
    else
      return "";
  }

  static
  auto number_to_float(const BKSTYPE& bkstype) -> float
  {
    const Number& n = std::get<Number>(bkstype);
    if (auto* fl = std::get_if<float>(&n))
      return *fl;
    else
      return static_cast<float>(std::get<int>(n));
  }

  static
  auto number_to_int(const BKSTYPE& bkstype) -> int
  {
    const Number& n = std::get<Number>(bkstype);
    if (auto* v = std::get_if<int>(&n))
      return *v;
    else
      return static_cast<int>(std::get<float>(n));
  }

  static
  auto number2_to_float2(const BKSTYPE& bkstype) -> float2
  {
    const Number2& n = std::get<Number2>(bkstype);
    return float2{
      number_to_float(n.v0),
      number_to_float(n.v1)
    };
  }

  static
  auto number3_to_float3(const BKSTYPE& bkstype) -> float3
  {
    const Number3& n = std::get<Number3>(bkstype);
    return float3{
      number_to_float(n.v0),
      number_to_float(n.v1),
      number_to_float(n.v2)
    };
  }

  static
  auto number4_to_float4(const BKSTYPE& bkstype) -> float4
  {
    const Number4& n = std::get<Number4>(bkstype);
    return float4{
      number_to_float(n.v0),
      number_to_float(n.v1),
      number_to_float(n.v2),
      number_to_float(n.v3)
    };
  }

  static
  auto number2_to_int2(const BKSTYPE& bkstype) -> int2
  {
    const Number2& n = std::get<Number2>(bkstype);
    return int2{
      number_to_int(n.v0),
      number_to_int(n.v1)
    };
  }

  static
  auto number3_to_int3(const BKSTYPE& bkstype) -> int3
  {
    const Number3& n = std::get<Number3>(bkstype);
    return int3{
      number_to_int(n.v0),
      number_to_int(n.v1),
      number_to_int(n.v2)
    };
  }

  static
  auto number4_to_int4(const BKSTYPE& bkstype) -> int4
  {
    const Number4& n = std::get<Number4>(bkstype);
    return int4{
      number_to_int(n.v0),
      number_to_int(n.v1),
      number_to_int(n.v2),
      number_to_int(n.v3)
    };
  }
}

%locations

%define parse.error detailed
%define api.prefix {bk}

%lex-param {BlkParser& parser}
%parse-param {BlkParser& parser}

%token BOOL_VAL
%token INT_VAL
%token FLOAT_VAL
%token TEXT_VAL
%token NAME_VAL
%token AT "@"
%token COLON ":"
%token SEMICOLON ";"
%token EQUAL_OP "="
%token LEFT_PARENTHESIS "("
%token RIGHT_PARENTHESIS ")"
%token LEFT_BRACKET "{"
%token RIGHT_BRACKET "}"
%token LEFT_SQUARE_BRACKET "["
%token RIGHT_SQUARE_BRACKET "]"
%token COMMA ","
%token TEXT_TYPE "text"
%token FLOAT_TYPE "float"
%token POINT_2D_TYPE "float2"
%token POINT_3D_TYPE "float3"
%token POINT_4D_TYPE "float4"
%token INT_TYPE "int"
%token INT_POINT_2D_TYPE "int2"
%token INT_POINT_3D_TYPE "int3"
%token INT_POINT_4D_TYPE "int4"
%token MAT3_TYPE "mat3"
%token MAT4_TYPE "mat4"
%token BOOL_TYPE "bool"

%%

CONFIG
  : PARAM_LIST[list] {
    auto& params = std::get<BlockParams>($list);
    DataBlock bk = block_params_to_bk(std::move(params));
    parser.setBlock(std::move(bk));
  }
  ;

PARAM_LIST
  : ANNOTATED_PARAM[p] PARAM_LIST[list] {
    std::get<BlockParams>($list).push_back() = std::move(std::get<BlockParam>($p));
    $$ = std::move($list);
  }
  | ANNOTATED_PARAM[p] {
    BlockParams params;
    params.push_back() = std::move(std::get<BlockParam>($p));
    $$ = params;
  }
  ;

ANNOTATED_PARAM
  : PARAM_NAME[name] "@" "(" TEXT_VAL[a] ")" BLOCK[block] {
    auto& bk = std::get<DataBlock>($block);
    parser.setBkName(bk, std::move(std::get<string>($name)));
    parser.setBkAnnotation(bk, std::move(std::get<string>($a)));
    $$ = BlockParam{bk};
  }
  | PARAM_NAME[name] BLOCK[block] {
    auto& bk = std::get<DataBlock>($block);
    parser.setBkName(bk, std::move(std::get<string>($name)));
    $$ = BlockParam{bk};
  }
  | ATTRIBUTE[attr] "@" "(" TEXT_VAL[a] ")" {
    auto& attribute = std::get<DataBlock::Attribute>($attr);
    attribute.annotation = std::move(std::get<string>($a));
    $$ = BlockParam{std::move(attribute)};
  }
  | ATTRIBUTE[attr] {
    $$ = BlockParam{std::move(std::get<DataBlock::Attribute>($attr))};
  }
  ;

BLOCK
  : "{" PARAM_LIST[list] "}" {
    auto& params = std::get<BlockParams>($list);
    $$ = block_params_to_bk(std::move(params));
  }
  | "{" "}" {
    $$ = DataBlock{};
  }
  ;

ATTRIBUTE
  : ATTRIBUTE_TYPE[type] PARAM_NAME[name] "=" ATTRIBUTE_VALUE[v] {
    DataBlock::Attribute attr{
      .name = std::move(std::get<string>($name)),
      .annotation = "",
      .type = std::get<DataBlock::Attribute::Type>($type)
    };

    #define AS_IS(v) std::move(v)
    #define ATTRIBUTE_CASE(AttrType, ValueType, Getter)                                        \
      case DataBlock::Attribute::Type::AttrType:                                               \
      {                                                                                        \
        if (auto* value = std::get_if<ValueType>(&$v))                                         \
          attr.as = Getter(*value);                                                            \
        else                                                                                   \
        {                                                                                      \
          auto err = fmt::format("invalid type of attribute value. Expected: {} Provided: {}", \
            #ValueType, bkstype_to_string($v));                                                \
          bkerror(parser, err.c_str());                                                        \
        }                                                                                      \
        break;                                                                                 \
      }

    auto attrType = std::get<DataBlock::Attribute::Type>($type);
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

    $$ = attr;
  }
  ;

PARAM_NAME
  : NAME_VAL[name] { $$ = std::move($name); }
  | TEXT_VAL[name] { $$ = std::move($name); }
  ;

ATTRIBUTE_TYPE
  : "int"    { $$ = DataBlock::Attribute::Type::Int; }
  | "int2"   { $$ = DataBlock::Attribute::Type::Int2; }
  | "int3"   { $$ = DataBlock::Attribute::Type::Int3; }
  | "int4"   { $$ = DataBlock::Attribute::Type::Int4; }
  | "float"  { $$ = DataBlock::Attribute::Type::Float; }
  | "float2" { $$ = DataBlock::Attribute::Type::Float2; }
  | "float3" { $$ = DataBlock::Attribute::Type::Float3; }
  | "float4" { $$ = DataBlock::Attribute::Type::Float4; }
  | "mat3"   { $$ = DataBlock::Attribute::Type::Mat3; }
  | "mat4"   { $$ = DataBlock::Attribute::Type::Mat4; }
  | "bool"   { $$ = DataBlock::Attribute::Type::Bool; }
  | "text"   { $$ = DataBlock::Attribute::Type::Text; }
  ;

ATTRIBUTE_VALUE
  : TEXT_VAL[v]      { $$ = $v; }
  | BOOL_VAL[v]      { $$ = $v; }
  | NUMBER_VALUE[v]  { $$ = $v; }
  | NUMBER2_VALUE[v] { $$ = $v; }
  | NUMBER3_VALUE[v] { $$ = $v; }
  | NUMBER4_VALUE[v] { $$ = $v; }
  | MAT3_VALUE[v]    { $$ = $v; }
  | MAT4_VALUE[v]    { $$ = $v; }
  ;

MAT4_VALUE
  : "[" NUMBER4_VALUE[v0] "]" "," "[" NUMBER4_VALUE[v1] "]" "," "[" NUMBER4_VALUE[v2] "]" "," "[" NUMBER4_VALUE[v3] "]" {
    mat4 m;
    m[0] = number4_to_float4($v0);
    m[1] = number4_to_float4($v1);
    m[2] = number4_to_float4($v2);
    m[3] = number4_to_float4($v3);

    $$ = m;
  }
  ;

MAT3_VALUE
  : "[" NUMBER3_VALUE[v0] "]" "," "[" NUMBER3_VALUE[v1] "]" "," "[" NUMBER3_VALUE[v2] "]" {
    mat3 m;
    m[0] = number3_to_float3($v0);
    m[1] = number3_to_float3($v1);
    m[2] = number3_to_float3($v2);

    $$ = m;
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
%%

void bkerror(BlkParser& parser, const char* msg) {
  const string err = fmt::format("parsing error: {} [{}:{}]", msg, parser.getCurrentFileName(), parser.getLine());
  parser.markParsingFailed(err);
}
