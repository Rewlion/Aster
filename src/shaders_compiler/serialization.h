#pragma once

#define ZPP_BITS_AUTODETECT_MEMBERS_MODE 1
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-attributes"
#include <zpp_bits/zpp_bits.h>
#pragma clang diagnostic pop

namespace vk
{
  template<class T>
  constexpr auto serialize(auto& archive, const vk::Flags<T>& flag)
  {
    return archive(zpp::bits::as_bytes(flag));
  }

  template<class T>
  constexpr auto serialize(auto& archive, vk::Flags<T>& flag)
  {
    return archive(zpp::bits::as_bytes(flag));
  }
}

namespace spirv::v2
{
  constexpr auto serialize(auto& archive, const DescriptorSetBinding& s)
  {
    return archive(zpp::bits::as_bytes(s));
  }

  constexpr auto serialize(auto& archive, DescriptorSetBinding& s)
  {
    return archive(zpp::bits::as_bytes(s));
  }
}

namespace glm
{
  template<size_t N, class T>
  constexpr auto serialize(auto& archive, const vec<N,T>& v)
  {
    return archive(zpp::bits::as_bytes(v));
  }

  template<size_t N, class T>
  constexpr auto serialize(auto& archive, vec<N,T>& v)
  {
    return archive(zpp::bits::as_bytes(v));
  }
}
