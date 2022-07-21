%{
    #define YY_DECL int shlex(ShadersSystem::Compiler& compiler)

    #include <stdio.h>
    #include "parser.tab.hpp"

    void sherror(ShadersSystem::Compiler& compiler, const char* msg);
    extern int shlex(ShadersSystem::Compiler& compiler);

    string codeBuf;
%}

%option yylineno
%option nounistd
%option never-interactive
%option noyywrap
%option prefix="sh"

%x hlsl

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
  return TOKEN_HLSL_CODE;
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
  return TOKEN_AT;
}

: {
  return TOKEN_COLON;
}

; {
  return TOKEN_SEMICOLON;
}

= {
  return TOKEN_EQUAL_OP;
}

\( {
  return TOKEN_LEFT_PARENTHESIS;
}

\) {
  return TOKEN_RIGHT_PARENTHESIS;
}

\{ {
  return TOKEN_LEFT_BRACKET;
}

\} {
  return TOKEN_RIGHT_BRACKET;
}

, {
  return TOKEN_COMMA;
}

- {
  return TOKEN_MINUS;
}

scope {
  return TOKEN_SCOPE;
}

shader {
  return TOKEN_SHADER;
}

vertex {
  return TOKEN_SHADER_VERTEX;
}

pixel {
  return TOKEN_SHADER_PIXEL;
}

register {
  return TOKEN_REGISTER;
}

texture {
  return TOKEN_TEXTURE;
}

input {
  return TOKEN_INPUT;
}

buffer {
  return TOKEN_BUFFER;
}

cbuffer {
  return TOKEN_CBUFFER;
}

extern {
  return TOKEN_EXTERN;
}

channel {
  return TOKEN_CHANNEL;
}

descriptor_set {
  return TOKEN_DESCRIPTOR_SET;
}

false {
  shlval.bval = false;
  return TOKEN_BOOL_VAL;
}

true {
  shlval.bval = true;
  return TOKEN_BOOL_VAL;
}

no {
  shlval.bval = false;
  return TOKEN_BOOL_VAL;
}

yes {
  shlval.bval = true;
  return TOKEN_BOOL_VAL;
}

[0-9]+ {
  shlval.ival = atoi(yytext);
  return TOKEN_INT_VAL;
}

[0-9]+[.][0-9]* { 
  shlval.fval = atof(yytext);
  return TOKEN_FLOAT_VAL;
}

float {
  return TOKEN_FLOAT;
}

float2 {
  return TOKEN_FLOAT2;
}

float3 {
  return TOKEN_FLOAT3;
}

float4 {
  return TOKEN_FLOAT4;
}

int {
  return TOKEN_INT;
}

int2 {
  return TOKEN_INT2;
}

int3 {
  return TOKEN_INT3;
}

int4 {
  return TOKEN_INT4;
}

texture2D {
  return TOKEN_TEXTURE2D;
}

technique {
  return TOKEN_TECHNIQUE;
}

activate {
  return TOKEN_ACTIVATE;
}

support {
  return TOKEN_SUPPORT;
}

compile {
  return TOKEN_COMPILE;
}

render_state {
  return TOKEN_RENDER_STATE;
}

primitive_topology {
  return TOKEN_PRIMITIVE_TOPOLOGY;
}

point_list {
  return TOKEN_PT_POINT_LIST;
}

line_list {
  return TOKEN_PT_LINE_LIST;
}

line_strip {
  return TOKEN_PT_LINE_STRIP;
}

triangle_list {
  return TOKEN_PT_TRIANGLE_LIST;
}

triangle_fan {
  return TOKEN_PT_TRIANGLE_FAN;
}

line_list_with_adjacency {
  return TOKEN_PT_LINE_LIST_WITH_ADJACENCY;
}

line_strip_with_adjacency {
  return TOKEN_PT_LINE_STRIP_WITH_ADJACENCY;
}

triangle_list_with_adjacency {
  return TOKEN_PT_TRIANGLE_LIST_WITH_ADJACENCY;
}

triangle_strip_with_adjacency {
  return TOKEN_PT_TRIANGLE_STRIP_WITH_ADJACENCY;
}

patch_list {
  return TOKEN_PT_PATCH_LIST;
}

"depth_test" {
  return TOKEN_DEPTH_TEST;
}

"depth_write" {
  return TOKEN_DEPTH_WRITE;
}

"depth_op" {
  return TOKEN_DEPTH_OP;
}

"stencil_test" {
  return TOKEN_STENCIL_TEST;
}

"stencil_fail_op" {
  return TOKEN_STENCIL_FAIL_OP;
}

"stencil_pass_op" {
  return TOKEN_STENCIL_PASS_OP;
}

"stencil_depth_fail_op" {
  return TOKEN_STENCIL_DEPTH_FAIL_OP;
}

"stencil_compare_op" {
  return TOKEN_STENCIL_COMPARE_OP;
}

"stencil_ref_val" {
  return TOKEN_STENCIL_REFERENCE_VALUE;
}

"never" {
  return TOKEN_NEVER;
}

"less" {
  return TOKEN_LESS;
}

"equal" {
  return TOKEN_EQUAL;
}

"less_or_equal" {
  return TOKEN_LESS_OR_EQUAL;
}

"greater" {
  return TOKEN_GREATER;
}

"not_equal" {
  return TOKEN_NOT_EQUAL;
}

"greater_or_equal" {
  return TOKEN_GREATER_OR_EQUAL;
}

"always" {
  return TOKEN_ALWAYS;
}

"keep" {
  return TOKEN_KEEP;
}

"zero" {
  return TOKEN_ZERO;
}

"replace" {
  return TOKEN_REPLACE;
}

"inc_clamp" {
  return TOKEN_INCREMENT_AND_CLAMP;
}

"dec_clamp" {
  return TOKEN_DECREMENT_AND_CLAMP;
}

"invert" {
  return TOKEN_INVERT;
}

"inc_and_wrap" {
  return TOKEN_INCREMENT_AND_WRAP;
}

"dec_and_wrap" {
  return TOKEN_DECREMENT_AND_WRAP;
}

"one" {
  return TOKEN_ONE;
}

"src_color" {
  return TOKEN_SRC_COLOR;
}

"one_minus_src_color" {
  return TOKEN_ONE_MINUS_SRC_COLOR;
}

"dst_color" {
  return TOKEN_DST_COLOR;
}

"one_minus_dst_color" {
  return TOKEN_ONE_MINUS_DST_COLOR;
}

"src_alpha" {
  return TOKEN_SRC_ALPHA;
}

"one_minus_src_alpha" {
  return TOKEN_ONE_MINUS_SRC_ALPHA;
}

"dst_alpha" {
  return TOKEN_DST_ALPHA;
}

"one_minus_dst_alpha" {
  return TOKEN_ONE_MINUS_DST_ALPHA;
}

"constant_color" {
  return TOKEN_CONSTANT_COLOR;
}

"one_minus_constant_color" {
  return TOKEN_ONE_MINUS_CONSTANT_COLOR;
}

"constant_alpha" {
  return TOKEN_CONSTANT_ALPHA;
}

"one_minus_constant_alpha" {
  return TOKEN_ONE_MINUS_CONSTANT_ALPHA;
}

"src_alpha_saturate" {
  return TOKEN_SRC_ALPHA_SATURATE;
}

"src_one_color" {
  return TOKEN_SRC_ONE_COLOR;
}

"one_minus_src_one_color" {
  return TOKEN_ONE_MINUS_SRC_ONE_COLOR;
}

"src_one_alpha" {
  return TOKEN_SRC_ONE_ALPHA;
}

"one_minus_src_one_alpha" {
  return TOKEN_ONE_MINUS_SRC_ONE_ALPHA;
}


vs_6_0 {
  return TOKEN_TARGET_VS_6_0;
}

vs_6_1 {
  return TOKEN_TARGET_VS_6_1;
}

vs_6_2 {
  return TOKEN_TARGET_VS_6_2;
}

vs_6_3 {
  return TOKEN_TARGET_VS_6_3;
}

vs_6_4 {
  return TOKEN_TARGET_VS_6_4;
}

vs_6_5 {
  return TOKEN_TARGET_VS_6_5;
}

vs_6_6 {
  return TOKEN_TARGET_VS_6_6;
}

vs_6_7 {
  return TOKEN_TARGET_VS_6_7;
}

ps_6_0 {
  return TOKEN_TARGET_PS_6_0;
}

ps_6_1 {
  return TOKEN_TARGET_PS_6_1;
}

ps_6_2 {
  return TOKEN_TARGET_PS_6_2;
}

ps_6_3 {
  return TOKEN_TARGET_PS_6_3;
}

ps_6_4 {
  return TOKEN_TARGET_PS_6_4;
}

ps_6_5 {
  return TOKEN_TARGET_PS_6_5;
}

ps_6_6 {
  return TOKEN_TARGET_PS_6_6;
}

ps_6_7 {
  return TOKEN_TARGET_PS_6_7;
}

[a-zA-Z\_][0-9a-zA-Z\-_.]* {
  shlval.sval = strdup(yytext);
  return TOKEN_NAME_VAL;
}

. { printf("Syntax error in line %d, `%s`\n", yylineno, yytext); exit(1); }

%%
