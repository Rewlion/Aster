#pragma once

#include <EASTL/vector.h>
#include <engine/types.h>

namespace Utils
{
  eastl::vector<char> ReadFile(const string& file);
  string GetFileName(const string& filePath);
  string GetFileExt(const string& fileName);
  bool CheckFileExt(const string& fileName, const string requiredExt);
}
