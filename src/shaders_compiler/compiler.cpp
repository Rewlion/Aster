#include "compiler.h"

#include <engine/assert.h>
#include <engine/log.h>

#include <exception>
#include <cstdio>
extern int shparse(ShadersSystem::Compiler& compiler);
extern void shrestart(FILE* f);
extern FILE *shin;

namespace ShadersSystem
{
  string Compiler::getScopeHlsl(const string& scope) const
  {
    return m_DeclaredScopes.find(str_hash(scope.c_str()))->second.hlslResourceDeclaration;
  }

  bool Compiler::hasScope(const string& scope) const
  {
    return m_DeclaredScopes.find(str_hash(scope.c_str())) != m_DeclaredScopes.end();
  }

  const ScopeDeclaration& Compiler::getScope(const string& scope)
  {
    return m_DeclaredScopes.find(str_hash(scope.c_str()))->second;
  }

  const TechniqueMacroDeclarationExp& Compiler::getTechniqueMacroDeclaration(const string& macroName) const
  {
    const string_hash nameHash = str_hash(macroName.c_str());
    const auto it = m_Module.macros.find(nameHash);
    if (it != m_Module.macros.end())
      return *it->second;
    else
      throw std::runtime_error(fmt::format("technique macro `{}` is not declared", macroName));
  }

  bool Compiler::compileModuleFromFile(const string& file)
  {
    FILE* f = fopen(file.c_str(), "r");

    if (!f)
    {
        logerror("failed to compile shader module `{}`: can.t open a file", file);
        return false;
    }

    m_IsCompilationOk = true;
    pushFile(file);
    m_Module = ModuleResources{};

    shin = f;
    shrestart(f);
    shparse(*this);
    fclose(f);

    return m_IsCompilationOk;
  }
}