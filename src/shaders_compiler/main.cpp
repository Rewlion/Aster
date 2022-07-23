#include "compiler.h"

#include "serialization.h"
#include "spirv.h"

#include <engine/assert.h>
#include <engine/datablock/utils.h>
#include <engine/log.h>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/program_options.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>

namespace po = boost::program_options;
namespace fs = std::filesystem;

int main(int argc, char** argv)
{
  Engine::InitLog();
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help", "produce help message")
    ("blk", po::value<string>(), "path to blk with shaders list")
    ("o", po::value<string>(), "output dir")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("blk") == 0 || vm.count("o") == 0 || vm.count("help")) {
      std::cout << desc << "\n";
      return 0;
  }

  const string blk = vm["blk"].as<string>();
  const string outputDir = vm["o"].as<string>();

  DataBlock shadersBlk;
  if (!load_blk_from_file(&shadersBlk, blk.c_str()))
  {
    logerror("failed to load shaders from `{}`", blk);
    return -1;
  }

  DataBlock* shadersList = shadersBlk.getChildBlock("shaders_list");

  const string outFile = outputDir + "/shaders_spirv.bin";
  log("compiling shaders to {}", outFile);
  fs::remove(outFile);

  std::error_code ec;
  fs::create_directories(outputDir, ec);
  if (ec)
  {
    log("failed to create output directory `{}`:{}", outputDir, ec.message());
    return -1;
  }

  std::ofstream out("tools/bin", std::ios::binary);
  if (!out.is_open())
  {
    logerror("failed to open {}", outFile);
    return -1;
  }

  ShadersSystem::Compiler compiler;
  const string blkDir = fs::path(blk).parent_path().string();
  for (const auto& shader: shadersList->getChildBlocks())
  {
    const auto shFile = blkDir + "/" + shader.getName();
    log("compiling {}", shFile);
    compiler.compileModuleFromFile(shFile);
  }

  const ShadersSystem::MaterialsBin& mBin = compiler.getMaterialsBins();
  boost::archive::binary_oarchive archive(out);
  archive << mBin;

  log("done");
  return 0;
}
