#include "ed.h"

namespace ed
{
  void Scope::addScope(Scope&& scope)
  {
    scope.m_CustomTypesRegistry = m_CustomTypesRegistry;
    m_Scopes.push_back(std::move(scope));
  }

  auto Scope::addVariable(string&& name, string&& annotation,
                          string_view custom_type_name, std::unique_ptr<Scope>&& data) -> bool
  {
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

  Scope::Scope(string&& name, string&& annotation, const std::shared_ptr<CustomTypeRegistry>& reg = nullptr)
    : name(std::move(name))
    , annotation(std::move(annotation))
    , m_CustomTypesRegistry(reg)
  {
  }
}
