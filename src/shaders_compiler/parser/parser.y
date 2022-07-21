
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
  bool bval;

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

  RenderStateExp* renderStateExp;
  gapi::PrimitiveTopology primitiveTopology;
  gapi::CompareOp compareOp;
  gapi::StencilOp stencilOp;
  BlendingExp* blendingExp;
  MrtBlendingExp* mrtBlendingExp;
  gapi::BlendFactor blendFactor;
  gapi::BlendOp blendOp;
  gapi::LogicOp logicOp;

  TechniqueExp* techniqueExp;
  TargetProfile targetProfile;
}

%token <fval> TOKEN_FLOAT_VAL
%token <ival> TOKEN_INT_VAL
%token <sval> TOKEN_NAME_VAL
%token <bval> TOKEN_BOOL_VAL
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
%token TOKEN_RENDER_STATE "render_state"
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
%token TOKEN_DEPTH_TEST "depth_test"
%token TOKEN_DEPTH_WRITE "depth_write"
%token TOKEN_DEPTH_OP "depth_op"
%token TOKEN_STENCIL_TEST "stencil_test"
%token TOKEN_STENCIL_FAIL_OP "stencil_fail_op"
%token TOKEN_STENCIL_PASS_OP "stencil_pass_op"
%token TOKEN_STENCIL_DEPTH_FAIL_OP "stencil_depth_fail_op"
%token TOKEN_STENCIL_COMPARE_OP "stencil_compare_op"
%token TOKEN_STENCIL_REFERENCE_VALUE "stencil_ref_val"
%token TOKEN_NEVER "never"
%token TOKEN_LESS "less"
%token TOKEN_EQUAL "equal"
%token TOKEN_LESS_OR_EQUAL "less_or_equal"
%token TOKEN_GREATER "greater"
%token TOKEN_NOT_EQUAL "not_equal"
%token TOKEN_GREATER_OR_EQUAL "greater_or_equal"
%token TOKEN_ALWAYS "always"
%token TOKEN_KEEP "keep"
%token TOKEN_ZERO "zero"
%token TOKEN_REPLACE "replace"
%token TOKEN_INCREMENT_AND_CLAMP "inc_and_clamp"
%token TOKEN_DECREMENT_AND_CLAMP "dec_and_clamp"
%token TOKEN_INVERT "invert"
%token TOKEN_INCREMENT_AND_WRAP "inc_and_wrap"
%token TOKEN_DECREMENT_AND_WRAP "dec_and_wrap"
%token TOKEN_BLENDING "blending"
%token TOKEN_HAS_BLANDING "has_blending"
%token TOKEN_LOGIC_OP_ENABLED "logic_op_enabled"
%token TOKEN_LOGIC_OP "logic_op"
%token TOKEN_BLEND_CONSTANTS "blend_constants"
%token TOKEN_MRT "mrt"
%token TOKEN_SRC_COLOR_BLEND_FACTOR "src_color_blend_factor"
%token TOKEN_DST_COLOR_BLEND_FACTOR "dst_color_blend_factor"
%token TOKEN_COLOR_BLEND_OP "color_blend_op"
%token TOKEN_SRC_ALPHA_BLEND_FACTOR "src_alpha_blend_factor"
%token TOKEN_DST_ALPHA_BLEND_FACTOR "dst_alpha_blend_factor"
%token TOKEN_ALPHA_BLEND_OP "alpha_blend_op"
%token TOKEN_ONE "one"
%token TOKEN_SRC_COLOR "src_color"
%token TOKEN_ONE_MINUS_SRC_COLOR "one_minus_src_color"
%token TOKEN_DST_COLOR "dst_color"
%token TOKEN_ONE_MINUS_DST_COLOR "one_minus_dst_color"
%token TOKEN_SRC_ALPHA "src_alpha"
%token TOKEN_ONE_MINUS_SRC_ALPHA "one_minus_src_alpha"
%token TOKEN_DST_ALPHA "dst_alpha"
%token TOKEN_ONE_MINUS_DST_ALPHA "one_minus_dst_alpha"
%token TOKEN_CONSTANT_COLOR "constant_color"
%token TOKEN_ONE_MINUS_CONSTANT_COLOR "one_minus_constant_color"
%token TOKEN_CONSTANT_ALPHA "constant_alpha"
%token TOKEN_ONE_MINUS_CONSTANT_ALPHA "one_minus_constant_alpha"
%token TOKEN_SRC_ALPHA_SATURATE "src_alpha_saturate"
%token TOKEN_SRC_ONE_COLOR "src_one_color"
%token TOKEN_ONE_MINUS_SRC_ONE_COLOR "one_minus_src_one_color"
%token TOKEN_SRC_ONE_ALPHA "src_one_alpha"
%token TOKEN_ONE_MINUS_SRC_ONE_ALPHA "one_minus_src_one_alpha"
%token TOKEN_ADD "add"
%token TOKEN_SUBTRACT "subtract"
%token TOKEN_REVERSE_SUBTRACT "reverse_subtract"
%token TOKEN_MIN "min"
%token TOKEN_MAX "max"
%token TOKEN_CLEAR "clear"
%token TOKEN_AND "and"
%token TOKEN_AND_REVERSE "and_reverse"
%token TOKEN_COPY "copy"
%token TOKEN_AND_INVERTED "and_inverted"
%token TOKEN_NO_OP "no_op"
%token TOKEN_XOR "xor"
%token TOKEN_OR "or"
%token TOKEN_NOR "nor"
%token TOKEN_EQUIVALENT "equivalent"
%token TOKEN_OR_REVERSE "or_reverse"
%token TOKEN_COPY_INVERTED "copy_inverted"
%token TOKEN_OR_INVERTED "or_inverted"
%token TOKEN_NAND "nand"
%token TOKEN_SET "set"
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

%token TOKEN_FLOAT "float"
%token TOKEN_FLOAT2 "float2"
%token TOKEN_FLOAT3 "float3"
%token TOKEN_FLOAT4 "float4"
%token TOKEN_INT "int"
%token TOKEN_INT2 "int2"
%token TOKEN_INT3 "int3"
%token TOKEN_INT4 "int4"
%token TOKEN_TEXTURE2D "texture2D"

%type <techniqueExp>         TECHNIQUE_EXP
%type <techniqueExp>         TECHNIQUE_EXP_LIST
%type <shaderExp>            SHADERS_LIST
%type <shader>               SHADERS_ELEM
%type <scopeExp>             SCOPE_EXP
%type <scopeExp>             SCOPE_EXP_LIST
%type <resReserveExp>        RESOURCE_RESERVE_EXP
%type <resReserveExp>        RESOURCE_RESERVE_EXP_LIST
%type <ival>                 INT_VALUE
%type <bval>                 BOOL_VALUE
%type <f4val>                FLOAT4_VALUE
%type <resourceType>         RESOURCE_TYPE
%type <resourceAssignExp>    ASSIGN_EXP
%type <targetProfile>        TARGET_PROFILE
%type <inputBufferExp>       INPUT_BUFFER
%type <inputBufferExp>       INPUT_BUFFER_LIST
%type <inputAttributeExp>    INPUT_ATTRIBUTE
%type <inputAttributeExp>    INPUT_ATTRIBUTE_LIST
%type <attributeType>        ATTRIBUTE_TYPE
%type <primitiveTopology>    PRIMITIVE_TOPOLOGY
%type <renderStateExp>       RENDER_STATE_EXP
%type <renderStateExp>       RENDER_STATE_EXP_LIST
%type <compareOp>            COMPARE_OP
%type <stencilOp>            STENCIL_OP
%type <blendingExp>          BLENDING_EXP
%type <blendingExp>          BLENDING_EXP_LIST
%type <mrtBlendingExp>       MRT_BLENDING_EXP
%type <mrtBlendingExp>       MRT_BLENDING_EXP_LIST
%type <blendFactor>          BLEND_FACTOR
%type <blendOp>              BLEND_OP
%type <logicOp>              LOGIC_OP

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
  | "render_state" ":" RENDER_STATE_EXP_LIST[rs] {
    $$ = $rs;
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

RENDER_STATE_EXP_LIST
  : RENDER_STATE_EXP[state] RENDER_STATE_EXP_LIST[next] {
    $$ = $state;
    $$->next = $next;
  }
  | RENDER_STATE_EXP[state] {
    $$ = $state;
    $$->next = nullptr;
  }
  ;

RENDER_STATE_EXP
  : "primitive_topology" "=" PRIMITIVE_TOPOLOGY[v] ";"{
    $$ = new PrimitiveTopologyExp($v);
  }
  | "depth_test" "=" BOOL_VALUE[v] ";" {
    $$ = new DepthTestExp($v);
  }
  | "depth_write" "=" BOOL_VALUE[v] ";" {
    $$ = new DepthWriteExp($v);
  }
  | "depth_op" "=" COMPARE_OP[op] ";" {
    $$ = new DepthOpExp($op);
  }
  | "stencil_test" "=" BOOL_VALUE[v] ";" {
    $$ = new StencilTestExp($v);
  }
  | "stencil_fail_op" "=" STENCIL_OP[op] ";" {
    $$ = new StencilFailOpExp($op);
  }
  | "stencil_pass_op" "=" STENCIL_OP[op] ";" {
    $$ = new StencilPassOpExp($op);
  }
  | "stencil_depth_fail_op" "=" STENCIL_OP[op] ";" {
    $$ = new StencilDepthFailOpExp($op);
  }
  | "stencil_compare_op" "=" COMPARE_OP[op] ";" {
    $$ = new StencilCompareOpExp($op);
  }
  | "stencil_ref_val" "=" INT_VALUE[v] ";" {
    $$ = new StencilReferenceValueExp($v);
  }
  | "blending" ":" BLENDING_EXP_LIST[exps] {
    $$ = $exps;
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

COMPARE_OP
  : "never" {
   $$ = gapi::CompareOp::Never;
  }
  | "less" {
   $$ = gapi::CompareOp::Less;
  }
  | "equal" {
   $$ = gapi::CompareOp::Never;
  }
  | "less_or_equal" {
   $$ = gapi::CompareOp::LessOrEqual;
  }
  | "greater" {
   $$ = gapi::CompareOp::Greater;
  }
  | "not_equal" {
   $$ = gapi::CompareOp::NotEqual;
  }
  | "greater_or_equal" {
   $$ = gapi::CompareOp::GreaterOrEqual;
  }
  | "always" {
   $$ = gapi::CompareOp::Always;
  }
  ;

STENCIL_OP
  : "keep" {
    $$ = gapi::StencilOp::Keep;
  }
  | "zero" {
    $$ = gapi::StencilOp::Zero;
  }
  | "replace" {
    $$ = gapi::StencilOp::Replace;
  }
  | "inc_and_clamp" {
    $$ = gapi::StencilOp::IncrementAndClamp;
  }
  | "dec_and_clamp" {
    $$ = gapi::StencilOp::DecrementAndClamp;
  }
  | "invert" {
    $$ = gapi::StencilOp::Invert;
  }
  | "inc_and_wrap" {
    $$ = gapi::StencilOp::IncrementAndWrap;
  }
  | "dec_and_wrap" {
    $$ = gapi::StencilOp::DecrementAndWrap;
  }
  ;

BLENDING_EXP_LIST
  : BLENDING_EXP[exp] BLENDING_EXP_LIST[next] {
    $$ = $exp;
    $$->next = $next;
  }
  | BLENDING_EXP[exp] {
    $$ = $exp;
    $$->next = nullptr;
  }

BLENDING_EXP
  : "logic_op_enabled" "=" BOOL_VALUE[v] ";" {
    $$ = new LogicOpEnablingExp($v);
  }
  | "logic_op" "=" LOGIC_OP[op] ";" {
    $$ = new LogicOpExp($op);
  }
  | "blend_constants" "=" FLOAT4_VALUE[v] ";" {
    $$ = new BlendConstants($v);
  }
  | "mrt" ":" MRT_BLENDING_EXP_LIST[exps] {
    $$ = $exps;
  }
  ;

MRT_BLENDING_EXP_LIST
  : MRT_BLENDING_EXP[el] MRT_BLENDING_EXP_LIST[next] {
    $$ = $el;
    $$->next = $next;
  }
  | MRT_BLENDING_EXP[el] {
    $$ = $el;
    $$->next = nullptr;
  }
  ;

MRT_BLENDING_EXP
  : "has_blending" "=" BOOL_VALUE[v] ";" {
    $$ = new MrtBlendingEnabledExp($v);
  }
  | "src_color_blend_factor" "=" BLEND_FACTOR[f] ";" {
    $$ = new MrtSrcColorBlendFactorExp($f);
  }
  | "dst_color_blend_factor" "=" BLEND_FACTOR[f] ";" {
    $$ = new MrtDstColorBlendFactorExp($f);
  }
  | "color_blend_op" "=" BLEND_OP[op] ";" {
    $$ = new MrtColorBlendOpExp($op);
  }
  | "src_alpha_blend_factor" "=" BLEND_FACTOR[f] ";" {
    $$ = new MrtSrcAlphaBlendFactorExp($f);
  }
  | "dst_alpha_blend_factor" "=" BLEND_FACTOR[f] ";" {
    $$ = new MrtDstAlphaBlendFactorExp($f);
  }
  | "alpha_blend_op" "=" BLEND_OP[op] ";" {
    $$ = new MrtAlphaBlendOpExp($op);
  }
  ;

BLEND_FACTOR
  : "zero" {
    $$ = gapi::BlendFactor::Zero;
  }
  | "one" {
    $$ = gapi::BlendFactor::One;
  }
  | "src_color" {
    $$ = gapi::BlendFactor::SrcColor;
  }
  | "one_minus_src_color" {
    $$ = gapi::BlendFactor::OneMinusSrcColor;
  }
  | "dst_color" {
    $$ = gapi::BlendFactor::DstColor;
  }
  | "one_minus_dst_color" {
    $$ = gapi::BlendFactor::OneMinusDstColor;
  }
  | "src_alpha" {
    $$ = gapi::BlendFactor::SrcAlpha;
  }
  | "one_minus_src_alpha" {
    $$ = gapi::BlendFactor::OneMinusSrcAlpha;
  }
  | "dst_alpha" {
    $$ = gapi::BlendFactor::DstAlpha;
  }
  | "one_minus_dst_alpha" {
    $$ = gapi::BlendFactor::OneMinusDstAlpha;
  }
  | "constant_color" {
    $$ = gapi::BlendFactor::ConstantColor;
  }
  | "one_minus_constant_color" {
    $$ = gapi::BlendFactor::OneMinusConstantColor;
  }
  | "constant_alpha" {
    $$ = gapi::BlendFactor::ConstantAlpha;
  }
  | "one_minus_constant_alpha" {
    $$ = gapi::BlendFactor::OneMinusConstantAlpha;
  }
  | "src_alpha_saturate" {
    $$ = gapi::BlendFactor::SrcAlphaSaturate;
  }
  | "src_one_color" {
    $$ = gapi::BlendFactor::SrcOneColor;
  }
  | "one_minus_src_one_color" {
    $$ = gapi::BlendFactor::OneMinusSrcOneColor;
  }
  | "src_one_alpha" {
    $$ = gapi::BlendFactor::SrcOneAlpha;
  }
  | "one_minus_src_one_alpha" {
    $$ = gapi::BlendFactor::OneMinusSrcOneAlpha;
  }
  ;

BLEND_OP
  : "add" {
    $$ = gapi::BlendOp::Add;
  }
  | "subtract" {
    $$ = gapi::BlendOp::Subtract;
  }
  | "reverse_subtract" {
    $$ = gapi::BlendOp::ReverseSubtract;
  }
  | "min" {
    $$ = gapi::BlendOp::Min;
  }
  | "max" {
    $$ = gapi::BlendOp::Max;
  }
  ;

LOGIC_OP
  : "clear" {
    $$ = gapi::LogicOp::Clear;
  }
  | "and" {
    $$ = gapi::LogicOp::And;
  }
  | "and_reverse" {
    $$ = gapi::LogicOp::AndReverse;
  }
  | "copy" {
    $$ = gapi::LogicOp::Copy;
  }
  | "and_inverted" {
    $$ = gapi::LogicOp::AndInverted;
  }
  | "no_op" {
    $$ = gapi::LogicOp::NoOp;
  }
  | "xor" {
    $$ = gapi::LogicOp::Xor;
  }
  | "or" {
    $$ = gapi::LogicOp::Or;
  }
  | "nor" {
    $$ = gapi::LogicOp::Nor;
  }
  | "equivalent" {
    $$ = gapi::LogicOp::Equivalent;
  }
  | "invert" {
    $$ = gapi::LogicOp::Invert;
  }
  | "or_reverse" {
    $$ = gapi::LogicOp::OrReverse;
  }
  | "copy_inverted" {
    $$ = gapi::LogicOp::CopyInverted;
  }
  | "or_inverted" {
    $$ = gapi::LogicOp::OrInverted;
  }
  | "nand" {
    $$ = gapi::LogicOp::Nand;
  }
  | "set" {
    $$ = gapi::LogicOp::Set;
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

FLOAT4_VALUE
  : "float4" "(" TOKEN_FLOAT_VAL[v0] "," TOKEN_FLOAT_VAL[v1] "," TOKEN_FLOAT_VAL[v2] "," TOKEN_FLOAT_VAL[v3] ")" {
    $$ = float4($v0, $v1, $v2, $v3);
  }
  ;

INT_VALUE
  : TOKEN_INT_VAL[v] {
    $$ = $v;
  }
  ;

BOOL_VALUE
  : TOKEN_BOOL_VAL[v] {
    $$ = $v;
  }

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