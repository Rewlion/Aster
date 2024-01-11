#include "ed.h"

#include <engine/utils/ranges.hpp>

namespace ed
{
  void Scope::addScope(Scope&& scope)
  {
    if (isEmpty())
      return;

    scope.m_CustomTypesRegistry = m_CustomTypesRegistry;
    m_Scopes.push_back(std::move(scope));
  }

  auto Scope::isEmpty() const -> bool
  {
    return this == &EMPTY_SCOPE;
  }
    
  auto Scope::getScope(string_view path) -> Scope&
  {
    Scope* sc = this;
    for (string_view scopeName : Utils::enumerate_substrings(path, "/"))
    {
      if (sc == &EMPTY_SCOPE)
        break;

      Scope* requiredSubScope = &EMPTY_SCOPE;
      for (Scope& childScope: sc->m_Scopes)
      {
        if (childScope.getName() == scopeName)
        {
          requiredSubScope = &childScope;
          break;
        }
      }

      sc = requiredSubScope;
    }

    return *sc;
  }

  auto Scope::getScope(string_view path) const -> const Scope&
  {
    return const_cast<Scope*>(this)->getScope(path);
  }

  auto Scope::addVariable(string&& name, string&& annotation,
                          string_view custom_type_name, std::unique_ptr<Scope>&& data) -> bool
  {
    if (isEmpty())
      return false;

    const CustomTypeRegistry::Entry* typeReg = m_CustomTypesRegistry->get(custom_type_name);
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

  Scope::Scope(const std::shared_ptr<CustomTypeRegistry>& reg)
    : m_CustomTypesRegistry(reg)
  {
  }

  Scope::Scope(string&& name, string&& annotation, const std::shared_ptr<CustomTypeRegistry>& reg)
    : m_Name(std::move(name))
    , m_Annotation(std::move(annotation))
    , m_CustomTypesRegistry(reg)
  {
  }
}
