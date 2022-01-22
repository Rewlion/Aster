#pragma once

#include <EASTL/vector.h>
#include <engine/types.h>

namespace utils
{
  eastl::vector<char> ReadFile(const String& file);
  String GetFilename(const String& filePath);
  String GetFileExt(const String& fileName);
  bool CheckFileExt(const String& fileName, const String requiredExt);
}
