#include "parser.h"

#include <engine/assert.h>
#include <engine/vfs/vfs.h>

#include <cstdio>
#include <cstdlib>

typedef size_t yy_size_t;
typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern YY_BUFFER_STATE bk_scan_buffer (char *base, yy_size_t size);
extern void bk_delete_buffer(YY_BUFFER_STATE buffer);
extern void bk_switch_to_buffer(YY_BUFFER_STATE  new_buffer );


extern int bkparse(BlkParser& parser);
extern void bkrestart(FILE* f);
extern FILE *bkin;

DataBlock BlkParser::ParseFile(const string& path, const bool accept_fails)
{
  eastl::vector<char> fileContent = vfs::read_file(path);

  if (fileContent.empty()) [[unlikely]]
  {
    m_ParsingFailed = true;
    logerror("can't open a datablock file `{}`", path);
    return EMPTY_BLOCK;
  }

  //bison expects last two bytes to be \0
  fileContent.push_back('\0');
  fileContent.push_back('\0');

  bkin = NULL;
  pushFile(path, std::move(fileContent));
  auto& buffer = getCurrentBuffer();
  bk_scan_buffer(buffer.data(), buffer.size());
  bkparse(*this);

  m_FileCtxs.clear();

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

