#include "compiler.h"

#include "serialization.h"
#include "spirv.h"

#include <engine/assert.h>
#include <engine/datablock/utils.h>
#include <engine/log.h>

#include <argparse/argparse.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

int main(int argc, char** argv)
{
  Engine::InitLog();

  argparse::ArgumentParser opts("Shaders Compiler");
  opts.add_argument("--blk")
      .help("path to blk with shaders list");
  
  opts.add_argument("--o")
      .help("output dir");

  opts.add_argument("-h", "--help")
      .flag()
      .help("show help");

  opts.add_argument("--D")
      .flag()
      .help("enable debugging and disable all optimizations");

  try
  {
    opts.parse_args(argc, argv);
  }
  catch (const std::exception& err)
  {
    logerror("error:{}\n\n{}", err.what(), opts.usage());
    return -1;
  }

  if (opts["-h"] != false || opts["--help"] != false || !opts.is_used("--blk") || !opts.is_used("--o"))
  {
    loginfo("{}", opts.usage());
    return 0;
  }

  const string blk = opts.get("--blk");
  const string outputDir = opts.get("--o");
  const bool debugMode = opts["--D"] == true;

  DataBlock shadersBlk;
  if (!load_blk_from_file(&shadersBlk, blk.c_str()))
  {
    logerror("failed to load shaders from `{}`", blk);
    return -1;
  }

  DataBlock* shadersList = shadersBlk.getChildBlock("shaders_list");

  const string outFile = outputDir + "/shaders_spirv.bin";
  loginfo("compiling shaders to {}", outFile);

  std::error_code ec;
  fs::create_directories(outputDir, ec);
  if (ec)
  {
    loginfo("failed to create output directory `{}`:{}", outputDir, ec.message());
    return -1;
  }

  ShadersSystem::Compiler compiler;
  const string blkDir = fs::path(blk).parent_path().string();
  for (const auto& shader: shadersList->getChildBlocks())
  {
    const auto shFile = blkDir + "/" + shader.getName();
    loginfo("compiling {}", shFile);
    const bool isOk = compiler.compileModuleFromFile(shFile, debugMode);
    if (!isOk)
      return -1;
  }

  fs::remove(outFile);
  std::ofstream out(outFile, std::ios::binary);
  if (!out.is_open())
  {
    logerror("failed to open {}", outFile);
    return -1;
  }

  const ShadersSystem::MaterialsBin& mBin = compiler.getMaterialsBins();
  auto [data, outStream] = zpp::bits::data_out();
  const std::errc err = outStream(mBin);
  if (std::error_code errCode = std::make_error_code(err))
  {
    logerror("failed to serialize materials bin: `{}`", errCode.message());
    return -1;
  }
  out.write(reinterpret_cast<const char*>(data.data()), data.size());

  loginfo("done");
  return 0;
}
