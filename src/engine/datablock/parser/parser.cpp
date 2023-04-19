#include "parser.h"

#include <engine/assert.h>

#include <cstdio>
#include <cstdlib>

extern int bkparse(BlkParser& parser);
extern void bkrestart(FILE* f);
extern FILE *bkin;

DataBlock BlkParser::ParseFile(const string& path, const bool accept_fails)
{
  FILE* f = fopen(path.c_str(), "r");

  if (!f) [[unlikely]]
  {
    m_ParsingFailed = true;
    logerror("can't open a datablock file `{}`", path);
    return EMPTY_BLOCK;
  }

  bkin = f;

  bkrestart(f);
  pushFile(path);
  bkparse(*this);
  fclose(f);

  if (!isParsingOk()) [[unlikely]]
  {
    ASSERT_FMT(accept_fails, "failed to parse blk file `{}`.\nErrors:{}\n", path, m_Errors);
    return EMPTY_BLOCK;
  }

  return std::move(m_ParsedBk);
}

void BlkParser::markParsingFailed(const string& error)
{
  m_ParsingFailed = true;
  m_Errors = fmt::format("{}\n{}", m_Errors, error);
}

