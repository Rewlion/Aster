#pragma once

#include <engine/algorithm/hash.h>
#include <engine/types.h>
#include <engine/utils/strong_typedef.h>

#include <memory>
#include <variant>
#include <optional>

#include <EASTL/vector.h>
#include <EASTL/vector_map.h>

#ifndef ED_ASSERT_FMT_RETURN
#define ED_ASSERT_FMT_RETURN(cond, ret_val, f, ...) ASSERT_FMT_RETURN(cond, ret_val, f, __VA_ARGS__)
#endif

STRONG_SCALAR_TYPEDEF(uint16_t, engine_data_type_id_t);

enum class ExpectedTypeValue : uint8_t
{
  Int, Int2, Int3, Int4,
  Float, Float2, Float3, Float4,
  Text, Complex,
};

class TypesRegistry
{
  class RegisteredCompileTypeId
  {
    public:
      template<class T>
      static
      auto from() -> engine_data_type_id_t
      {
        return fromInternal<typename std::remove_cvref<T>::type>();
      }

    private:
      template<class T>
      static
      auto fromInternal() -> engine_data_type_id_t
      {
        static engine_data_type_id_t id = engine_data_type_id_t{m_LastId++};
        return id;
      }

    private:
      static engine_data_type_id_t m_LastId;
  };
public:
  struct Entry
  {
    engine_data_type_id_t compileTypeId;
    ExpectedTypeValue expectedTypeValue;
    string name;
    string_hash nameHash;
  };

  template<class T>
  constexpr
  void add(const char* type_name, const ExpectedTypeValue expected_type_value)
  {
    const engine_data_type_id_t tId = RegisteredCompileTypeId::template from<T>();
    const string_hash nameHash = str_hash(type_name);
    
    const size_t entryId = m_Entries.size();

    m_Entries.push_back(Entry{
      .compileTypeId = tId,
      .expectedTypeValue = expected_type_value,
      .name = string{type_name},
      .nameHash = nameHash
    });

    m_NameHashToEntryId.insert({nameHash, entryId});
    m_CompileTimeIdToEntryId.insert({tId, entryId});
  }

  auto get(const string& type_name) -> const Entry*
  {
    const string_hash nameHash = str_hash(type_name);
    const auto it = m_NameHashToEntryId.find(nameHash);
    if (it != m_NameHashToEntryId.end())
      return &m_Entries[it->second];

    return nullptr;
  }

  auto get(const engine_data_type_id_t& type_id) -> const Entry*
  {
    const auto it = m_CompileTimeIdToEntryId.find(type_id);
    if (it != m_CompileTimeIdToEntryId.end())
      return &m_Entries[it->second];

    return nullptr;
  }

private:
  eastl::vector<Entry> m_Entries;
  eastl::vector_map<string_hash, size_t> m_NameHashToEntryId;
  eastl::vector_map<engine_data_type_id_t, size_t> m_CompileTimeIdToEntryId;
};

template <class ValueConverter>
class TEngineData
{
public:
  template<class T>
  struct TrivialValue
  {
    T data;
  };

  struct ComplexValue
  {
    std::unique_ptr<TEngineData> data;
  };

  using AttributeValue = std::variant<
    TrivialValue<int>,   TrivialValue<int2>,   TrivialValue<int3>,   TrivialValue<int4>,
    TrivialValue<float>, TrivialValue<float2>, TrivialValue<float3>, TrivialValue<float4>,
    TrivialValue<string>,
    ComplexValue
  >;

  struct Attribute
  {
    string name;
    string annotation;
    AttributeValue value;
    engine_data_type_id_t typeId;
  };

public:
  void addAttribute(string&& name, string&& annotation, AttributeValue&& v, const string& type_name)
  {
    const TypesRegistry::Entry* typeReg = m_TypesRegistry->get(type_name);

    ED_ASSERT_FMT_RETURN(typeReg != nullptr, ,
      "can't add attribute {}:{} because it's not registered in the EngineData", name, type_name);

    ED_ASSERT_FMT_RETURN((size_t)typeReg->expectedTypeValue == v.index(), ,
      "can't add attribute {}:{} because provided value type differs from the registered one", name, type_name);

    m_Attributes.push_back(Attribute{
      .name = std::move(name),
      .annotation = std::move(annotation),
      .value = std::move(v),
      .typeId = typeReg->compileTypeId
    });
  }

  void insertScope(TEngineData&& scope)
  {
    m_Scopes.push_back(std::move(scope));
  }

  TEngineData() = default;
  TEngineData(const std::shared_ptr<TypesRegistry>& reg)
    : m_TypesRegistry(reg)
  {
  }

private:
  std::shared_ptr<TypesRegistry> m_TypesRegistry;
  eastl::vector<Attribute> m_Attributes;
  eastl::vector<TEngineData> m_Scopes;
};