#pragma once

#include "ast.h"

#include <engine/types.h>
#include <engine/datablock/datablock.h>

class DbkParser
{
public:
  DataBlock parse_file(const String& pathToDbk);
private:
  DataBlock traverse_ast(const Ast::Config& ast);
  DataBlock construct_block(const Ast::AnnotatedParam* paramList);
  DataBlock::Attribute construct_attribute(const Ast::Attribute* attr);
};
