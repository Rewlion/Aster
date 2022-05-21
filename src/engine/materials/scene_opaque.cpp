#include "materials.h"
#include "storage.h"

#include <engine/shaders/shaders/uniforms_common.hlsl>

namespace Engine
{
  class SceneOpaqueMaterial: public Material
  {
    public:
      SceneOpaqueMaterial()
      {
        m_Name = "SceneOpaqueMaterial";
        m_ParamsMap = {
          {str_hash("albedo"), {DSET_PER_MATERIAL, 0, BindingType::Texture}},
          {str_hash("metal_roughness"), {DSET_PER_MATERIAL, 1, BindingType::Texture}},
          {str_hash("normal"), {DSET_PER_MATERIAL, 2, BindingType::Texture}}
        };

        m_RenderState.shaders = {str_hash("scene_opaque_vs"), str_hash("scene_opaque_ps")};
        m_RenderState.topology = gapi::PrimitiveTopology::TriangleList;
        m_RenderState.depthStencil = {
            .depthTestEnabled = true,
            .depthWriteEnabled = true,
            .depthOp = gapi::CompareOp::LessOrEqual
        };
      }
  };

  REGISTER_MATERIAL(SceneOpaqueMaterial);
}