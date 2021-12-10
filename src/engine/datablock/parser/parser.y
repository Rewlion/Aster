
%{
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
%}

%code requires  {
  #include "ast.h"
  #include <cstdio>

  using glm::vec2;
  using glm::vec3;
  using glm::vec4;
  using glm::ivec2;
  using glm::ivec3;
  using glm::ivec4;
  using glm::mat3;
  using glm::mat4;
}

%define parse.error detailed

%parse-param {Ast::Config* rootAst}

%union {
  bool bval;
  int ival;
  float fval;
  char* sval;
  vec2 f2val;
  vec3 f3val;
  vec4 f4val;
  ivec2 i2val;
  ivec3 i3val;
  ivec4 ival4;
  Ast::Config* configNode;
  Ast::AnnotatedParam* paramListNode;
  Ast::Block* blockNode;
  Ast::Attribute* attributeNode;
  Ast::AttributeType attributeType;
  Ast::AttributeValue* attributeValue;
}

%token <bval> BOOL_VAL
%token <ival> INT_VAL
%token <fval> FLOAT_VAL
%token <sval> TEXT_VAL
%token <sval> NAME_VAL
%token AT "@"
%token COLON ":"
%token EQUAL_OP "="
%token LEFT_BRACKET "{"
%token RIGHT_BRACKET "}"
%token LEFT_SQUARE_BRACKET "["
%token RIGHT_SQUARE_BRACKET "]"
%token COMMA ","
%token INT_TYPE
%token FLOAT_TYPE
%token TEXT_TYPE
%token POINT_2D_TYPE
%token POINT_3D_TYPE
%token POINT_4D_TYPE
%token INT_POINT_2D_TYPE
%token INT_POINT_3D_TYPE
%token INT_POINT_4D_TYPE
%token MAT4_TYPE
%token MAT3_TYPE
%token BOOL_TYPE;

%type <configNode> CONFIG;
%type <paramListNode> PARAM_LIST;
%type <paramListNode> ANNOTATED_PARAM;
%type <blockNode> BLOCK;
%type <attributeNode> ATTRIBUTE;
%type <f4val> ROW4;
%type <f3val> ROW3;
%type <attributeType> ATTRIBUTE_TYPE
%type <attributeValue> ATTRIBUTE_VALUE
%%

CONFIG
  : PARAM_LIST[params] {
    rootAst->paramList = $params;
  }
  ;

PARAM_LIST
  : ANNOTATED_PARAM[p] PARAM_LIST[next] {
    $$ = $p;
    $$->next = $next;
  }
  | ANNOTATED_PARAM[p] {
    $$ = $p;
  }
  ;

ANNOTATED_PARAM
  : "@" NAME_VAL[a] ":" NAME_VAL[name] BLOCK[block] { printf("KOKO:!!:%s:%s", $a, $name);
    $$ = $block;
    $$->name = $name; free($name);
    $$->annotation = $a;
  }
  | NAME_VAL[name] BLOCK[block] {
    $$ = $block;
    $$->name = $name;
  }
  | "@" NAME_VAL[a] ":" NAME_VAL[name] ":" ATTRIBUTE[attr] {
    $$ = $attr;
    $$->name = $name; free($name);
    $$->annotation = $a;
  }
  | NAME_VAL[name] ":" ATTRIBUTE[attr] {
    $$ = $attr;
    $$->name = $name;
  }
  ;

BLOCK
  : "{" PARAM_LIST[params] "}" {
    $$ = new Ast::Block;
    $$->paramList = $params;
  }
  | "{" "}" {
    $$ = new Ast::Block;
  }
  ;

ATTRIBUTE
  : ATTRIBUTE_TYPE[type] "=" ATTRIBUTE_VALUE[value] {
    auto valueType = $value->GetType();
    if ( $type == valueType )
    {
      $$ = new Ast::Attribute;
      $$->valueNode = $value;
      $$->type = $type;
    }
    else
    {
      auto expectedTypeStr = Ast::GetAttributeTypeStr($type);
      auto valueTypeStr = Ast::GetAttributeTypeStr(valueType);
      auto valueStr = $value->ToStr();

      char buffer [100];
      std::snprintf(buffer, 100,"%s=%s  error: value has wrong type `%s`, `%s` expected",
      expectedTypeStr,  valueStr.c_str(),
      valueTypeStr, expectedTypeStr
      );

      yyerror(nullptr, buffer);
    }
  }
  ;

ATTRIBUTE_TYPE
  : INT_TYPE {
    $$ = Ast::AttributeType::Int;
  }
  | INT_POINT_2D_TYPE {
    $$ = Ast::AttributeType::Int2;
  }
  | INT_POINT_3D_TYPE {
    $$ = Ast::AttributeType::Int3;
  }
  | INT_POINT_4D_TYPE {
    $$ = Ast::AttributeType::Int4;
  }
  | FLOAT_TYPE {
    $$ = Ast::AttributeType::Float;
  }
  | POINT_2D_TYPE {
    $$ = Ast::AttributeType::Float2;
  }
  | POINT_3D_TYPE {
    $$ = Ast::AttributeType::Float3;
  }
  | POINT_4D_TYPE {
    $$ = Ast::AttributeType::Float4;
  }
  | MAT3_TYPE {
    $$ = Ast::AttributeType::Mat3;
  }
  | MAT4_TYPE {
    $$ = Ast::AttributeType::Mat4;
  }
  | BOOL_TYPE {
    $$ = Ast::AttributeType::Bool;
  }
  | TEXT_TYPE {
    $$ = Ast::AttributeType::Text;
  }
  ;

ATTRIBUTE_VALUE
  : TEXT_VAL[t] {
    auto v = new Ast::TextValue; v->value = std::string($t); free($t);
    $$ = v;
  }
  | FLOAT_VAL[v] {
    auto v = new Ast::FloatValue; v->value = $v;
    $$ = v;
  }
  | FLOAT_VAL[f0] "," FLOAT_VAL[f1] {
    auto v = new Ast::Float2Value; v->value = vec2($f0, $f1);
    $$ = v;
  }
  | FLOAT_VAL[f0] "," FLOAT_VAL[f1] "," FLOAT_VAL[f2] {
    auto v = new Ast::Float3Value; v->value = vec3($f0, $f1, $f2);
    $$ = v;
  }
  | FLOAT_VAL[f0] "," FLOAT_VAL[f1] "," FLOAT_VAL[f2] "," FLOAT_VAL[f3] {
    auto v = new Ast::Float4Value; v->value = vec4($f0, $f1, $f2, $f3);
    $$ = v;
  }
  | INT_VAL[v] {
    auto v = new Ast::IntValue; v->value = $v;
    $$ = v;
  }
  | INT_VAL[f0] "," INT_VAL[f1] {
    auto v = new Ast::Int2Value; v->value = ivec2($f0, $f1);
    $$ = v;
  }
  | INT_VAL[f0] "," INT_VAL[f1] "," INT_VAL[f2] {
    auto v = new Ast::Int3Value; v->value = ivec3($f0, $f1, $f2);
    $$ = v;
  }
  | INT_VAL[f0] "," INT_VAL[f1] "," INT_VAL[f2] "," INT_VAL[f3] {
    auto v = new Ast::Int4Value; v->value = ivec4($f0, $f1, $f2, $f3);
    $$ = v;
  } 
  | BOOL_VAL[b] {
    auto v = new Ast::BoolValue; v->value = $b;
    $$ = v;
  }
  | "[" ROW3[r0] "," ROW3[r1] "," ROW3[r2] "]" {
    auto v = new Ast::Mat3Value; v->value = mat3( $r0, $r1, $r2);
    $$ = v;
  }
  | "[" ROW4[r0] "," ROW4[r1] "," ROW4[r2] "," ROW4[r3] "]" {
    auto v = new Ast::Mat4Value; v->value = mat4( $r0, $r1, $r2, $r3);
    $$ = v;
  }
  ;

ROW3
  : "[" FLOAT_VAL[v0] "," FLOAT_VAL[v1] "," FLOAT_VAL[v2] "]" {
    $$ = vec3{ $v0, $v1, $v2 };
  }
  ;

ROW4
  : "[" FLOAT_VAL[v0] "," FLOAT_VAL[v1] "," FLOAT_VAL[v2] "," FLOAT_VAL[v3] "]" {
    $$ = vec4{ $v0, $v1, $v2, $v3 };
  }
  ;

%%

void yyerror(Ast::Config* rootAst, const char* msg) {
  logerror("blk parsing error: {} {}", yylineno, msg);
  assert(!"fuck");
  //exit(-1);
}