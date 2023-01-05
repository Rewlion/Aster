#include "fs.h"

#include <engine/log.h>

#include <filesystem>
#include <fstream>
#include <sstream>

namespace Utils
{
  eastl::vector<char> read_file(const string& file)
  {
    std::error_code ec;
    const auto fileSize = std::filesystem::file_size(file, ec);
    
    if (ec)
    {
      logerror("failed to open the file {}: {}", file, ec.message());
      return {};
    }

    std::ifstream f;

    f.open(file, std::ios::binary);
    if (!f.is_open())
    {
      logerror("failed to open the file {}", file);
      return {};
    }

    eastl::vector<char> readenFile;
    readenFile.resize(fileSize);
    f.read(readenFile.data(), fileSize);
    
    return readenFile;
  }

  string read_file_as_string(const string_view& file)
  {
    std::ifstream f;

    f.open(file);
    if (!f.is_open())
    {
      logerror("failed to open the file {}", file);
      return {};
    }

    std::stringstream ss;
    ss << f.rdbuf();
    string str = std::move(ss).str();
    
    return str;
  }

  string get_file_name(const string& filePath)
  {
    const size_t i = filePath.find_last_of("/\\");
    if (i != string::npos)
    {
      const size_t j = filePath.find_last_of(".");
      if (j != string::npos)
        return filePath.substr(i+1, j-i-1);
      else
        return filePath.substr(i+1);
    }
    else
      return "";
  }

  string get_file_ext(const string& fileName)
  {
    if (!fileName.empty())
    {
      const size_t i = fileName.find_last_of(".");
      if (i != string::npos)
        return fileName.substr(i+1);
    }

    return "";
  }

  bool check_file_ext(const string& fileName, const string requiredExt)
  {
    return get_file_ext(fileName) == requiredExt;
  }

  string get_dir_name(const string& filePath)
  {
    const size_t i = filePath.find_last_of("/\\");
    return i != string::npos ?
            filePath.substr(0, i) :
            "";
  }
}