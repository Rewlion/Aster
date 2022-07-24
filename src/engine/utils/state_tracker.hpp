#pragma once

#include "types_array.hpp"
#include "bit_capacity.hpp"

namespace Utils
{
  #define TRACKED_STATE_FIELD_ATTR(type, fieldName)                       \
    type fieldName;                                                       \
    inline void set(const type& fieldName) {this->fieldName = fieldName;} \
    inline bool hasDiff(const type& fieldName) { return this->fieldName != fieldName; }

  #define TRACKED_STATE_FIELD_ATTR_PTR(type, fieldName)                   \
    const type  fieldName;                                                \
    inline void set(const type fieldName) {this->fieldName = fieldName;}  \
    inline bool hasDiff(const type fieldName) { return this->fieldName != fieldName; }

  #define TRACKED_STATE_FIELD_ATTR_MANAGED_PTR(type, fieldName) \
    const type  fieldName = nullptr;                            \
    inline void set(const type fieldName)                       \
    {                                                           \
      if (this->fieldName != nullptr)                           \
        delete[] this->fieldName;                               \
      this->fieldName = fieldName;                              \
    }                                                           \
    inline bool hasDiff(const type fieldName) { return this->fieldName != fieldName; }

  template<class Applier, class... TrackedStateFields>
  class TrackedState
  {
    public:
      void apply(Applier& applier)
      {
        (applyField<TrackedStateFields>(applier), ...);
        m_DirtyStates.resetAll();
      }

      template<class RequiredType, class Value>
      bool set(const Value& v)
      {
        constexpr const size_t typeId = Utils::get_type_Id<RequiredType, TrackedStateFields...>();

        auto& field = m_Storage.get<RequiredType>();
        if (field.hasDiff(v))
        {
          field.set(v);
          m_DirtyStates.set(typeId);

          return true;
        }

        return false;
      }

      template<class RequiredType, class Value>
      bool set(const Value& v, const size_t n)
      {
        constexpr const size_t typeId = Utils::get_type_Id<RequiredType, TrackedStateFields...>();

        auto& field = m_Storage.get<RequiredType>();
        if (field.hasDiff(v,n))
        {
          field.set(v,n);
          m_DirtyStates.set(typeId);

          return true;
        }

        return false;
      }

      template<class RequiredType>
      void markDirty()
      {
        constexpr const size_t typeId = Utils::get_type_Id<RequiredType, TrackedStateFields...>();
        m_DirtyStates.set(typeId);
      }

      void markDirtyAll()
      {
        m_DirtyStates.setAll();
      }

    private:
      template<class RequiredType>
      void applyField(Applier& applier)
      {
        constexpr const size_t typeId = Utils::get_type_Id<RequiredType, TrackedStateFields...>();

        if (m_DirtyStates.isSet(typeId))
        {
          RequiredType& field = m_Storage.get<RequiredType>();
          field.apply(applier);
        }
      }

    private:
      Utils::TypesArray<TrackedStateFields...> m_Storage;
      Utils::BitCapacity<sizeof...(TrackedStateFields)> m_DirtyStates;
  };
}
