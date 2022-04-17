#include "materials.h"

#include <engine/algorithm/hash.h>
#include <engine/log.h>
#include <engine/assert.h>

namespace Engine
{
  static const char* get_param_type_name(const Material::BindingType type)
  {
    switch (type)
    {
      case Material::BindingType::Texture: return "texture";
      default: ASSERT(!"unsupported"); return "";
    }
  }

  bool Material::addParams(const Params& params)
  {
    bool isValid = true;

    for(const auto& param: params)
    {
      const param_name_hash paramName = str_hash(param.name.c_str());
      auto it = m_ParamsMap.find(paramName);

      if (it == m_ParamsMap.end())
      {
        logerror("Material ({}) set params error: doesn't have param with name ({})", m_Name, param.name);
        isValid = false;
        continue;
      }

      if (it->second.type != param.type)
      {
        logerror("Material ({}) set params error: param ({}) has wrong type ({}) expected: ({})",
          m_Name, param.name,
          get_param_type_name(param.type),
          get_param_type_name(it->second.type));
        isValid = false;
        continue;
      }

      Param p = param;
      p.set = it->second.set;
      p.binding = it->second.binding;

      m_Params.push(p);
    }

    return isValid;
  }

  void Material::setParams(gapi::CmdEncoder& encoder)
  {
    for (const auto& param: m_Params)
    {
      switch(param.type)
      {
        case BindingType::Texture:
        {
          const auto h = (gapi::TextureHandler)(param.handler);
          encoder.bindTexture(h, param.set, param.binding);
          break;
        }

        default:
        {
          ASSERT(!"unsupported");
        }
      }
    }
  }

  void Material::setState(gapi::CmdEncoder& encoder, const RenderPassType pass)
  {
    encoder.bindGraphicsShaders(m_RenderState.shaders);
    encoder.setDepthStencil(m_RenderState.depthStencil);
  }
}