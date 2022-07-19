#pragma once

#include "ast_processing_types.h"

namespace ShadersSystem
{
  struct MaterialsBin
  {
    struct Scope
    {
      Scope(const ShadersSystem::ScopeDeclaration& sc)
        : name(sc.name)
        , cbufferReg(sc.cbuffer)
        , cbufferSize(sc.cbufferSize)
        , byteCode(sc.byteCode)
      {
      }

      string name;
      uint8_t cbufferReg;
      uint16_t cbufferSize;
      ByteCodes byteCode;
    };

    typedef TechniqueDeclaration Technique;

    eastl::vector<Scope> scopes;
    eastl::vector<Technique> techniques;
  };
}
