#include "fs.h"

#include <engine/log.h>

#include <fstream>
#include <filesystem>

namespace Utils
{
  eastl::vector<char> ReadFile(const String& file)
  {
    std::error_code ec;
    const auto fileSize = std::filesystem::file_size(file, ec);
    if (!ec)
    {
      std::ifstream f;
      f.open(file);
      if (f.is_open())
      {
        eastl::vector<char> readenFile;
        readenFile.resize(fileSize);

        f.read(readenFile.data(), fileSize);
        return readenFile;
      }
      else
        logerror("failed to read the file {}", file);
    }
    else
      logerror("failed to read the file {}: {}", file, ec.message());

    return {};
  }

  String GetFilename(const String& filePath)
  {
    const size_t i = filePath.find_last_of("/\\");
    if (i != String::npos)
      return filePath.substr(i+1);
    else
      return "";
  }

  String GetFileExt(const String& fileName)
  {
    if (!fileName.empty())
    {
      const size_t i = fileName.find_last_of(".");
      if (i != String::npos)
        return fileName.substr(i+1);
    }

    return "";
  }

  bool CheckFileExt(const String& fileName, const String requiredExt)
  {
    return GetFileExt(fileName) == requiredExt;
  }
}