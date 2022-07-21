
%{
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
%}

%code requires  {
  #include <shaders_compiler/compiler.h>
  #include <cstdio>
    using namespace ShadersSystem;
}

%define parse.error detailed
%define api.prefix {sh}

%lex-param {Compiler& compiler}
%parse-param {Compiler& compiler}

%union {
  float4 f4val;
  float3 f3val;
  float2 f2val;
  float fval;
  int ival;
  char* sval;

  ScopeDeclarationExp* scopeDeclaration;
  ScopeExp* scopeExp;

  ShadersResourcesReserveExp* shadersResourcesReserve;
  ResourceReserveExp* resReserveExp;
  gapi::ShaderStage shader;
  DescriptorSetReserveExp* dsetExp;
  ShaderExp* shaderExp;

  ResourceDeclaration* resourceDeclaration;
  ResourceType resourceType;
  ResourceAssignExp* resourceAssignExp;

  AttributeType attributeType;
  InputBufferExp* inputBufferExp;
  InputAttributeExp* inputAttributeExp;

  gapi::PrimitiveTopology primitiveTopology;

  TechniqueExp* techniqueExp;
  TargetProfile targetProfile;
}

%token <fval> TOKEN_FLOAT_VAL
%token <ival> TOKEN_INT_VAL
%token <sval> TOKEN_NAME_VAL
%token <sval> TOKEN_HLSL_CODE

%token TOKEN_AT "@"
%token TOKEN_COLON ":"
%token TOKEN_SEMICOLON ";"
%token TOKEN_EQUAL_OP "="
%token TOKEN_LEFT_PARENTHESIS "("
%token TOKEN_RIGHT_PARENTHESIS ")"
%token TOKEN_LEFT_BRACKET "{"
%token TOKEN_RIGHT_BRACKET "}"
%token TOKEN_COMMA ","
%token TOKEN_MINUS "-"
%token TOKEN_INPUT "input"
%token TOKEN_BUFFER "buffer"
%token TOKEN_SCOPE "scope"
%token TOKEN_SHADER "shader"
%token TOKEN_SHADER_VERTEX "vertex"
%token TOKEN_SHADER_PIXEL "pixel"
%token TOKEN_REGISTER "register"
%token TOKEN_TEXTURE "texture"
%token TOKEN_CBUFFER "cbuffer"
%token TOKEN_EXTERN "extern"
%token TOKEN_CHANNEL "channel"
%token TOKEN_DESCRIPTOR_SET "descriptor_set"
%token TOKEN_TECHNIQUE "technique"
%token TOKEN_SUPPORT "support"
%token TOKEN_ACTIVATE "activate"
%token TOKEN_COMPILE "compile"
%token TOKEN_PRIMITIVE_TOPOLOGY "primitive_topology"
%token TOKEN_PT_POINT_LIST "point_list"
%token TOKEN_PT_LINE_LIST "line_list"
%token TOKEN_PT_LINE_STRIP "line_strip"
%token TOKEN_PT_TRIANGLE_LIST "triangle_list"
%token TOKEN_PT_TRIANGLE_FAN "triangle_fan"
%token TOKEN_PT_LINE_LIST_WITH_ADJACENCY "line_list_with_adjacency"
%token TOKEN_PT_LINE_STRIP_WITH_ADJACENCY "line_strip_with_adjacency"
%token TOKEN_PT_TRIANGLE_LIST_WITH_ADJACENCY "triangle_list_with_adjacency"
%token TOKEN_PT_TRIANGLE_STRIP_WITH_ADJACENCY "triangle_strip_with_adjacency"
%token TOKEN_PT_PATCH_LIST "patch_list"
%token TOKEN_TARGET_VS_6_0 "vs_6_0"
%token TOKEN_TARGET_VS_6_1 "vs_6_1"
%token TOKEN_TARGET_VS_6_2 "vs_6_2"
%token TOKEN_TARGET_VS_6_3 "vs_6_3"
%token TOKEN_TARGET_VS_6_4 "vs_6_4"
%token TOKEN_TARGET_VS_6_5 "vs_6_5"
%token TOKEN_TARGET_VS_6_6 "vs_6_6"
%token TOKEN_TARGET_VS_6_7 "vs_6_7"
%token TOKEN_TARGET_PS_6_0 "ps_6_0"
%token TOKEN_TARGET_PS_6_1 "ps_6_1"
%token TOKEN_TARGET_PS_6_2 "ps_6_2"
%token TOKEN_TARGET_PS_6_3 "ps_6_3"
%token TOKEN_TARGET_PS_6_4 "ps_6_4"
%token TOKEN_TARGET_PS_6_5 "ps_6_5"
%token TOKEN_TARGET_PS_6_6 "ps_6_6"
%token TOKEN_TARGET_PS_6_7 "ps_6_7"

%token TOKEN_FLOAT
%token TOKEN_FLOAT2
%token TOKEN_FLOAT3
%token TOKEN_FLOAT4
%token TOKEN_INT
%token TOKEN_INT2
%token TOKEN_INT3
%token TOKEN_INT4
%token TOKEN_TEXTURE2D

%type <techniqueExp>         TECHNIQUE_EXP
%type <techniqueExp>         TECHNIQUE_EXP_LIST
%type <shaderExp>            SHADERS_LIST
%type <shader>               SHADERS_ELEM
%type <scopeExp>             SCOPE_EXP
%type <scopeExp>             SCOPE_EXP_LIST
%type <resReserveExp>        RESOURCE_RESERVE_EXP
%type <resReserveExp>        RESOURCE_RESERVE_EXP_LIST
%type <ival>                 INT_VALUE
%type <resourceType>         RESOURCE_TYPE
%type <resourceAssignExp>    ASSIGN_EXP
%type <targetProfile>        TARGET_PROFILE
%type <inputBufferExp>       INPUT_BUFFER
%type <inputBufferExp>       INPUT_BUFFER_LIST
%type <inputAttributeExp>    INPUT_ATTRIBUTE
%type <inputAttributeExp>    INPUT_ATTRIBUTE_LIST
%type <attributeType>        ATTRIBUTE_TYPE
%type <primitiveTopology>    PRIMITIVE_TOPOLOGY

%%

MODULE_EXPRESSIONS
  : MODULE_EXPRESSION MODULE_EXPRESSIONS {
  }
  | MODULE_EXPRESSION {
  }
  ;

MODULE_EXPRESSION
  : SCOPE_DECLARATION {
  }
  | TECHNIQUE_DECLARATION {
  }
  ;

TECHNIQUE_DECLARATION
  : "technique" TOKEN_NAME_VAL[name] "{" TECHNIQUE_EXP_LIST[exps] "}" ";" {
    compiler.onTechniqueDeclaration(new TechniqueDeclarationExp{$name, $exps});
  }
  ;

TECHNIQUE_EXP_LIST
  : TECHNIQUE_EXP[exp] TECHNIQUE_EXP_LIST[next] {
    $$ = $exp;
    $$->next = $next;
  }
  | TECHNIQUE_EXP[exp] {
    $$ = $exp;
  }
  ;

TECHNIQUE_EXP
  : "support" "scope" TOKEN_NAME_VAL[name] ";" {
    $$ = new ScopeSupportExp($name);
  }
  | "activate" "scope" TOKEN_NAME_VAL[name] ";" {
    $$ = new ScopeActivateExp($name);
  }
  | "compile" "(" TARGET_PROFILE[target] "," TOKEN_NAME_VAL[entry] ")" ";" {
    $$ = new CompileExp($target, $entry);
  }
  | TOKEN_HLSL_CODE[code] {
    $$ = new HlslExp($code);
  }
  | "input" ":" INPUT_BUFFER_LIST[buffers] {
    $$ = new InputExp($buffers);
  }
  | "primitive_topology" "=" PRIMITIVE_TOPOLOGY[v] ";"{
    $$ = new PrimitiveTopologyExp($v);
  }
  ;

INPUT_BUFFER_LIST
  : INPUT_BUFFER[buffer] INPUT_BUFFER_LIST[next] {
    $$ = $buffer;
    $$->next = $next;
  }
  | INPUT_BUFFER[buffer] {
    $$ = $buffer;
    $$->next = nullptr;
  }
  ;

INPUT_BUFFER
  : "buffer" "(" INT_VALUE[reg] ")" ":" INPUT_ATTRIBUTE_LIST[attrs] {
    $$ = new InputBufferExp($reg, $attrs);
  }
  ;

INPUT_ATTRIBUTE_LIST
  : INPUT_ATTRIBUTE[attr] INPUT_ATTRIBUTE_LIST[next] {
    $$ = $attr;
    $$->next = $next;
  }
  | INPUT_ATTRIBUTE[attr] {
    $$ = $attr;
    $$->next = nullptr;
  }
  ;

INPUT_ATTRIBUTE
  : ATTRIBUTE_TYPE[type] TOKEN_NAME_VAL[name] ";" {
    $$ = new InputAttributeExp($type, $name);
  }
  ;

PRIMITIVE_TOPOLOGY
  : "point_list" {
    $$ = gapi::PrimitiveTopology::PointList;
  }
  | "line_list" {
    $$ = gapi::PrimitiveTopology::LineList;
  }
  | "line_strip" {
    $$ = gapi::PrimitiveTopology::LineStrip;
  }
  | "triangle_list" {
    $$ = gapi::PrimitiveTopology::TriangleList;
  }
  | "triangle_fan" {
    $$ = gapi::PrimitiveTopology::TriangleFan;
  }
  | "line_list_with_adjacency" {
    $$ = gapi::PrimitiveTopology::LineListWithAdjacency;
  }
  | "line_strip_with_adjacency" {
    $$ = gapi::PrimitiveTopology::LineStripWithAdjacency;
  }
  | "triangle_list_with_adjacency" {
    $$ = gapi::PrimitiveTopology::TriangleListWithAdjacency;
  }
  | "triangle_strip_with_adjacency" {
    $$ = gapi::PrimitiveTopology::TriangleStripWithAdjacency;
  }
  | "patch_list" {
    $$ = gapi::PrimitiveTopology::PatchList;
  }
  ;

SCOPE_DECLARATION
  : "scope" TOKEN_NAME_VAL[name] "{" SCOPE_EXP_LIST[exps] "}" ";" {
    compiler.onScopeDeclaration(new ScopeDeclarationExp{$name, $exps});
  }
  ;

SCOPE_EXP_LIST
  : SCOPE_EXP[el] SCOPE_EXP_LIST[next] {
    $$ = $el;
    $$->next = $next;
  }
  | SCOPE_EXP[el] {
    $$ = $el;
    $$->next = nullptr;
  }
  ;

SCOPE_EXP
  : TOKEN_SHADER SHADERS_LIST[shaders] ":" RESOURCE_RESERVE_EXP_LIST[exps] {
    $$ = new ShadersResourcesReserveExp($shaders, $exps);
  }
  | RESOURCE_TYPE[type] TOKEN_NAME_VAL[name] "=" ASSIGN_EXP[exps] ";" {
    $$ = new ResourceDeclarationExp($type, $name, $exps);
  }
  | ATTRIBUTE_TYPE[type] TOKEN_NAME_VAL[name] "=" ASSIGN_EXP[exps] ";" {
    $$ = new CbufferVarDeclarationExp($type, $name, $exps);
  }
  ;

RESOURCE_RESERVE_EXP_LIST
 : RESOURCE_RESERVE_EXP[el] ";" RESOURCE_RESERVE_EXP_LIST[next] {
  $$ = $el;
  $$->next = $next;
 }
 | RESOURCE_RESERVE_EXP[el] ";" {
   $$ = $el;
 }
 ;

RESOURCE_RESERVE_EXP
  : "register" "(" "texture" "," INT_VALUE[v0] "-" INT_VALUE[v1] ")" {
    $$ = new RegistersReserveExp(RegistersReserveExp::Register::Texture, $v0, $v1);

  }
  | "cbuffer" "(" INT_VALUE[register] ")" {
    $$ = new CBufferReserveExp($register);
  }
  | "descriptor_set" "(" INT_VALUE[v] ")" {
    $$ = new DescriptorSetReserveExp($v);
  }
  ;

ASSIGN_EXP
  : "extern" "(" TOKEN_NAME_VAL[name] ")" {
    $$ = new AccessResource(ResourceAccessType::Extern, $name);
  }
  | "channel" "(" TOKEN_NAME_VAL[name] ")" {
    $$ = new AccessResource(ResourceAccessType::Channel, $name);
  }
  ;

RESOURCE_TYPE
  : TOKEN_TEXTURE2D {
    $$ = ResourceType::Texture2D;
  }
  ;

ATTRIBUTE_TYPE
  : TOKEN_INT {
    $$ = AttributeType::Int;
  }
  | TOKEN_INT2 {
    $$ = AttributeType::Int2;
  }
  | TOKEN_INT3 {
    $$ = AttributeType::Int3;
  }
  | TOKEN_INT4 {
    $$ = AttributeType::Int4;
  }
  | TOKEN_FLOAT {
    $$ = AttributeType::Float;
  }
  | TOKEN_FLOAT2 {
    $$ = AttributeType::Float2;
  }
  | TOKEN_FLOAT3 {
    $$ = AttributeType::Float3;
  }
  | TOKEN_FLOAT4 {
    $$ = AttributeType::Float4;
  }
  ;

INT_VALUE
  : TOKEN_INT_VAL[v] {
    $$ = $v;
  }
  ;

SHADERS_LIST
  : SHADERS_ELEM[shader] "," SHADERS_LIST[shaders] {
    $$ = $shaders;
    $$->shaders = (gapi::ShaderStage)($$->shaders | $shader);
  }
  | SHADERS_ELEM[shader] {
    $$ = new ShaderExp($shader);
  }
  ;

SHADERS_ELEM
  : "vertex" {
    $$ = gapi::ShaderStage::Vertex;
  }
  | "pixel" {
    $$ = gapi::ShaderStage::Fragment;
  }
  ;

TARGET_PROFILE
  : "vs_6_0" {
    $$ = TargetProfile::VS_6_0;
  }
  | "vs_6_1" {
    $$ = TargetProfile::VS_6_1;
  }
  | "vs_6_2" {
    $$ = TargetProfile::VS_6_2;
  }
  | "vs_6_3" {
    $$ = TargetProfile::VS_6_3;
  }
  | "vs_6_4" {
    $$ = TargetProfile::VS_6_4;
  }
  | "vs_6_5" {
    $$ = TargetProfile::VS_6_5;
  }
  | "vs_6_6" {
    $$ = TargetProfile::VS_6_6;
  }
  | "vs_6_7" {
    $$ = TargetProfile::VS_6_7;
  }
  | "ps_6_0" {
    $$ = TargetProfile::PS_6_0;
  }
  | "ps_6_1" {
    $$ = TargetProfile::PS_6_1;
  }
  | "ps_6_2" {
    $$ = TargetProfile::PS_6_2;
  }
  | "ps_6_3" {
    $$ = TargetProfile::PS_6_3;
  }
  | "ps_6_4" {
    $$ = TargetProfile::PS_6_4;
  }
  | "ps_6_5" {
    $$ = TargetProfile::PS_6_5;
  }
  | "ps_6_6" {
    $$ = TargetProfile::PS_6_6;
  }
  | "ps_6_7" {
    $$ = TargetProfile::PS_6_7;
  }

%%

void sherror(Compiler& compiler, const char* msg) {
  logerror("[{}] shader technique parsing error: {} {}", "<todo>", shlineno, msg);
}