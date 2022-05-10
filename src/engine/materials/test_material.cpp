#include "test_materials.h"
#include "storage.h"

#include <engine/shaders/shaders/uniforms_common.hlsl>

namespace Engine
{
  REGISTER_MATERIAL(TestMaterial);

  TestMaterial::TestMaterial()
  {
    m_Name = "TestMaterial";
    m_ParamsMap = {
      {str_hash("test_texture"), {DSET_PER_MATERIAL, 0, BindingType::Texture}}
    };

    m_RenderState.shaders = {str_hash("test_vs"), str_hash("test_ps")};
    m_RenderState.topology = gapi::PrimitiveTopology::TriangleList;
    m_RenderState.depthStencil = {
        .depthTestEnabled = true,
        .depthWriteEnabled = true,
        .depthOp = gapi::CompareOp::LessOrEqual
    };
  }
}
