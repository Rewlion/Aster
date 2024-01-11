#pragma once

#include "custom_type_registry.h"

#include <engine/types.h>

#include <memory>
#include <variant>

#include <EASTL/vector.h>


//Engine Data
namespace ed
{
  struct TypeConstructor
  {
    engine_data_type_id_t typeId;
    string typeName;
    std::unique_ptr<Scope> data;
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

  class Scope
  {
  public:
    Scope() = default;
    Scope(Scope&&) = default;
    Scope(const std::shared_ptr<CustomTypeRegistry>& reg);
    Scope(string&& name, string&& annotation = "",
          const std::shared_ptr<CustomTypeRegistry>& reg = nullptr);

    Scope& operator=(Scope&&) = default;

    auto isEmpty() const -> bool;

    void addScope(Scope&& scope);
    auto getScope(string_view path) -> Scope&;
    auto getScope(string_view path) const -> const Scope&;

    auto getName() const -> string_view { return m_Name; }
    auto getAnnotation() const -> string_view { return m_Annotation; }

    template<class T>
    requires Utils::IsAnyConvertibleTo<T, ED_BASE_VALUE_TYPES>
    auto addVariable(string&& name, string&& annotation, T&& value) -> bool;

    auto addVariable(string&& name, string&& annotation,
                     string_view custom_type_name, std::unique_ptr<Scope>&& data) -> bool;

    template<class T>
    auto addVariable(string&& name, string&& annotation, std::unique_ptr<Scope>&& data) -> bool;

    template<class T, bool verboseFail = true>
    auto getVariableDefinition(string_view name) const -> const Variable*;

    template<class T>
    auto getVariableOr(string_view name, const T& def) const -> std::remove_cvref_t<T>;

    template<class T>
    auto getVariable(string_view name) const
      -> decltype(getVariableOr<T>(name, {}))
    {
      T def{};
      return getVariableOr(name, def);
    }

    auto getVariables() const -> const eastl::vector<Variable>&
    {
      return m_Variables;
    }

    auto getScopes() const -> const eastl::vector<Scope>&
    {
      return m_Scopes;
    }

  private:
    string m_Name;
    string m_Annotation;
    std::shared_ptr<CustomTypeRegistry> m_CustomTypesRegistry;

    eastl::vector<Variable> m_Variables;
    eastl::vector<Scope> m_Scopes;
  };

  template<class T>
  requires Utils::IsAnyConvertibleTo<T, ED_BASE_VALUE_TYPES>
  auto Scope::addVariable(string&& name, string&& annotation, T&& value) -> bool
  {
    if (isEmpty())
      return false;

    m_Variables.push_back(Variable{
      .name = std::move(name),
      .annotation = std::move(annotation),
      .value = std::move(value)
    });

    return true;
  }

  template<class T>
  auto Scope::addVariable(string&& name, string&& annotation, std::unique_ptr<Scope>&& data) -> bool
  {
    if (isEmpty())
      return false;

    const CustomTypeRegistry::Entry* typeReg = m_CustomTypesRegistry->get<T>();
    if (!typeReg)
      return false;
    
    m_Variables.push_back(Variable{
      .name = std::move(name),
      .annotation = std::move(annotation),
      .value = TypeConstructor{
        .typeId = typeReg->compileTypeId,
        .typeName = typeReg->name,
        .data = std::move(data)
      }
    });

    return true;
  }

  template<class T, bool verboseFail>
  auto Scope::getVariableDefinition(string_view name) const -> const Variable*
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
  auto Scope::getVariableOr(string_view name, const T& def) const -> std::remove_cvref_t<T>
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

  static Scope EMPTY_SCOPE;
}