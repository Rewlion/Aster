#pragma once

#include <engine/utils/types_array.hpp>
#include <engine/utils/bit_capacity.hpp>

#define TRACKED_STATE_FIELD_ATTR(type, fieldName)                       \
  type fieldName;                                                       \
  inline void Set(const type& fieldName) {this->fieldName = fieldName;} \
  inline bool HasDiff(const type& fieldName) { return this->fieldName != fieldName; }

#define TRACKED_STATE_FIELD_ATTR_PTR(type, fieldName)                   \
  const type  fieldName;                                                \
  inline void Set(const type fieldName) {this->fieldName = fieldName;}  \
  inline bool HasDiff(const type fieldName) { return this->fieldName != fieldName; }

namespace gapi::vulkan
{
  class CompileContext;
  struct BackendState;

  template<class T>
  class TrackedStateField
  {
    public:
      void Set(const T& data)
      {
        m_Data = data;
      }

      void Apply(CompileContext& ctx)
      {
      }

      bool HasDiff(const T& with) const
      {
        return m_Data != with;
      }

    private:
      T m_Data;
  };

  template<class... TrackedStateFields>
  class TrackedState
  {
    public:
      void Apply(CompileContext& ctx, BackendState& state)
      {
        (ApplyField<TrackedStateFields>(ctx, state), ...);
        m_DirtyStates.ResetAll();
      }

      template<class RequiredType, class Value>
      bool Set(const Value& v)
      {
        constexpr const size_t typeId = Utils::GetTypeId<RequiredType, TrackedStateFields...>();

        auto& field = m_Storage.Get<RequiredType>();
        if (field.HasDiff(v))
        {
          field.Set(v);
          m_DirtyStates.Set(typeId);

          return true;
        }

        return false;
      }

      template<class RequiredType>
      void MarkDirty()
      {
        constexpr const size_t typeId = Utils::GetTypeId<RequiredType, TrackedStateFields...>();
        m_DirtyStates.Set(typeId);
      }

      void MarkDirtyAll()
      {
        m_DirtyStates.SetAll();
      }

    private:
      template<class RequiredType>
      void ApplyField(CompileContext& ctx, BackendState& state)
      {
        constexpr const size_t typeId = Utils::GetTypeId<RequiredType, TrackedStateFields...>();

        if (m_DirtyStates.IsSet(typeId))
        {
          RequiredType& field = m_Storage.Get<RequiredType>();
          field.Apply(ctx, state);
        }
      }

    private:
      Utils::TypesArray<TrackedStateFields...> m_Storage;
      Utils::BitCapacity<sizeof...(TrackedStateFields)> m_DirtyStates;
  };
}
