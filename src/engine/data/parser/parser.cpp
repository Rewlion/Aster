#include "parser.h"

#include <engine/assert.h>
#include <engine/fs/vfs.h>

#include <cstdio>
#include <cstdlib>

typedef size_t yy_size_t;
typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern YY_BUFFER_STATE ed_scan_buffer (char *base, yy_size_t size);
extern void ed_delete_buffer(YY_BUFFER_STATE buffer);
extern void ed_switch_to_buffer(YY_BUFFER_STATE  new_buffer );


extern int edparse(ed::Parser& parser);
extern void edrestart(FILE* f);
extern FILE *edin;

namespace ed
{
  Scope Parser::parseFile(string_view path, const bool accept_fails)
  {
    eastl::vector<char> fileContent = vfs::read_file(path);

    if (fileContent.empty()) [[unlikely]]
    {
      m_ParsingFailed = true;
      logerror("can't open an engine data file `{}`", path);
      return {};
    }

    //bison expects last two bytes to be \0
    fileContent.push_back('\0');
    fileContent.push_back('\0');

    edin = NULL;
    pushFile(path, std::move(fileContent));
    auto& buffer = getCurrentBuffer();
    ed_scan_buffer(buffer.data(), buffer.size());
    edparse(*this);

    m_FileCtxs.clear();

    if (!isParsingOk()) [[unlikely]]
    {
      ASSERT_FMT(accept_fails, "failed to parse engine data file `{}`.\nErrors:{}\n", path, m_Errors);
      return {};
    } 

    return std::move(m_MainScope);
  }

  void Parser::markParsingFailed(const string& error)
  {
    m_ParsingFailed = true;
    m_Errors = fmt::format("{}\n{}", m_Errors, error);
  }

}
