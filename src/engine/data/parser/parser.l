%{
    #define YY_DECL int edlex(ed::Parser& parser)

    #include "parser.h"
    #include <engine/log.h>
    #include <engine/fs/vfs.h>

    #include <stdio.h>
    #include "parser.tab.hpp"

    void ederror(ed::Parser& parser, const char* msg);
    extern int edlex(ed::Parser& parser);
%}

%option nounistd
%option never-interactive
%option noyywrap
%option prefix="ed"

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
    ederror(parser, error.c_str());
    yyterminate();
  }

  stagingBuffer.push_back('\0');
  stagingBuffer.push_back('\0');

  parser.pushFile(string{fileName}, std::move(stagingBuffer));
  auto& buffer = parser.getCurrentBuffer();

	yypush_buffer_state(YY_CURRENT_BUFFER);
  ed_scan_buffer(buffer.data(), buffer.size());

  BEGIN(INITIAL);
}

<include>. {
  const std::string error = fmt::format("unexpected string: {}", yytext);
  ederror(parser, error.c_str());
  yyterminate();
}

#include[ ] { BEGIN(include); }

<<EOF>> {
  yypop_buffer_state();
  parser.popFile();
  if ( !YY_CURRENT_BUFFER )
  {
    yyterminate();
  }
}

[/][/][^\n]* ; // comment

[ \t\r]* {}

\n { parser.incLine(); }

@ { return AT; }

: { return COLON; }

= { return EQUAL_OP; }

\( { return LEFT_PARENTHESIS; }

\) { return RIGHT_PARENTHESIS; }

\{ { return LEFT_BRACKET; }

\} { return RIGHT_BRACKET; }

\[ { return LEFT_SQUARE_BRACKET; }

\] { return RIGHT_SQUARE_BRACKET; }

, { return COMMA; }

yes { edlval = true; return BOOL_VAL; }

true { edlval = true; return BOOL_VAL; }

on { edlval = true; return BOOL_VAL; }

no { edlval = false; return BOOL_VAL; }

false { edlval = false; return BOOL_VAL; }

off { edlval = false; return BOOL_VAL; }

[-+]?[0-9]+ { edlval = Number{atoi(yytext)}; return INT_VAL; }

[-+]?[0-9]+[.][0-9]* { edlval = Number{(float)atof(yytext)}; return FLOAT_VAL;
}

[a-zA-Z\-_][0-9a-zA-Z\-_.]* { edlval = string{yytext}; return NAME_VAL; }

["][^\t\r\n\"]*["] {
  char* text = yytext + 1;
  text[strlen(yytext) - 2] = '\0';
  edlval = string{text}; return TEXT_VAL;
}

. { 
  const string error =  fmt::format("unexpected symbol `{}`", yytext);
  ederror(parser, error.c_str());
  yyterminate();
}

%%
