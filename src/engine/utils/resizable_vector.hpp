#pragma once

#include <EASTL/vector.h>

namespace Utils
{
  template<class T, class IdType>
  class ResizableVector : public eastl::vector<T>
  {
    using base_type = eastl::vector<T>;
    public:
      auto operator[](const IdType strong_id)
        -> typename base_type::reference 
      {
        const size_t id = (size_t) strong_id;
        if (base_type::size() <= id)
          base_type::resize(id+1);
        
        return base_type::operator[] (id);
      }

      auto get(const IdType strong_id) -> typename base_type::reference
      {
        return base_type::operator[] ((size_t) strong_id);
      }

    private:
      auto operator[](typename base_type::size_type n) -> typename base_type::reference;
		  auto operator[](typename base_type::size_type n) const -> typename base_type::const_reference;
      auto at(typename base_type::size_type n) -> typename base_type::reference;
		  auto at(typename base_type::size_type n) const -> typename base_type::const_reference;
  };
}
