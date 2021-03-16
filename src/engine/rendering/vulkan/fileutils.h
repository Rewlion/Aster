#pragma once

#include <fstream>
#include <vector>

namespace Vulkan
{
  inline std::vector<uint32_t> ReadFile(const std::string location)
  {
    std::ifstream file{ location, std::ios::binary };
    if (file.is_open() == false)
      throw std::runtime_error("file not found");

    std::vector<uint32_t> raw;

    file.seekg(0, std::ios_base::end);
    const size_t fileSize = file.tellg();

    raw.resize(fileSize / sizeof(uint32_t));
    file.seekg(0);

    file.read(reinterpret_cast<char*>(raw.data()), fileSize);

    return raw;
  }
}
