#pragma once

#include "ast.h"
#include "spirv.h"
#include "bin.h"

#include <engine/algorithm/hash.h>
#include <engine/types.h>

#include <memory>

#include <EASTL/span.h>
#include <EASTL/vector_map.h>
#include <EASTL/vector_set.h>


namespace ShadersSystem
{
  enum CompilationFlags
  {
    COMPILE_DEBUG = 1
  };
  class Compiler
  {
    public:
      Compiler(eastl::span<const string> include_dirs);

      bool compileModuleFromFile(const string& file, const int flags);

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

      void pushFile(const string& file)
      {
        m_FileCtxs.push_back({file, 1 });
      }
      void popFile()
      {
        m_FileCtxs.pop_back();
      }
      void incLine() { ++m_FileCtxs.back().line; }
      auto getLine() const -> unsigned int { return m_FileCtxs.back().line; }
      auto getCurrentFileName() const -> const string& { return m_FileCtxs.back().fileName; }
      auto getFlags() const -> int { return m_CompileFlags; }
      auto resolveLocalInclude(const string_view file) -> std::optional<string>;
      auto resolveSystemInclude(const string_view file) -> std::optional<string>;
      auto openIncludeFile(const string_view file, const bool is_local) -> FILE*;
    private:
      eastl::span<const string> m_IncludeDirs;

      struct FileContext
      {
        string fileName;
        unsigned int line;
      };
      eastl::vector<FileContext> m_FileCtxs;

      int m_CompileFlags = 0;
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
