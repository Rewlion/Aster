#pragma once

#include "materials.h"

#include <engine/algorithm/hash.h>

namespace Engine
{
  class TestMaterial: public Material
  {
    public:
      TestMaterial()
      {
        m_Name = "TestMaterial";
        m_ParamsMap = {
          {str_hash("test_texture"), {}}
        };

        m_RenderState.shaders = {str_hash("test_vs"), str_hash("test_ps")};
        m_RenderState.topology = gapi::PrimitiveTopology::TriangleList;
      }
  };
}
