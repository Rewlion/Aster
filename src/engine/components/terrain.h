#pragma once

#include <engine/types.h>

namespace ed
{
  class Scope;
}

class VTerrainComponent
{
  public:
    VTerrainComponent() = default;
    VTerrainComponent(const ed::Scope*);

    auto getTerrainName() const -> string_view { return m_TerrainName; }
    auto getHeightmapMinMaxBorder() const -> float2 { return m_HeightmapMinMaxBorder; }
    auto getHeightmapMaxHeight() const -> float { return m_HeightmapMaxHeight; }
    auto getPathSideBits() const -> int { return m_PatchSideBits; }
    auto getWorldSize() const -> int { return m_WorldSize; }
    auto getDetailSize() const -> int { return m_DetailSize; }

  private:
    string m_TerrainName;
    float2 m_HeightmapMinMaxBorder;
    float m_HeightmapMaxHeight;
    int m_PatchSideBits;
    int m_WorldSize;
    int m_DetailSize;
};
