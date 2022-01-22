#pragma once

#include <EASTL/vector.h>
#include <engine/types.h>

namespace utils
{
  eastl::vector<char> readFile(const String& file);
  String getFilename(const String& filePath);
  String getFileExt(const String& fileName);
  bool checkFileExt(const String& fileName, const String requiredExt);
}
