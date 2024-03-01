#include "compiler.h"

#include <engine/assert.h>
#include <engine/log.h>

#include <filesystem>
#include <exception>
#include <cstdio>
extern int shparse(ShadersSystem::Compiler& compiler);
extern void shrestart(FILE* f);
extern FILE *shin;

namespace ShadersSystem
{
  Compiler::Compiler(eastl::span<const string> include_dirs)
    : m_IncludeDirs(include_dirs)
  {
    m_WIncludeDirs.reserve(m_IncludeDirs.size());
    for (const string& incDir: include_dirs)
      m_WIncludeDirs.push_back(wstring{incDir.begin(), incDir.end()});
  }

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

  bool Compiler::compileModuleFromFile(const string& file, const int flags)
  {
    m_CompileFlags = flags;

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

  auto Compiler::resolveLocalInclude(const string_view file) -> std::optional<string>
  {
    const string localDir = std::filesystem::path(getCurrentFileName()).parent_path().string();
    const string resolvedPath = fmt::format("{}/{}", localDir, file);

    if (std::filesystem::exists(resolvedPath))
      return string{resolvedPath};

    return resolveSystemInclude(file);
  }

  auto Compiler::resolveSystemInclude(const string_view file) -> std::optional<string>
  {
    for (string_view incDir : m_IncludeDirs)
    {
      const string resolvedPath = fmt::format("{}/{}", incDir, file);
      if (std::filesystem::exists(resolvedPath))
        return resolvedPath;
    }

    return std::nullopt;
  }

  auto Compiler::openIncludeFile(const string_view file, const bool is_local) -> FILE*
  {
    const std::optional<string> resolvedPath = is_local ?
                                                resolveLocalInclude(file) :
                                                resolveSystemInclude(file);
    if (!resolvedPath.has_value())
      throw std::runtime_error(fmt::format("can't include file `{}`: File not found", file));

    FILE* fd = fopen(resolvedPath->c_str(), "r" );
    if (!fd)
      throw std::runtime_error(fmt::format("can't include file `{}`: error:{}, {}",
                               resolvedPath.value(), errno, strerror(errno)));

    pushFile(resolvedPath.value());

    return fd;
  }
}