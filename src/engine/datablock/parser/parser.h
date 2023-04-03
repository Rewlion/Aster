#pragma once

#include "ast.h"

#include <engine/types.h>
#include <engine/datablock/datablock.h>

#include <EASTL/vector.h>

class BlkParser
{
public:
  DataBlock ParseFile(const string& pathToDbk);

  void setRoot(Ast::AnnotatedParam* p) { m_Root = p; }
  void pushFile(const string& file) { m_FileCtxs.push_back({file }); }
  void popFile() { m_FileCtxs.pop_back(); }
  auto getCurrentFileName() const -> string_view { return m_FileCtxs.back().fileName; }

  void markParsingFailed() { m_ParsingFailed = true; }
  auto isParsingOk() const -> bool { return !m_ParsingFailed; }

  void incLineno() { ++m_FileCtxs.back().lineno; m_FileCtxs.back().columnno = 1; }
  void incColumnno() { ++m_FileCtxs.back().columnno; }
  auto getLineno() const -> int { return m_FileCtxs.back().lineno; }
  auto getColumnno() const -> int { return m_FileCtxs.back().columnno; }

private:
  struct FileContext
  {
    string fileName;
    int lineno = 1;
    int columnno = 1;
  };

  DataBlock TraverseAst();
  DataBlock ConstructBlock(const Ast::AnnotatedParam* paramList);
  DataBlock::Attribute ConstructAttribute(const Ast::Attribute* attr);

private:
  bool m_ParsingFailed = false;
  Ast::AnnotatedParam* m_Root = nullptr;
  eastl::vector<FileContext> m_FileCtxs;
};
