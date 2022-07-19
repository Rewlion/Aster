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

  bool Compiler::compileModuleFromFile(const string& file)
  {
    FILE* f = fopen(file.c_str(), "r");

    if (!f)
    {
        fprintf(stderr, "can.t open a file");
        return false;
    }

    m_IsCompilationOk = true;
    m_CurrentCompilationFile = file;
    m_Module = ModuleResources{};

    shin = f;
    shrestart(f);
    shparse(*this);
    fclose(f);

    return m_IsCompilationOk;
  }
}