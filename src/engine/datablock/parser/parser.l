%{
    #include <stdio.h>
    #include "parser.tab.hpp"

    void yyerror(Ast::Config* rootAst, const char* msg);
    extern int yylex();
    int columno = 1;
    std::string lineText;
%}

%option yylineno
%option nounistd
%option never-interactive
%option noyywrap

%x include

%%

<include>[ \t]* {

}

<include>["][^ \t\r\n]+["] {
  char* fileName = yytext + 1;
  fileName[strlen(yytext) - 2] = '\0';

  yyin = fopen( fileName, "r" );
  if (!yyin)
    printf("(%d) can't include file `%s`", yylineno, fileName);

	yypush_buffer_state(yy_create_buffer( yyin, YY_BUF_SIZE ));
  BEGIN(INITIAL);
}

<include>. { printf("(%d) unexpected string: `%s`\n", yylineno, yytext); exit(1); }

#include[ ] {
  BEGIN(include);
}

<<EOF>> {
  yypop_buffer_state();
  if ( !YY_CURRENT_BUFFER )
  {
    yyterminate();
  }
}

[/][/][^\n]* ; // comment

[ \t\n]* {
  columno = 1;
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
  yylval.bval = true; return BOOL_VAL;
}

true {
  yylval.bval = true; return BOOL_VAL;
}

on {
  yylval.bval = true; return BOOL_VAL;
}

no {
  yylval.bval = false; return BOOL_VAL;
}

false {
  yylval.bval = false; return BOOL_VAL;
}

off {
  yylval.bval = false; return BOOL_VAL;
}

[-+]?[0-9]+ {
  yylval.ival = atoi(yytext); return INT_VAL; 
}

[-+]?[0-9]+[.][0-9]* { 
  yylval.fval = atof(yytext); return FLOAT_VAL;
}

[a-zA-Z\-_][0-9a-zA-Z\-_.]* {
  yylval.sval = strdup(yytext); return NAME_VAL;
}

["][^ \t\r\n]+["] {
  char* text = yytext + 1;
  text[strlen(yytext) - 2] = '\0';
  yylval.sval = strdup(text); return TEXT_VAL;
}

. { printf("Syntax error in line %d, `%s`\n", yylineno, yytext); exit(1); }

%%
