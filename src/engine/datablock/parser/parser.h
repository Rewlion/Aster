#pragma once

#include <engine/types.h>
#include <engine/datablock/datablock.h>

#include <EASTL/vector.h>

class BlkParser
{
public:
  DataBlock ParseFile(const string& pathToDbk, const bool accept_fails = true);

  void setBlock(DataBlock&& bk) { m_ParsedBk = std::move(bk); }
  void pushFile(const string& file) { m_FileCtxs.push_back({file }); }
  void popFile() { m_FileCtxs.pop_back(); }
  auto getCurrentFileName() const -> string_view { return m_FileCtxs.back().fileName; }

  void markParsingFailed() { m_ParsingFailed = true; }
  auto isParsingOk() const -> bool { return !m_ParsingFailed; }

  void setBkName(DataBlock& bk, string&& name) { bk.m_Name = std::move(name); }
  void setBkAnnotation(DataBlock& bk, string&& annotation) { bk.m_Annotation = annotation; }

private:
  struct FileContext
  {
    string fileName;
  };

private:
  bool m_ParsingFailed = false;
  DataBlock m_ParsedBk;
  eastl::vector<FileContext> m_FileCtxs;
};
