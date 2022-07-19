#include "ast_processing_types.h"
#include "ast.h"

namespace ShadersSystem
{
  bool RegistersReserve::operator==(const RegistersReserve& rvl) const
  {
    return begin == rvl.begin && end == rvl.end;
  }

  bool ResourceDeclaration::operator==(const ResourceDeclaration& rvl) const
  {
    return type == rvl.type &&
           name == rvl.name &&
           binding == rvl.binding &&
           ((assignExp == rvl.assignExp) || (assignExp && rvl.assignExp && *assignExp == *rvl.assignExp));
  }

  bool CBufferVar::operator==(const CBufferVar& rvl) const
  {
    return type == rvl.type &&
           name == rvl.name &&
           offset == rvl.offset &&
           ((assignExp == rvl.assignExp) || (assignExp && rvl.assignExp && *assignExp == *rvl.assignExp));
  }

  bool InputDescription::Attribute::operator==(const Attribute& rvl) const
  {
    return std::tie(offset, location, binding, type) ==
           std::tie(rvl.offset, rvl.location, rvl.binding, rvl.type);
  }

  bool InputDescription::Buffer::operator==(const InputDescription::Buffer& rvl) const
  {
    return std::tie(stride, binding) == std::tie(rvl.stride, rvl.binding);
  }

  bool InputDescription::operator==(const InputDescription& ia) const
  {
    return attributes == ia.attributes &&
           buffers == ia.buffers;
  }
}
