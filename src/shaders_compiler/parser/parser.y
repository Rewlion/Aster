
%{
  #include <shaders_compiler/compiler.h>
  #include <shaders_compiler/limits.h>

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
  DescriptorSetReserveExp* dsetExp;

  ResourceDeclaration* resourceDeclaration;
  ResourceType resourceType;
  ResourceAssignExp* resourceAssignExp;

  gapi::AttributeType attributeType;
  InputBufferExp* inputBufferExp;
  InputAttributeExp* inputAttributeExp;

  RenderStateExp* renderStateExp;
  gapi::CullMode cullMode;
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

%token <fval> TFX_TOKEN_FLOAT_VAL
%token <ival> TFX_TOKEN_INT_VAL
%token <sval> TFX_TOKEN_NAME_VAL
%token <bval> TFX_TOKEN_BOOL_VAL
%token <sval> TFX_TOKEN_HLSL_CODE

%token TFX_TOKEN_AT "@"
%token TFX_TOKEN_COLON ":"
%token TFX_TOKEN_SEMICOLON ";"
%token TFX_TOKEN_EQUAL_OP "="
%token TFX_TOKEN_LEFT_PARENTHESIS "("
%token TFX_TOKEN_RIGHT_PARENTHESIS ")"
%token TFX_TOKEN_LEFT_BRACKET "{"
%token TFX_TOKEN_RIGHT_BRACKET "}"
%token TFX_TOKEN_COMMA ","
%token TFX_TOKEN_MINUS "-"
%token TFX_TOKEN_INPUT "input"
%token TFX_TOKEN_BUFFER "buffer"
%token TFX_TOKEN_SCOPE "scope"
%token TFX_TOKEN_RESERVE "reserve"
%token TFX_TOKEN_REGISTER "register"
%token TFX_TOKEN_TEXTURE "texture"
%token TFX_TOKEN_SAMPLER "sampler"
%token TFX_TOKEN_CBUFFER "cbuffer"
%token TFX_TOKEN_EXTERN "extern"
%token TFX_TOKEN_CHANNEL "channel"
%token TFX_TOKEN_DESCRIPTOR_SET "descriptor_set"
%token TFX_TOKEN_TECHNIQUE "technique"
%token TFX_TOKEN_TECHNIQUE_MACRO "technique_macro"
%token TFX_TOKEN_SUPPORT "support"
%token TFX_TOKEN_ACTIVATE "activate"
%token TFX_TOKEN_COMPILE "compile"
%token TFX_TOKEN_RENDER_STATE "render_state"
%token TFX_TOKEN_CULL_MODE "cull_mode"
%token TFX_TOKEN_NONE "none"
%token TFX_TOKEN_CCW "ccw"
%token TFX_TOKEN_CW "cw"
%token TFX_TOKEN_PRIMITIVE_TOPOLOGY "primitive_topology"
%token TFX_TOKEN_PT_POINT_LIST "point_list"
%token TFX_TOKEN_PT_LINE_LIST "line_list"
%token TFX_TOKEN_PT_LINE_STRIP "line_strip"
%token TFX_TOKEN_PT_TRIANGLE_LIST "triangle_list"
%token TFX_TOKEN_PT_TRIANGLE_FAN "triangle_fan"
%token TFX_TOKEN_PT_TRIANGLE_STRIP "triangle_strip"
%token TFX_TOKEN_PT_LINE_LIST_WITH_ADJACENCY "line_list_with_adjacency"
%token TFX_TOKEN_PT_LINE_STRIP_WITH_ADJACENCY "line_strip_with_adjacency"
%token TFX_TOKEN_PT_TRIANGLE_LIST_WITH_ADJACENCY "triangle_list_with_adjacency"
%token TFX_TOKEN_PT_TRIANGLE_STRIP_WITH_ADJACENCY "triangle_strip_with_adjacency"
%token TFX_TOKEN_PT_PATCH_LIST "patch_list"
%token TFX_TOKEN_DEPTH "depth"
%token TFX_TOKEN_STENCIL "stencil"
%token TFX_TOKEN_TEST "test"
%token TFX_TOKEN_WRITE "write"
%token TFX_TOKEN_FAIL_OP "fail_op"
%token TFX_TOKEN_PASS_OP "pass_op"
%token TFX_TOKEN_DEPTH_FAIL_OP "depth_fail_op"
%token TFX_TOKEN_COMPARE_OP "compare_op"
%token TFX_TOKEN_REFERENCE_VALUE "ref_val"
%token TFX_TOKEN_NEVER "never"
%token TFX_TOKEN_LESS "less"
%token TFX_TOKEN_EQUAL "equal"
%token TFX_TOKEN_LESS_OR_EQUAL "less_or_equal"
%token TFX_TOKEN_GREATER "greater"
%token TFX_TOKEN_NOT_EQUAL "not_equal"
%token TFX_TOKEN_GREATER_OR_EQUAL "greater_or_equal"
%token TFX_TOKEN_ALWAYS "always"
%token TFX_TOKEN_KEEP "keep"
%token TFX_TOKEN_ZERO "zero"
%token TFX_TOKEN_REPLACE "replace"
%token TFX_TOKEN_INCREMENT_AND_CLAMP "inc_and_clamp"
%token TFX_TOKEN_DECREMENT_AND_CLAMP "dec_and_clamp"
%token TFX_TOKEN_INVERT "invert"
%token TFX_TOKEN_INCREMENT_AND_WRAP "inc_and_wrap"
%token TFX_TOKEN_DECREMENT_AND_WRAP "dec_and_wrap"
%token TFX_TOKEN_BLENDING "blending"
%token TFX_TOKEN_HAS_BLANDING "has_blending"
%token TFX_TOKEN_LOGIC_OP_ENABLED "logic_op_enabled"
%token TFX_TOKEN_LOGIC_OP "logic_op"
%token TFX_TOKEN_BLEND_CONSTANTS "blend_constants"
%token TFX_TOKEN_MRT "mrt"
%token TFX_TOKEN_SRC_COLOR_BLEND_FACTOR "src_color_blend_factor"
%token TFX_TOKEN_DST_COLOR_BLEND_FACTOR "dst_color_blend_factor"
%token TFX_TOKEN_COLOR_BLEND_OP "color_blend_op"
%token TFX_TOKEN_SRC_ALPHA_BLEND_FACTOR "src_alpha_blend_factor"
%token TFX_TOKEN_DST_ALPHA_BLEND_FACTOR "dst_alpha_blend_factor"
%token TFX_TOKEN_ALPHA_BLEND_OP "alpha_blend_op"
%token TFX_TOKEN_ONE "one"
%token TFX_TOKEN_SRC_COLOR "src_color"
%token TFX_TOKEN_ONE_MINUS_SRC_COLOR "one_minus_src_color"
%token TFX_TOKEN_DST_COLOR "dst_color"
%token TFX_TOKEN_ONE_MINUS_DST_COLOR "one_minus_dst_color"
%token TFX_TOKEN_SRC_ALPHA "src_alpha"
%token TFX_TOKEN_ONE_MINUS_SRC_ALPHA "one_minus_src_alpha"
%token TFX_TOKEN_DST_ALPHA "dst_alpha"
%token TFX_TOKEN_ONE_MINUS_DST_ALPHA "one_minus_dst_alpha"
%token TFX_TOKEN_CONSTANT_COLOR "constant_color"
%token TFX_TOKEN_ONE_MINUS_CONSTANT_COLOR "one_minus_constant_color"
%token TFX_TOKEN_CONSTANT_ALPHA "constant_alpha"
%token TFX_TOKEN_ONE_MINUS_CONSTANT_ALPHA "one_minus_constant_alpha"
%token TFX_TOKEN_SRC_ALPHA_SATURATE "src_alpha_saturate"
%token TFX_TOKEN_SRC_ONE_COLOR "src_one_color"
%token TFX_TOKEN_ONE_MINUS_SRC_ONE_COLOR "one_minus_src_one_color"
%token TFX_TOKEN_SRC_ONE_ALPHA "src_one_alpha"
%token TFX_TOKEN_ONE_MINUS_SRC_ONE_ALPHA "one_minus_src_one_alpha"
%token TFX_TOKEN_ADD "add"
%token TFX_TOKEN_SUBTRACT "subtract"
%token TFX_TOKEN_REVERSE_SUBTRACT "reverse_subtract"
%token TFX_TOKEN_MIN "min"
%token TFX_TOKEN_MAX "max"
%token TFX_TOKEN_CLEAR "clear"
%token TFX_TOKEN_AND "and"
%token TFX_TOKEN_AND_REVERSE "and_reverse"
%token TFX_TOKEN_COPY "copy"
%token TFX_TOKEN_AND_INVERTED "and_inverted"
%token TFX_TOKEN_NO_OP "no_op"
%token TFX_TOKEN_XOR "xor"
%token TFX_TOKEN_OR "or"
%token TFX_TOKEN_NOR "nor"
%token TFX_TOKEN_EQUIVALENT "equivalent"
%token TFX_TOKEN_OR_REVERSE "or_reverse"
%token TFX_TOKEN_COPY_INVERTED "copy_inverted"
%token TFX_TOKEN_OR_INVERTED "or_inverted"
%token TFX_TOKEN_NAND "nand"
%token TFX_TOKEN_SET "set"
%token TFX_TOKEN_TARGET_VS_6_0 "vs_6_0"
%token TFX_TOKEN_TARGET_VS_6_1 "vs_6_1"
%token TFX_TOKEN_TARGET_VS_6_2 "vs_6_2"
%token TFX_TOKEN_TARGET_VS_6_3 "vs_6_3"
%token TFX_TOKEN_TARGET_VS_6_4 "vs_6_4"
%token TFX_TOKEN_TARGET_VS_6_5 "vs_6_5"
%token TFX_TOKEN_TARGET_VS_6_6 "vs_6_6"
%token TFX_TOKEN_TARGET_VS_6_7 "vs_6_7"
%token TFX_TOKEN_TARGET_PS_6_0 "ps_6_0"
%token TFX_TOKEN_TARGET_PS_6_1 "ps_6_1"
%token TFX_TOKEN_TARGET_PS_6_2 "ps_6_2"
%token TFX_TOKEN_TARGET_PS_6_3 "ps_6_3"
%token TFX_TOKEN_TARGET_PS_6_4 "ps_6_4"
%token TFX_TOKEN_TARGET_PS_6_5 "ps_6_5"
%token TFX_TOKEN_TARGET_PS_6_6 "ps_6_6"
%token TFX_TOKEN_TARGET_PS_6_7 "ps_6_7"

%token TFX_TOKEN_FLOAT "float"
%token TFX_TOKEN_FLOAT2 "float2"
%token TFX_TOKEN_FLOAT3 "float3"
%token TFX_TOKEN_FLOAT4 "float4"
%token TFX_TOKEN_FLOAT4_u8 "float4_u8"
%token TFX_TOKEN_FLOAT4X4 "float4x4"
%token TFX_TOKEN_INT "int"
%token TFX_TOKEN_INT2 "int2"
%token TFX_TOKEN_INT3 "int3"
%token TFX_TOKEN_INT4 "int4"
%token TFX_TOKEN_TEXTURE2D "Texture2D"
%token TFX_TOKEN_TEXTURE_CUBE "TextureCube"

%type <techniqueExp>         TECHNIQUE_EXP
%type <techniqueExp>         TECHNIQUE_EXP_LIST
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
%type <cullMode>             CULL_MODE
%type <primitiveTopology>    PRIMITIVE_TOPOLOGY
%type <renderStateExp>       RENDER_STATE_EXP
%type <renderStateExp>       RENDER_STATE_EXP_LIST
%type <renderStateExp>       DEPTH_EXP
%type <renderStateExp>       DEPTH_EXP_LIST
%type <renderStateExp>       STENCIL_EXP
%type <renderStateExp>       STENCIL_EXP_LIST
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
  | TECHNIQUE_MACRO_DECLARATION {
  }
  ;

TECHNIQUE_MACRO_DECLARATION
  : "technique_macro" TFX_TOKEN_NAME_VAL[name] "{" TECHNIQUE_EXP_LIST[exps] "}" ";" {
    const bool isOk = compiler.onTechniqueMacroDeclaration(new TechniqueMacroDeclarationExp{$name, $exps});
    if (!isOk)
      YYABORT;
}

TECHNIQUE_DECLARATION
  : "technique" TFX_TOKEN_NAME_VAL[name] "{" TECHNIQUE_EXP_LIST[exps] "}" ";" {
    const bool isOk = compiler.onTechniqueDeclaration(new TechniqueDeclarationExp{$name, $exps});
    if (!isOk)
      YYABORT;
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
  : "support" "scope" TFX_TOKEN_NAME_VAL[name] ";" {
    $$ = new ScopeSupportExp($name);
  }
  | "activate" "scope" TFX_TOKEN_NAME_VAL[name] ";" {
    $$ = new ScopeActivateExp($name);
  }
  | "compile" "(" TARGET_PROFILE[target] "," TFX_TOKEN_NAME_VAL[entry] ")" ";" {
    $$ = new CompileExp($target, $entry);
  }
  | TFX_TOKEN_HLSL_CODE[code] {
    $$ = new HlslExp($code);
  }
  | "render_state" ":" RENDER_STATE_EXP_LIST[rs] {
    $$ = $rs;
  }
  | TFX_TOKEN_NAME_VAL[name] "(" ")" ";" {
    $$ = new TechniqueMacroInvokeExp($name);
  }
  ;

RENDER_STATE_EXP_LIST
  : RENDER_STATE_EXP[state] RENDER_STATE_EXP_LIST[next] {
    $$ = $state;
    $$->next = $next;
  }
  | RENDER_STATE_EXP[state] {
    $$ = $state;
  }
  ;

RENDER_STATE_EXP
  : "cull_mode" "=" CULL_MODE[m] ";" {
    $$ = new CullModeExp($m);
  }
  | "primitive_topology" "=" PRIMITIVE_TOPOLOGY[v] ";"{
    $$ = new PrimitiveTopologyExp($v);
  }
  | "input" ":" INPUT_BUFFER_LIST[buffers] {
    $$ = new InputExp($buffers);
  }
  | "depth" ":" DEPTH_EXP_LIST[exps] {
    $$ = $exps;
  }
  | "stencil" ":" STENCIL_EXP_LIST[exps] {
    $$ = $exps;
  }
  | "blending" ":" BLENDING_EXP_LIST[exps] {
    $$ = $exps;
  }
  ;

DEPTH_EXP_LIST
  : DEPTH_EXP[exp] DEPTH_EXP_LIST[next] {
    $$ = $exp;
    $$->next = $next;
  }
  | DEPTH_EXP[exp] {
    $$ = $exp;
  }
  ;

DEPTH_EXP
  : "test" "=" BOOL_VALUE[v] ";" {
    $$ = new DepthTestExp($v);
  }
  | "write" "=" BOOL_VALUE[v] ";" {
    $$ = new DepthWriteExp($v);
  }
  | "compare_op" "=" COMPARE_OP[op] ";" {
    $$ = new DepthOpExp($op);
  }
  ;

STENCIL_EXP_LIST
  : STENCIL_EXP[exp] STENCIL_EXP_LIST[next] {
    $$ = $exp;
    $$->next = $next;
  }
  | STENCIL_EXP[exp] {
    $$ = $exp;
  }
  ;

STENCIL_EXP
  : "test" "=" BOOL_VALUE[v] ";" {
    $$ = new StencilTestExp($v);
  }
  | "fail_op" "=" STENCIL_OP[op] ";" {
    $$ = new StencilFailOpExp($op);
  }
  | "pass_op" "=" STENCIL_OP[op] ";" {
    $$ = new StencilPassOpExp($op);
  }
  | "depth_fail_op" "=" STENCIL_OP[op] ";" {
    $$ = new StencilDepthFailOpExp($op);
  }
  | "compare_op" "=" COMPARE_OP[op] ";" {
    $$ = new StencilCompareOpExp($op);
  }
  | "ref_val" "=" INT_VALUE[v] ";" {
    $$ = new StencilReferenceValueExp($v);
  }
  ;

CULL_MODE
  : "none" {
    $$ = gapi::CullMode::None;
  }
  | "cw" {
    $$ = gapi::CullMode::CW;
  }
  | "ccw" {
    $$ = gapi::CullMode::CCW;
  }

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
  | "triangle_strip" {
    $$ = gapi::PrimitiveTopology::TriangleStrip;
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
  : ATTRIBUTE_TYPE[type] TFX_TOKEN_NAME_VAL[name] ":" TFX_TOKEN_NAME_VAL[semantic] ";" {
    $$ = new InputAttributeExp($type, $name, $semantic);
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
  | "mrt" "(" INT_VALUE[v] ")" ":" MRT_BLENDING_EXP_LIST[exps] {
    $exps->n = $v;
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
  : "scope" TFX_TOKEN_NAME_VAL[name] "{" SCOPE_EXP_LIST[exps] "}" ";" {
    const bool isOk = compiler.onScopeDeclaration(new ScopeDeclarationExp{$name, $exps});
    if (!isOk)
      YYABORT;
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
  : "reserve" ":" RESOURCE_RESERVE_EXP_LIST[exps] {
    $$ = new ShadersResourcesReserveExp($exps);
  }
  | RESOURCE_TYPE[type] TFX_TOKEN_NAME_VAL[name] "=" ASSIGN_EXP[exps] ";" {
    $$ = new ResourceDeclarationExp($type, $name, $exps);
  }
  | ATTRIBUTE_TYPE[type] TFX_TOKEN_NAME_VAL[name] "=" ASSIGN_EXP[exps] ";" {
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
  | "register" "(" "texture" "," INT_VALUE[v0] ")" {
    $$ = new RegistersReserveExp(RegistersReserveExp::Register::Texture, $v0, $v0);
  }
  | "register" "(" "sampler" "," INT_VALUE[v0] "-" INT_VALUE[v1] ")" {
    $$ = new RegistersReserveExp(RegistersReserveExp::Register::Sampler, $v0, $v1);
  }
  | "register" "(" "sampler" "," INT_VALUE[v0] ")" {
    $$ = new RegistersReserveExp(RegistersReserveExp::Register::Sampler, $v0, $v0);
  }
  | "register" "(" "buffer" "," INT_VALUE[v0] "-" INT_VALUE[v1] ")" {
    $$ = new RegistersReserveExp(RegistersReserveExp::Register::Buffer, $v0, $v1);
  }
  | "register" "(" "buffer" "," INT_VALUE[v0] ")" {
    $$ = new RegistersReserveExp(RegistersReserveExp::Register::Buffer, $v0, $v0);
  }
  | "descriptor_set" "(" INT_VALUE[v] ")" {
    $$ = new DescriptorSetReserveExp($v);
  }
  ;

ASSIGN_EXP
  : "extern" "(" TFX_TOKEN_NAME_VAL[name] ")" {
    $$ = new AccessResource(ResourceAccessType::Extern, $name);
  }
  | "channel" "(" TFX_TOKEN_NAME_VAL[name] ")" {
    $$ = new AccessResource(ResourceAccessType::Channel, $name);
  }
  ;

RESOURCE_TYPE
  : TFX_TOKEN_TEXTURE2D {
    $$ = ResourceType::Texture2D;
  }
  | TFX_TOKEN_TEXTURE_CUBE {
    $$ = ResourceType::TextureCube;
  }
  | TFX_TOKEN_SAMPLER {
    $$ = ResourceType::Sampler;
  }
  ;

ATTRIBUTE_TYPE
  : TFX_TOKEN_INT {
    $$ = gapi::AttributeType::Int;
  }
  | TFX_TOKEN_INT2 {
    $$ = gapi::AttributeType::Int2;
  }
  | TFX_TOKEN_INT3 {
    $$ = gapi::AttributeType::Int3;
  }
  | TFX_TOKEN_INT4 {
    $$ = gapi::AttributeType::Int4;
  }
  | TFX_TOKEN_FLOAT {
    $$ = gapi::AttributeType::Float;
  }
  | TFX_TOKEN_FLOAT2 {
    $$ = gapi::AttributeType::Float2;
  }
  | TFX_TOKEN_FLOAT3 {
    $$ = gapi::AttributeType::Float3;
  }
  | TFX_TOKEN_FLOAT4 {
    $$ = gapi::AttributeType::Float4;
  }
  | TFX_TOKEN_FLOAT4_u8 {
    $$ = gapi::AttributeType::Float4_u8;
  }
  | TFX_TOKEN_FLOAT4X4 {
    $$ = gapi::AttributeType::Float4x4;
  }
  ;

FLOAT4_VALUE
  : "float4" "(" TFX_TOKEN_FLOAT_VAL[v0] "," TFX_TOKEN_FLOAT_VAL[v1] "," TFX_TOKEN_FLOAT_VAL[v2] "," TFX_TOKEN_FLOAT_VAL[v3] ")" {
    $$ = float4($v0, $v1, $v2, $v3);
  }
  ;

INT_VALUE
  : TFX_TOKEN_INT_VAL[v] {
    $$ = $v;
  }
  ;

BOOL_VALUE
  : TFX_TOKEN_BOOL_VAL[v] {
    $$ = $v;
  }

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
  logerror("parsing error: {} [{}:{}]", msg, compiler.getCurrentFileName(), compiler.getLine());
  compiler.markCompilationFailed();
}