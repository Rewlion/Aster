#pragma once

#include <vector>
#include <string>

class ReflectionHeaderTool
{
public:
  struct Options
  {
    std::vector<std::string> filesToParse;
    std::vector<std::string> includePaths;
    std::string outputFolder;
    std::vector<const char*> clangToolArgv;
  };

public:
  int Parse(const Options& opts);

private:
  Options m_Options;
};
