pushd ..\..\..\..\
  set parser_dir=src/engine/data/parser
  deps\bison\win_flex -o %parser_dir%/lex.yy.cpp %parser_dir%/parser.l
  deps\bison\win_bison %1 -d -o %parser_dir%/parser.tab.cpp %parser_dir%/parser.y
popd

