#include "reflection_header_tool.h"

#include <boost/program_options.hpp>
#include <iostream>
#include <filesystem>
#include <tuple>
#include <map>
#include <set>

namespace po = boost::program_options;


namespace
{
  typedef std::vector<std::string> IncludePaths;
  typedef std::vector<std::string> InputFiles;
  typedef std::string OutputFolder;
  typedef std::string FileName;
  typedef std::string FilePath;
  typedef int ErrorCode;

  constexpr ErrorCode OK = 0;
  constexpr ErrorCode FailedToParse = -1;

  std::set<FilePath> FindFilesWithNotUniqueNames(std::vector<std::string>& files)
  {

    std::map<FileName, FilePath> checkedFileNames;
    std::set<FilePath> conflictFiles;

    for (const auto file : files)
    {
      const auto fileName = std::filesystem::path{ file }.string();
      if (checkedFileNames.contains(fileName))
      {
        const auto conflictedFile = checkedFileNames.at(fileName);

        if (!conflictFiles.contains(conflictedFile))
          conflictFiles.insert(conflictedFile);

        conflictFiles.insert(fileName);
      }
      else
        checkedFileNames.insert({ fileName, file });
    }

    return conflictFiles;
  }

  std::tuple<IncludePaths, InputFiles, OutputFolder, ErrorCode> ParseOptions(const int argc, const char** argv)
  {
    std::vector<std::string> includePaths;
    std::vector<std::string> inputFiles;
    std::string outputFolder = "";

    po::options_description desc("Allowed options");
    desc.add_options()
      ("help", "prints help")
      ("include-path,I", po::value<std::vector<std::string>>(&includePaths), "include path")
      ("input-files", po::value<std::vector<std::string> >(&inputFiles), "input files")
      ("output-folder,o", po::value<std::string>(&outputFolder), "output folder");

    po::positional_options_description p;
    p.add("input-files", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).allow_unregistered().run(), vm);
    po::notify(vm);

    const auto filesWithNotUniqueNames = FindFilesWithNotUniqueNames(inputFiles);
    if (!filesWithNotUniqueNames.empty())
    {
      std::printf("error: input file names has to be unique.\n Conflict files: ");
      std::copy(filesWithNotUniqueNames.begin(), filesWithNotUniqueNames.end(), std::ostream_iterator<std::string>(std::cout, "\n "));
    }

    if (outputFolder == "")
    {
      std::printf("output folder has to be specified. (--output-folder, -o)\n");
      return { {}, {},  {}, FailedToParse };
    }

    return {includePaths, inputFiles, outputFolder, OK};
  }

  void PrintOptions(const ReflectionHeaderTool::Options& opts)
  {
    std::printf("///////// RHT options //////////\n");
    std::printf("Files to parse:\n ");
    std::copy(opts.filesToParse.begin(), opts.filesToParse.end(), std::ostream_iterator<std::string>(std::cout, "\n "));

    std::printf("\nIncludes:\n ");
    std::copy(opts.includePaths.begin(), opts.includePaths.end(), std::ostream_iterator<std::string>(std::cout, "\n "));

    std::printf("\nOutput folder:\n %s", opts.outputFolder.c_str());

    std::printf("\nClang default args:\n ");
    std::copy(opts.clangToolArgv.begin(), opts.clangToolArgv.end(), std::ostream_iterator<const char*>(std::cout, "\n "));
    std::printf("////////////////////////////////\n");
  }
}

int main(int argc, const char** argv)
{
  auto[includePaths, inputFiles, outputFolder, errCode] = ParseOptions(argc, argv);
  if (errCode)
    return errCode;

  if (inputFiles.empty())
    return OK;

  const auto opts = ReflectionHeaderTool::Options{
    .filesToParse = inputFiles,
    .includePaths = includePaths,
    .outputFolder = outputFolder,
    .clangToolArgv = {
      "--",
      "-xc++",
      "-std=c++17"
    }
  };

  PrintOptions(opts);

  return ReflectionHeaderTool().Parse(opts);
}