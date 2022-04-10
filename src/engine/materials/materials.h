#pragma once

#include <engine/gapi/resources.h>
#include <engine/gapi/cmd_encoder.h>
#include <engine/utils/fixed_stack.hpp>

#include <EASTL/vector_map.h>

namespace Engine
{
  enum class RenderPassType
  {
    Main = 0
  };

  class Material
  {
    public:
      enum RenderFlags: uint64_t
      {
        RF_OPAQUE = 1,
        RF_TRANSPARENT = 2
      };

      enum class BindingType
      {
        Texture = 0
      };

      struct MaterialBinding
      {
        size_t      set = 0;
        size_t      binding = 0;
        BindingType type;
      };

      struct Param
      {
        friend Material;

        string name;
        gapi::ResourceHandler handler;
        BindingType type;

        private:
          size_t set, binding;
      };

      using Params = Utils::FixedStack<Param, 32>;

    public:
      bool addParams(const Params& params);
      void setState(gapi::CmdEncoder& encoder, const RenderPassType pass);
      void setParams(gapi::CmdEncoder& encoder);

      inline gapi::PrimitiveTopology getTopology() const
      {
        return m_RenderState.topology;
      }

    protected:
      string m_Name;

      typedef string_hash param_name_hash;
      eastl::vector_map<param_name_hash, MaterialBinding> m_ParamsMap;
      Params m_Params;

      gapi::RenderState m_RenderState;
  };
}
