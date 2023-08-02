#include "resource_request.hpp"
#include "manager.h"

namespace fg
{
  extern
  auto get_manager() -> Manager*;

  auto TextureAccessor::access(const virt_res_id_t id, const Timeline timeline) -> return_type
  {
    return get_manager()->getTexture(id, timeline);
  }

  auto TextureAccessor::getAllocDescription(const virt_res_id_t id) -> alloc_desc_return_type
  {
    return get_manager()->getTextureAllocDescription(id);
  }

  auto BlobAccessorInternal::getBlob(const virt_res_id_t id) -> std::byte*
  {
    return get_manager()->getBlob(id);
  }

  auto SamplerAccessor::access(const virt_res_id_t id, const Timeline) -> return_type
  {
    return get_manager()->getSampler(id);
  }

  auto SamplerAccessor::getAllocDescription(const virt_res_id_t id) -> alloc_desc_return_type
  {
    return get_manager()->getSamplerAllocDescription(id);
  }

  auto BufferAccessor::access(const virt_res_id_t id, const Timeline) -> return_type
  {
    return get_manager()->getBuffer(id);
  }

  auto BufferAccessor::getAllocDescription(const virt_res_id_t id) -> alloc_desc_return_type
  {
    return get_manager()->getBufferAllocDescription(id);
  }
}