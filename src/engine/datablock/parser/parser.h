#pragma once

#include <engine/types.h>
#include <engine/datablock/datablock.h>

#include <EASTL/vector.h>

class BlkParser
{
public:
  DataBlock ParseFile(const string& pathToDbk, const bool accept_fails = false);

  void setBlock(DataBlock&& bk) { m_ParsedBk = std::move(bk); }
  void pushFile(const string& file, eastl::vector<char>&& buffer)
  {
    m_FileCtxs.push_back({file, 1, std::move(buffer) });
  }
  auto getCurrentBuffer() -> eastl::vector<char>&
  {
    return m_FileCtxs.back().buffer;
  }
  void popFile()
  {
    m_FileCtxs.pop_back();
  }
  void incLine() { ++m_FileCtxs.back().line; }
  auto getLine() const -> unsigned int { return m_FileCtxs.back().line; }
  auto getCurrentFileName() const -> string_view { return m_FileCtxs.back().fileName; }

  void markParsingFailed(const string& error);
  auto isParsingOk() const -> bool { return !m_ParsingFailed; }

  void setBkName(DataBlock& bk, string&& name) { bk.m_Name = std::move(name); }
  void setBkAnnotation(DataBlock& bk, string&& annotation) { bk.m_Annotation = annotation; }

private:
  struct FileContext
  {
    string fileName;
    unsigned int line;
    eastl::vector<char> buffer;
  };

private:
  bool m_ParsingFailed = false;
  DataBlock m_ParsedBk;
  eastl::vector<FileContext> m_FileCtxs;
  string m_Errors;
};
