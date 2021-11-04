win_flex -o lex.yy.cpp parser.l
win_bison %1 -d -o parser.tab.cpp parser.y