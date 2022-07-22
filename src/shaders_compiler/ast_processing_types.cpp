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
}

namespace gapi
{
  bool VertexInputDescription::Attribute::operator==(const VertexInputDescription::Attribute& rvl) const
  {
    return std::tie(offset, location, binding, type) ==
           std::tie(rvl.offset, rvl.location, rvl.binding, rvl.type);
  }

  bool VertexInputDescription::Buffer::operator==(const VertexInputDescription::Buffer& rvl) const
  {
    return std::tie(stride, binding) == std::tie(rvl.stride, rvl.binding);
  }

  bool VertexInputDescription::operator==(const VertexInputDescription& ia) const
  {
    return attributes == ia.attributes &&
           buffers == ia.buffers;
  }
}

