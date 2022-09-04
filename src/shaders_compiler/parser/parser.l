%{
    #define YY_DECL int shlex(ShadersSystem::Compiler& compiler)

    #include <engine/log.h>

    #include <stdio.h>
    #include <filesystem>
    #include "parser.tab.hpp"

    void sherror(ShadersSystem::Compiler& compiler, const char* msg);
    extern int shlex(ShadersSystem::Compiler& compiler);

    string codeBuf;

    namespace fs = std::filesystem;
%}

%option yylineno
%option nounistd
%option never-interactive
%option noyywrap
%option prefix="sh"

%x hlsl
%x include

%%

#hlsl {
  codeBuf = "";
  BEGIN(hlsl);
}

<hlsl>. {
  codeBuf += *yytext;
}

<hlsl>\n {
  codeBuf += "\n";
}

<hlsl>#end {
  BEGIN(INITIAL);
  shlval.sval = strdup(codeBuf.c_str());
  return TFX_TOKEN_HLSL_CODE;
}


<include>[ \t]* {

}

<include>["][^ \t\r\n]+["] {
  char* fileName = yytext + 1;
  fileName[strlen(yytext) - 2] = '\0';

  const string currentDir = fs::path(compiler.getCurrentCompilationFile()).parent_path().string();
  const string file = fmt::format("{}/{}", currentDir, fileName);
  yyin = fopen( file.c_str(), "r" );

  if (!yyin)
  {
    const string error = fmt::format("can't include file `{}`: File not found", fileName);
    sherror(compiler, error.c_str());
    yyterminate();
  }

	yypush_buffer_state(yy_create_buffer( yyin, YY_BUF_SIZE ));
  BEGIN(INITIAL);
}

<include>. {
  const std::string error = fmt::format("unexpected string: {}", yytext);
  sherror(compiler, error.c_str());
  yyterminate();
}

#include[ ] {
  BEGIN(include);
}


<<EOF>> {
  fclose(yyin);
  yypop_buffer_state();
  if ( !YY_CURRENT_BUFFER )
  {
    yyterminate();
  }
}

[/][/][^\n]* ; // comment

[ \t\n]* ;

@ {
  return TFX_TOKEN_AT;
}

: {
  return TFX_TOKEN_COLON;
}

; {
  return TFX_TOKEN_SEMICOLON;
}

= {
  return TFX_TOKEN_EQUAL_OP;
}

\( {
  return TFX_TOKEN_LEFT_PARENTHESIS;
}

\) {
  return TFX_TOKEN_RIGHT_PARENTHESIS;
}

\{ {
  return TFX_TOKEN_LEFT_BRACKET;
}

\} {
  return TFX_TOKEN_RIGHT_BRACKET;
}

, {
  return TFX_TOKEN_COMMA;
}

- {
  return TFX_TOKEN_MINUS;
}

scope {
  return TFX_TOKEN_SCOPE;
}

reserve {
  return TFX_TOKEN_RESERVE;
}

register {
  return TFX_TOKEN_REGISTER;
}

texture {
  return TFX_TOKEN_TEXTURE;
}

sampler {
  return TFX_TOKEN_SAMPLER;
}

input {
  return TFX_TOKEN_INPUT;
}

buffer {
  return TFX_TOKEN_BUFFER;
}

cbuffer {
  return TFX_TOKEN_CBUFFER;
}

extern {
  return TFX_TOKEN_EXTERN;
}

channel {
  return TFX_TOKEN_CHANNEL;
}

descriptor_set {
  return TFX_TOKEN_DESCRIPTOR_SET;
}

false {
  shlval.bval = false;
  return TFX_TOKEN_BOOL_VAL;
}

true {
  shlval.bval = true;
  return TFX_TOKEN_BOOL_VAL;
}

no {
  shlval.bval = false;
  return TFX_TOKEN_BOOL_VAL;
}

yes {
  shlval.bval = true;
  return TFX_TOKEN_BOOL_VAL;
}

[0-9]+ {
  shlval.ival = atoi(yytext);
  return TFX_TOKEN_INT_VAL;
}

[0-9]+[.][0-9]* { 
  shlval.fval = atof(yytext);
  return TFX_TOKEN_FLOAT_VAL;
}

float {
  return TFX_TOKEN_FLOAT;
}

float2 {
  return TFX_TOKEN_FLOAT2;
}

float3 {
  return TFX_TOKEN_FLOAT3;
}

float4 {
  return TFX_TOKEN_FLOAT4;
}

float4x4 {
  return TFX_TOKEN_FLOAT4X4;
}

int {
  return TFX_TOKEN_INT;
}

int2 {
  return TFX_TOKEN_INT2;
}

int3 {
  return TFX_TOKEN_INT3;
}

int4 {
  return TFX_TOKEN_INT4;
}

Texture2D {
  return TFX_TOKEN_TEXTURE2D;
}

TextureCube {
  return TFX_TOKEN_TEXTURE_CUBE;
}

technique {
  return TFX_TOKEN_TECHNIQUE;
}

technique_macro {
  return TFX_TOKEN_TECHNIQUE_MACRO;
}

activate {
  return TFX_TOKEN_ACTIVATE;
}

support {
  return TFX_TOKEN_SUPPORT;
}

compile {
  return TFX_TOKEN_COMPILE;
}

render_state {
  return TFX_TOKEN_RENDER_STATE;
}

primitive_topology {
  return TFX_TOKEN_PRIMITIVE_TOPOLOGY;
}

point_list {
  return TFX_TOKEN_PT_POINT_LIST;
}

line_list {
  return TFX_TOKEN_PT_LINE_LIST;
}

line_strip {
  return TFX_TOKEN_PT_LINE_STRIP;
}

triangle_list {
  return TFX_TOKEN_PT_TRIANGLE_LIST;
}

triangle_fan {
  return TFX_TOKEN_PT_TRIANGLE_FAN;
}

triangle_strip {
  return TFX_TOKEN_PT_TRIANGLE_STRIP;
}

line_list_with_adjacency {
  return TFX_TOKEN_PT_LINE_LIST_WITH_ADJACENCY;
}

line_strip_with_adjacency {
  return TFX_TOKEN_PT_LINE_STRIP_WITH_ADJACENCY;
}

triangle_list_with_adjacency {
  return TFX_TOKEN_PT_TRIANGLE_LIST_WITH_ADJACENCY;
}

triangle_strip_with_adjacency {
  return TFX_TOKEN_PT_TRIANGLE_STRIP_WITH_ADJACENCY;
}

patch_list {
  return TFX_TOKEN_PT_PATCH_LIST;
}

"depth" {
  return TFX_TOKEN_DEPTH;
}

"stencil" {
  return TFX_TOKEN_STENCIL;
}

"test" {
  return TFX_TOKEN_TEST;
}

"write" {
  return TFX_TOKEN_WRITE;
}

"fail_op" {
  return TFX_TOKEN_FAIL_OP;
}

"pass_op" {
  return TFX_TOKEN_PASS_OP;
}

"depth_fail_op" {
  return TFX_TOKEN_DEPTH_FAIL_OP;
}

"compare_op" {
  return TFX_TOKEN_COMPARE_OP;
}

"ref_val" {
  return TFX_TOKEN_REFERENCE_VALUE;
}

"never" {
  return TFX_TOKEN_NEVER;
}

"less" {
  return TFX_TOKEN_LESS;
}

"equal" {
  return TFX_TOKEN_EQUAL;
}

"less_or_equal" {
  return TFX_TOKEN_LESS_OR_EQUAL;
}

"greater" {
  return TFX_TOKEN_GREATER;
}

"not_equal" {
  return TFX_TOKEN_NOT_EQUAL;
}

"greater_or_equal" {
  return TFX_TOKEN_GREATER_OR_EQUAL;
}

"always" {
  return TFX_TOKEN_ALWAYS;
}

"keep" {
  return TFX_TOKEN_KEEP;
}

"zero" {
  return TFX_TOKEN_ZERO;
}

"replace" {
  return TFX_TOKEN_REPLACE;
}

"inc_clamp" {
  return TFX_TOKEN_INCREMENT_AND_CLAMP;
}

"dec_clamp" {
  return TFX_TOKEN_DECREMENT_AND_CLAMP;
}

"invert" {
  return TFX_TOKEN_INVERT;
}

"inc_and_wrap" {
  return TFX_TOKEN_INCREMENT_AND_WRAP;
}

"dec_and_wrap" {
  return TFX_TOKEN_DECREMENT_AND_WRAP;
}

"blending" {
  return TFX_TOKEN_BLENDING;
}

"has_blending" {
  return TFX_TOKEN_HAS_BLANDING;
}

"logic_op_enabled" {
  return TFX_TOKEN_LOGIC_OP_ENABLED;
}

"logic_op" {
  return TFX_TOKEN_LOGIC_OP;
}

"blend_constants" {
  return TFX_TOKEN_BLEND_CONSTANTS;
}

"mrt" {
  return TFX_TOKEN_MRT;
}

"src_color_blend_factor" {
  return TFX_TOKEN_SRC_COLOR_BLEND_FACTOR;
}

"dst_color_blend_factor" {
  return TFX_TOKEN_DST_COLOR_BLEND_FACTOR;
}

"color_blend_op" {
  return TFX_TOKEN_COLOR_BLEND_OP;
}

"src_alpha_blend_factor" {
  return TFX_TOKEN_SRC_ALPHA_BLEND_FACTOR;
}

"dst_alpha_blend_factor" {
  return TFX_TOKEN_DST_ALPHA_BLEND_FACTOR;
}

"alpha_blend_op" {
  return TFX_TOKEN_ALPHA_BLEND_OP;
}

"one" {
  return TFX_TOKEN_ONE;
}

"src_color" {
  return TFX_TOKEN_SRC_COLOR;
}

"one_minus_src_color" {
  return TFX_TOKEN_ONE_MINUS_SRC_COLOR;
}

"dst_color" {
  return TFX_TOKEN_DST_COLOR;
}

"one_minus_dst_color" {
  return TFX_TOKEN_ONE_MINUS_DST_COLOR;
}

"src_alpha" {
  return TFX_TOKEN_SRC_ALPHA;
}

"one_minus_src_alpha" {
  return TFX_TOKEN_ONE_MINUS_SRC_ALPHA;
}

"dst_alpha" {
  return TFX_TOKEN_DST_ALPHA;
}

"one_minus_dst_alpha" {
  return TFX_TOKEN_ONE_MINUS_DST_ALPHA;
}

"constant_color" {
  return TFX_TOKEN_CONSTANT_COLOR;
}

"one_minus_constant_color" {
  return TFX_TOKEN_ONE_MINUS_CONSTANT_COLOR;
}

"constant_alpha" {
  return TFX_TOKEN_CONSTANT_ALPHA;
}

"one_minus_constant_alpha" {
  return TFX_TOKEN_ONE_MINUS_CONSTANT_ALPHA;
}

"src_alpha_saturate" {
  return TFX_TOKEN_SRC_ALPHA_SATURATE;
}

"src_one_color" {
  return TFX_TOKEN_SRC_ONE_COLOR;
}

"one_minus_src_one_color" {
  return TFX_TOKEN_ONE_MINUS_SRC_ONE_COLOR;
}

"src_one_alpha" {
  return TFX_TOKEN_SRC_ONE_ALPHA;
}

"one_minus_src_one_alpha" {
  return TFX_TOKEN_ONE_MINUS_SRC_ONE_ALPHA;
}

"add" {
  return TFX_TOKEN_ADD;
}

"subtract" {
  return TFX_TOKEN_SUBTRACT;
}

"reverse_subtract" {
  return TFX_TOKEN_REVERSE_SUBTRACT;
}

"min" {
  return TFX_TOKEN_MIN;
}

"max" {
  return TFX_TOKEN_MAX;
}

"clear" {
  return TFX_TOKEN_CLEAR;
}

"and" {
  return TFX_TOKEN_AND;
}

"and_reverse" {
  return TFX_TOKEN_AND_REVERSE;
}

"copy" {
  return TFX_TOKEN_COPY;
}

"and_inverted" {
  return TFX_TOKEN_AND_INVERTED;
}

"no_op" {
  return TFX_TOKEN_NO_OP;
}

"xor" {
  return TFX_TOKEN_XOR;
}

"or" {
  return TFX_TOKEN_OR;
}

"nor" {
  return TFX_TOKEN_NOR;
}

"equivalent" {
  return TFX_TOKEN_EQUIVALENT;
}

"or_reverse" {
  return TFX_TOKEN_OR_REVERSE;
}

"copy_inverted" {
  return TFX_TOKEN_COPY_INVERTED;
}

"or_inverted" {
  return TFX_TOKEN_OR_INVERTED;
}

"nand" {
  return TFX_TOKEN_NAND;
}

"set" {
  return TFX_TOKEN_SET;
}


vs_6_0 {
  return TFX_TOKEN_TARGET_VS_6_0;
}

vs_6_1 {
  return TFX_TOKEN_TARGET_VS_6_1;
}

vs_6_2 {
  return TFX_TOKEN_TARGET_VS_6_2;
}

vs_6_3 {
  return TFX_TOKEN_TARGET_VS_6_3;
}

vs_6_4 {
  return TFX_TOKEN_TARGET_VS_6_4;
}

vs_6_5 {
  return TFX_TOKEN_TARGET_VS_6_5;
}

vs_6_6 {
  return TFX_TOKEN_TARGET_VS_6_6;
}

vs_6_7 {
  return TFX_TOKEN_TARGET_VS_6_7;
}

ps_6_0 {
  return TFX_TOKEN_TARGET_PS_6_0;
}

ps_6_1 {
  return TFX_TOKEN_TARGET_PS_6_1;
}

ps_6_2 {
  return TFX_TOKEN_TARGET_PS_6_2;
}

ps_6_3 {
  return TFX_TOKEN_TARGET_PS_6_3;
}

ps_6_4 {
  return TFX_TOKEN_TARGET_PS_6_4;
}

ps_6_5 {
  return TFX_TOKEN_TARGET_PS_6_5;
}

ps_6_6 {
  return TFX_TOKEN_TARGET_PS_6_6;
}

ps_6_7 {
  return TFX_TOKEN_TARGET_PS_6_7;
}

[a-zA-Z\_][0-9a-zA-Z\-_.]* {
  shlval.sval = strdup(yytext);
  return TFX_TOKEN_NAME_VAL;
}

. { 
  const string error =  fmt::format("lexical error in `{}`", yytext);
  sherror(compiler, error.c_str());}

%%
