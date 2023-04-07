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
    logerror("can't open a datablock file {}", path);
    return EMPTY_BLOCK;
  }

  bkin = f;

  bkrestart(f);
  pushFile(path);
  bkparse(*this);
  fclose(f);

  if (!isParsingOk()) [[unlikely]]
  {
    if (!accept_fails)
      ASSERT(!"failed to parse blk file");
    return EMPTY_BLOCK;
  }

  return std::move(m_ParsedBk);
}
