#pragma once

#include <engine/algorithm/hash.h>
#include <engine/assert.h>
#include <engine/types.h>
#include <engine/utils/concept.hpp>
#include <engine/utils/strong_typedef.h>

#include <memory>
#include <variant>
#include <optional>

#include <EASTL/vector.h>
#include <EASTL/vector_map.h>

STRONG_SCALAR_TYPEDEF(uint16_t, engine_data_type_id_t);

#define ED_BASE_VALUE_TYPES int, int2, int3, int4, float, float2, float3, float4, string

template<class T>
concept IsEdVarCustomType = !Utils::IsAnyOf<std::remove_cvref_t<T>, ED_BASE_VALUE_TYPES>;

class TEngineData;

template<class T>
concept HasCtorForEngineData = requires (const TEngineData* data){
  T{data};
};

class CustomTypeRegistry
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
    string name;
    string_hash nameHash;
  };

  template<class T>
  requires HasCtorForEngineData<T>
  constexpr
  void add(const char* type_name)
  {
    const engine_data_type_id_t tId = RegisteredCompileTypeId::template from<T>();
    const string_hash nameHash = str_hash(type_name);
    
    const size_t entryId = m_Entries.size();

    m_Entries.push_back(Entry{
      .compileTypeId = tId,
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

  template<class T>
  auto getTypeId() -> engine_data_type_id_t
  {
    return RegisteredCompileTypeId::template from<T>();
  }

private:
  eastl::vector<Entry> m_Entries;
  eastl::vector_map<string_hash, size_t> m_NameHashToEntryId;
  eastl::vector_map<engine_data_type_id_t, size_t> m_CompileTimeIdToEntryId;
};

class TEngineData
{
public:
  struct TypeConstructor
  {
    TypeConstructor(string&& type_name, std::unique_ptr<TEngineData>&& data)
      : typeName(std::move(type_name))
      , data(std::move(data))
    {
    }

    TypeConstructor(string&& type_name)
      : typeName(std::move(type_name))
    {
    }

    string typeName;
    std::unique_ptr<TEngineData> data;
  private:
    friend TEngineData;
    engine_data_type_id_t typeId;
  };

  enum class ValueType : uint8_t
  {
    Int, Int2, Int3, Int4,
    Float, Float2, Float3, Float4,
    Text, TypeConstructor,
  };

  using Value = std::variant<ED_BASE_VALUE_TYPES, TypeConstructor>;

  struct Variable
  {
    string name;
    string annotation;
    Value value;

    auto getValueType() const -> ValueType
    {
      return static_cast<ValueType>(value.index());
    }
  };

public:
  auto addVariable(string&& name, string&& annotation, Value&& value) -> bool
  {
    if (TypeConstructor* tc = std::get_if<TypeConstructor>(&value))
    {
      const CustomTypeRegistry::Entry* typeReg = m_CustomTypesRegistry->get(tc->typeName);
      if (!typeReg)
        return false;
      
      tc->typeId = typeReg->compileTypeId;
    }

    m_Variables.push_back(Variable{
      .name = std::move(name),
      .annotation = std::move(annotation),
      .value = std::move(value)
    });

    return true;
  }

  template<class T, bool verboseFail = true>
  auto getVariableDefinition(string_view name) const
    -> const Variable*
  {
    using VarType = std::remove_cvref_t<T>;

    for (const Variable& v : m_Variables)
    {
      if (v.name == name)
      {
        if constexpr (IsEdVarCustomType<T>)
        {
          const engine_data_type_id_t varTypeId = m_CustomTypesRegistry->getTypeId<VarType>();
          if (const TypeConstructor* tc = std::get_if<TypeConstructor>(&v.value); tc->typeId == varTypeId)
            return &v;
        }
        else
        {
          if (std::holds_alternative<VarType>(v.value))
            return &v;
        }
      }
    }

    return nullptr;
  }

  template<class T>
  auto getVariableOr(string_view name, const T& def) const
    -> std::remove_cvref_t<T>
  {
    using Type = std::remove_cvref_t<T>;
    using VarType = typename std::conditional<std::is_same_v<Type, string_view>, string, Type>::type;

    const Variable* var = getVariableDefinition<T, false>(name);
    if (var)
    {
      if constexpr (IsEdVarCustomType<VarType>)
        return VarType{std::get<TypeConstructor>(var->value).data.get()};
      else
        return std::get<VarType>(var->value);
    }

    return def;
  }

  template<class T>
  auto getVariable(string_view name) const
    -> decltype(getVariableOr<T>(name, {}))
  {
    T def{};
    return getVariableOr(name, def);
  }

  void insertScope(TEngineData&& scope)
  {
    scope.m_CustomTypesRegistry = m_CustomTypesRegistry;
    m_Scopes.push_back(std::move(scope));
  }

  TEngineData() = default;
  TEngineData(const std::shared_ptr<CustomTypeRegistry>& reg)
    : m_CustomTypesRegistry(reg)
  {
  }

private:
  std::shared_ptr<CustomTypeRegistry> m_CustomTypesRegistry;
  eastl::vector<Variable> m_Variables;
  eastl::vector<TEngineData> m_Scopes;
};