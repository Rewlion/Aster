#pragma once

#include "ast.h"

#include <engine/types.h>
#include <engine/datablock/datablock.h>

class BlkParser
{
public:
  DataBlock ParseFile(const String& pathToDbk);
private:
  DataBlock TraverseAst(const Ast::Config& ast);
  DataBlock ConstructBlock(const Ast::AnnotatedParam* paramList);
  DataBlock::Attribute ConstructAttribute(const Ast::Attribute* attr);
};
