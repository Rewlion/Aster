#pragma once

#include "ast.h"
#include "spirv.h"
#include "bin.h"

#include <engine/algorithm/hash.h>
#include <engine/types.h>

#include <memory>

#include <EASTL/vector_map.h>
#include <EASTL/vector_set.h>

namespace ShadersSystem
{
  class Compiler
  {
    public:
      bool compileModuleFromFile(const string& file);

      inline void markCompilationFailed() { m_IsCompilationOk = false; }
      bool onScopeDeclaration(ScopeDeclarationExp*);
      bool onTechniqueMacroDeclaration(TechniqueMacroDeclarationExp*);
      bool onTechniqueDeclaration(TechniqueDeclarationExp*);

      bool hasScope(const string& scope) const;
      string getScopeHlsl(const string& scope) const;
      const ScopeDeclaration& getScope(const string& scope);
      const TechniqueMacroDeclarationExp& getTechniqueMacroDeclaration(const string& macroName) const;

      inline const MaterialsBin& getMaterialsBins() const
      {
        return m_Bin;
      }

      inline const string& getCurrentCompilationFile() const
      {
        return m_CurrentCompilationFile;
      }

    private:
      string m_CurrentCompilationFile;
      bool m_IsCompilationOk = true;

      MaterialsBin m_Bin;

      struct ModuleResources
      {
        eastl::vector_set<string_hash> declaredScopes;
        eastl::vector_set<string_hash> declaredTechniques;
        eastl::vector_map<string_hash, TechniqueMacroDeclarationExp*> macros;
      };
      ModuleResources m_Module;

      eastl::vector_map<string_hash, ScopeDeclaration> m_DeclaredScopes;
      eastl::vector_map<string_hash, TechniqueDeclaration> m_DeclaredTechniques;

      eastl::vector<std::unique_ptr<Node>> m_GC;
  };
}
