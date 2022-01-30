#include "resources.h"

#include <boost/functional/hash.hpp>

namespace gapi
{
  size_t BeginRenderPassCmd::hash() const
  {
    using boost::hash_combine;
    size_t hash = 0;

    for(size_t i = 0; i < MAX_RENDER_TARGETS; ++i)
    {
      auto& rt = renderTargets[i];
      hash_combine(hash, rt.loadOp);
      hash_combine(hash, rt.storeOp);
    }

    hash_combine(hash, depthStencil.texture);
    hash_combine(hash, depthStencil.depthLoadOp);
    hash_combine(hash, depthStencil.depthStoreOp);
    hash_combine(hash, depthStencil.stencilLoadOp);
    hash_combine(hash, depthStencil.stencilStoreOp);

    return hash;
  }

  size_t DepthStencilStateDescription::hash() const
  {
    using boost::hash_combine;
    size_t hash = 0;

    hash_combine(hash, depthTestEnabled);
    hash_combine(hash, depthWriteEnabled);
    hash_combine(hash, depthOp);

    hash_combine(hash, stencilTestEnabled);
    hash_combine(hash, stencilFailOp);
    hash_combine(hash, stencilPassOp);
    hash_combine(hash, stencilDepthFailOp);
    hash_combine(hash, stencilCompareOp);
    hash_combine(hash, stencilReferenceValue);

    return hash;
  }

  size_t GraphicsPipelineDescription::hash() const
  {
    using boost::hash_combine;
    size_t hash = 0;

    for (size_t i = 0; i < shadersCount; ++i)
      hash_combine(hash, shadersNames[i]);
    hash_combine(hash, topology);
    hash_combine(hash, depthStencilState.hash());

    return hash;
  }
}
