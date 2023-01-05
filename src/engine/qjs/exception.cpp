#include "exception.h"
#include "value.hpp"
#include "inc.h"

#include <engine/log.h>

namespace qjs
{
  void logerror_exception(Value&& e)
  {
    ValueDump dump = e.dump();
    logerror("ui: {}", dump.asString());

    if (e.isError())
    {
      auto eView = e.as<ObjectView>();
      Value stack = eView.getProperty("stack");
      if (!stack.isUndefined())
      {
        ValueDump stackDump = stack.dump();
        logerror("ui: {}", stackDump.asString());
      }
    }
  }
}
