#include "test_materials.h"

#include "storage.h"

namespace Engine
{
  REGISTER_MATERIAL(TestMaterial);

  TestMaterial::TestMaterial()
  {
    m_Name = "TestMaterial";
    m_ParamsMap = {
      {str_hash("test_texture"), {}}
    };

    m_RenderState.shaders = {str_hash("test_vs"), str_hash("test_ps")};
    m_RenderState.topology = gapi::PrimitiveTopology::TriangleList;
    m_RenderState.depthStencil = {
        .depthTestEnabled = true,
        .depthWriteEnabled = true,
        .depthOp = gapi::CompareOp::Greater
    };
  }
}
