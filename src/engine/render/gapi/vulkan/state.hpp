#pragma once

#include <engine/utils/types_array.hpp>
#include <engine/utils/bit_capacity.hpp>

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

namespace gapi::vulkan
{
  class CompileContext;
  struct BackendState;

  template<class T>
  class TrackedStateField
  {
    public:
      void set(const T& data)
      {
        m_Data = data;
      }

      void apply(CompileContext& ctx)
      {
      }

      bool hasDiff(const T& with) const
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
      void apply(CompileContext& ctx, BackendState& state)
      {
        (applyField<TrackedStateFields>(ctx, state), ...);
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
      void applyField(CompileContext& ctx, BackendState& state)
      {
        constexpr const size_t typeId = Utils::get_type_Id<RequiredType, TrackedStateFields...>();

        if (m_DirtyStates.isSet(typeId))
        {
          RequiredType& field = m_Storage.get<RequiredType>();
          field.apply(ctx, state);
        }
      }

    private:
      Utils::TypesArray<TrackedStateFields...> m_Storage;
      Utils::BitCapacity<sizeof...(TrackedStateFields)> m_DirtyStates;
  };
}
