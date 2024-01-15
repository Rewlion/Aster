#include "utils.h"

#include <engine/data/parser/parser.h>

namespace ed
{
  auto load_from_file(const string_view path, const std::shared_ptr<CustomTypeRegistry> reg) -> std::optional<Scope>
  {
    Parser parser{reg};
    ed::Scope data = parser.parseFile(path);

    if (parser.isParsingOk())
      return data;

    return std::nullopt;
  }
}
