%{
    #define YY_DECL int bklex(BlkParser& parser)

    #include "parser.h"
    #include <engine/log.h>
    #include <engine/vfs/vfs.h>

    #include <stdio.h>
    #include "parser.tab.hpp"

    void bkerror(BlkParser& parser, const char* msg);
    extern int bklex(BlkParser& parser);
%}

%option nounistd
%option never-interactive
%option noyywrap
%option prefix="bk"

%x include

%%

<include>[ \t]* {

}

<include>["][^ \t\r\n]+["] {
  char* fileName = yytext + 1;
  fileName[strlen(yytext) - 2] = '\0';

  eastl::vector<char> stagingBuffer = vfs::read_file(fileName);
  if (stagingBuffer.empty())
  {
    const string error = fmt::format("can't include file `{}`: File not found", fileName);
    bkerror(parser, error.c_str());
    yyterminate();
  }

  stagingBuffer.push_back('\0');
  stagingBuffer.push_back('\0');

  parser.pushFile(string{fileName}, std::move(stagingBuffer));
  auto& buffer = parser.getCurrentBuffer();

	yypush_buffer_state(YY_CURRENT_BUFFER);
  bk_scan_buffer(buffer.data(), buffer.size());

  BEGIN(INITIAL);
}

<include>. {
  const std::string error = fmt::format("unexpected string: {}", yytext);
  bkerror(parser, error.c_str());
  yyterminate();
}

#include[ ] {
  BEGIN(include);
}

<<EOF>> {
  yypop_buffer_state();
  parser.popFile();
  if ( !YY_CURRENT_BUFFER )
  {
    yyterminate();
  }
}

[/][/][^\n]* ; // comment

[ \t\r]* {
}

\n {
  parser.incLine();
}

@ {
  return AT;
}

: {
  return COLON;
}

= {
  return EQUAL_OP;
}

\( {
  return LEFT_PARENTHESIS;
}

\) {
  return RIGHT_PARENTHESIS;
}

\{ {
  return LEFT_BRACKET;
}

\} {
  return RIGHT_BRACKET;
}

\[ {
  return LEFT_SQUARE_BRACKET;
}

\] {
  return RIGHT_SQUARE_BRACKET;
}

, {
  return COMMA;
}

int {
  return INT_TYPE;
}

float {
  return FLOAT_TYPE;
}

text {
  return TEXT_TYPE;
}

float2 {
  return POINT_2D_TYPE;
}

float3 {
  return POINT_3D_TYPE;
}

float4 {
  return POINT_4D_TYPE;
}

int2 {
  return INT_POINT_2D_TYPE;
}

int3 {
  return INT_POINT_3D_TYPE;
}

int4 {
  return INT_POINT_4D_TYPE;
}

mat3 {
  return MAT3_TYPE;
}

mat4 {
  return MAT4_TYPE;
}

bool {
  return BOOL_TYPE;
}

yes {
  bklval = true; return BOOL_VAL;
}

true {
  bklval = true; return BOOL_VAL;
}

on {
  bklval = true; return BOOL_VAL;
}

no {
  bklval = false; return BOOL_VAL;
}

false {
  bklval = false; return BOOL_VAL;
}

off {
  bklval = false; return BOOL_VAL;
}

[-+]?[0-9]+ {
  bklval = Number{atoi(yytext)}; return INT_VAL; 
}

[-+]?[0-9]+[.][0-9]* { 
  bklval = Number{(float)atof(yytext)}; return FLOAT_VAL;
}

[a-zA-Z\-_][0-9a-zA-Z\-_.]* {
  bklval = string{yytext}; return NAME_VAL;
}

["][^\t\r\n\"]*["] {
  char* text = yytext + 1;
  text[strlen(yytext) - 2] = '\0';
  bklval = string{text}; return TEXT_VAL;
}

. { 
  const string error =  fmt::format("unexpected symbol `{}`", yytext);
  bkerror(parser, error.c_str());
  yyterminate();
}

%%
