#include "resources.h"

#include <boost/functional/hash.hpp>

namespace gapi
{
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
    hash_combine(hash, blendState.hash());

    return hash;
  }

  size_t AttachmentBlendState::hash() const
  {
    using boost::hash_combine;
    size_t hash = 0;

     hash_combine(hash, blendEnabled);
     hash_combine(hash, srcColorBlendFactor);
     hash_combine(hash, dstColorBlendFactor);
     hash_combine(hash, colorBlendOp);
     hash_combine(hash, srcAlphaBlendFactor);
     hash_combine(hash, dstAlphaBlendFactor);
     hash_combine(hash, alphaBlendOp);

    return hash;
  }

  size_t BlendState::hash() const
  {
    using boost::hash_combine;
    size_t hash = 0;

    hash_combine(hash, logicOpEnabled);
    hash_combine(hash, logicOp);
    hash_combine(hash, blendConstants[0]);
    hash_combine(hash, blendConstants[1]);
    hash_combine(hash, blendConstants[2]);
    hash_combine(hash, blendConstants[3]);

    for(size_t i = 0; i < attachmentsCount; ++i)
      hash_combine(hash, attachments[i].hash());

    return hash;
  }
}
