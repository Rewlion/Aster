#include "assert.h"
#include "engine/render/debug_text.h"

#include <engine/log.h>

#define BOOST_STACKTRACE_USE_WINDBG 1
#include <boost/stacktrace.hpp>
#include <winuser.h>

void show_assert_window(string error, const bool isIgnorable)
{
  Engine::dbg::vis_logerr(fmt::format("{}", error));

  const auto st = boost::stacktrace::stacktrace();
  error += "\n";
  error += boost::stacktrace::detail::to_string(&st.as_vector()[1], st.size()-1);

  logerror_no_vis(error);

  int flags = MB_ICONERROR | MB_DEFBUTTON2;
  flags |= isIgnorable ? MB_ABORTRETRYIGNORE : MB_OK;

  const int msgboxID = MessageBox(NULL, error.c_str(), "Error", flags);

  switch (msgboxID)
  {
    case IDIGNORE:
      if (isIgnorable)
        break;
    case IDOK:
    case IDRETRY:
    case IDABORT:
        exit(-1);
        break;
  }
}