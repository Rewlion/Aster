
#include "spirv.h"
#include "constants.h"

#include <engine/assert.h>
#include <engine/datablock/utils.h>
#include <engine/log.h>
#include <engine/types.h>
#include <engine/utils/fs.h>
#include <engine/gapi/resources.h>

#include <boost/program_options.hpp>
#include <Unknwn.h>
#include <dxc/dxcapi.h>

#include <combaseapi.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <wrl/client.h>

namespace po = boost::program_options;
namespace fs = std::filesystem;


struct ShaderBlob
{
  char                shaderName[spirv::SHADERS_NAME_LEN];
  gapi::ShaderStage   stage;
  spirv::Reflection   reflection;
  eastl::vector<char> src;
};

class ShadersCompiler
{
  public:
    void compileShaders(const DataBlock& shaders, const string& blkDir, const string& outputDir)
    {
      m_BlkDir = blkDir;

      const string outFile = outputDir + "/shaders_spirv.bin";
      log("compiling shaders to {}", outFile);
      fs::remove(outFile);

      std::error_code ec;
      fs::create_directories(outputDir, ec);
      if (ec)
      {
        log("failed to create output directory `{}`:{}", outputDir, ec.message());
        return;
      }

      m_OutputFile.open(outFile, std::ios::binary);
      if (!m_OutputFile.is_open())
      {
        logerror("failed to open {}", outFile);
        return;
      }

      for (const auto& shader: shaders.getChildBlocks())
        if (!Compile(shader))
          return;

      writeShadersFile();
    }

  private:

    bool Compile(const DataBlock& shader)
    {
      const string shaderFile = m_BlkDir + "/" + shader.getName();
      log("compiling `{}`", shaderFile);

      const auto shaderData = Utils::read_file(shaderFile);
      if (shaderData.size() == 0)
      {
        logerror("failed to read shader data");
        return false;
      }

      const auto compileShader = [&](const char* blkName, const char* entry, const gapi::ShaderStage stage)
      {
        const DataBlock* stageBlk = shader.getChildBlock(blkName);
        const string currentDir = Utils::get_dir_name(shaderFile);
        if (!stageBlk->isEmpty())
        {
          const string name = stageBlk->getText("name");
          const string entry = stageBlk->getText("entry");

          ShaderBlob blob;
          if (compileShaderStage(blob, shaderData, stage, name, entry, currentDir))
          {
            m_Shaders.push_back(std::move(blob));
            return true;
          }
        }

        return false;
      };

      if (!compileShader("vs", "vs_main", gapi::ShaderStage::Vertex))
        return false;

      if (!compileShader("ps", "ps_main", gapi::ShaderStage::Fragment))
        return false;

      return true;
    }

    inline const wchar_t* getShaderTarget(const gapi::ShaderStage stage) const
    {
      switch(stage)
      {
        case gapi::ShaderStage::Vertex: { return L"vs_6_0"; }
        case gapi::ShaderStage::Fragment: { return L"ps_6_0"; }

        default:
        {
          logerror("unsupported stage: {}", ShaderStageToText(stage));
          ASSERT(!"stage is not supported");
          return L"";
        }
      }
    }

    bool compileShaderStage(ShaderBlob& result, const eastl::vector<char>& shaderData,
                           const gapi::ShaderStage stage, const string& shaderName,
                           const string& entryName, const string& currentDir)
    {
      log("compiling stage: {}, entry:{}, shader:{}", gapi::ShaderStageToText(stage), entryName, shaderName);

      using Microsoft::WRL::ComPtr;
      ComPtr<IDxcUtils> pUtils;
      DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(pUtils.GetAddressOf()));

      ComPtr<IDxcCompiler3> pCompiler;
      DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(pCompiler.GetAddressOf()));

      ComPtr<IDxcBlobEncoding> pSource;
      pUtils->CreateBlob(shaderData.data(), shaderData.size(), CP_UTF8, pSource.GetAddressOf());

      ComPtr<IDxcIncludeHandler> pIncludeHandler;
      pUtils->CreateDefaultIncludeHandler(&pIncludeHandler);

      const std::wstring entry(entryName.begin(), entryName.end());
      const std::wstring dir(currentDir.begin(), currentDir.end());

      std::vector<LPCWSTR> args;
      args.push_back(L"-spirv");
      args.push_back(L"-HV");
      args.push_back(L"2018");
      args.push_back(L"-fspv-target-env=vulkan1.1");
      args.push_back(L"-E");
      args.push_back(entry.c_str());
      args.push_back(L"-T");
      args.push_back(getShaderTarget(stage));
      args.push_back(L"-I");
      args.push_back(dir.c_str());
      args.push_back(L"-D");
      args.push_back(L"_TARGET_SHADER");

      DxcBuffer dxcSrc;
      dxcSrc.Ptr = pSource->GetBufferPointer();
      dxcSrc.Size = pSource->GetBufferSize();
      dxcSrc.Encoding = 0;

      ComPtr<IDxcResult> pCompileResult;
      HRESULT hr = pCompiler->Compile(&dxcSrc, args.data(), args.size(), pIncludeHandler.Get(), IID_PPV_ARGS(pCompileResult.GetAddressOf()));
      ASSERT(SUCCEEDED(hr));

      {
        const char* error;

        ComPtr<IDxcBlobUtf8> dxcErrors;
        hr = pCompileResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(dxcErrors.GetAddressOf()), nullptr);
        ASSERT(SUCCEEDED(hr));

        if (dxcErrors && dxcErrors->GetStringLength() > 0)
        {
          error = (char*)dxcErrors->GetBufferPointer();
          logerror("compilation failed: {}", error);
          return false;
        }
      }

      ComPtr<IDxcBlob> blob;
      hr = pCompileResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(blob.GetAddressOf()), nullptr);
      ASSERT(SUCCEEDED(hr));

      ASSERT(blob->GetBufferSize() != 0);

      result.src.resize(blob->GetBufferSize());
      std::memcpy(result.src.data(), blob->GetBufferPointer(), blob->GetBufferSize());

      result.stage = stage;
      result.reflection = spirv::reflect(result.src);

      std::snprintf(result.shaderName, spirv::SHADERS_NAME_LEN, "%s", shaderName.c_str() );

      return true;
    }

    void writeShadersFile()
    {
      uint64_t buf64 = 0;

      buf64 = spirv::SHADERS_MAGIC;
      m_OutputFile.write((char*)&buf64, sizeof(buf64));

      buf64 = m_Shaders.size();
      m_OutputFile.write((char*)&buf64, sizeof(buf64));

      for(const auto& blob: m_Shaders)
        writeShadersBlobToFile(blob);

      log("saved bin file");
    }

    void writeShadersBlobToFile(const ShaderBlob& blob)
    {
      uint64_t buf64 = 0;
      const uint64_t stage = blob.stage;
      const uint64_t srcSize = blob.src.size();

      m_OutputFile.write(blob.shaderName, spirv::SHADERS_NAME_LEN);

      m_OutputFile.write((char*)&blob.reflection, sizeof(blob.reflection));

      buf64 = srcSize;
      m_OutputFile.write((char*)&buf64, sizeof(buf64));

      m_OutputFile.write(blob.src.data(), srcSize);
    }

  private:
    DataBlock m_ShadersBlk;
    string m_BlkDir;
    std::ofstream m_OutputFile;
    eastl::vector<ShaderBlob> m_Shaders;
};

int main(int argc, char** argv)
{
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

  Engine::InitLog();

  DataBlock shadersBlk;
  if (!load_blk_from_file(&shadersBlk, blk.c_str()))
  {
    logerror("failed to load shaders from `{}`", blk);
    return -1;
  }

  DataBlock* shadersList = shadersBlk.getChildBlock("shaders_list");
  ShadersCompiler compiler;

  try
  {
    const string blkDir = fs::path(blk).parent_path().string();
    compiler.compileShaders(*shadersList, blkDir, outputDir);
  }
  catch(std::exception& e)
  {
    logerror("compilation interrupted via exception: {}", e.what());
  }
}