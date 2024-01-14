#pragma once

#include <engine/data/ed.h>

namespace ed
{
  class CustomTypeRegistry;

  class Parser
  {
  public:
    Parser() = default;
    Parser(std::shared_ptr<CustomTypeRegistry> reg)
      : m_CustomTypeRegistry(reg)
    {
    }

    Scope parseFile(string_view path, const bool accept_fails = false);

    auto makeScope() -> ed::Scope
    {
      auto scope = ed::Scope{};
      scope.m_CustomTypesRegistry = m_CustomTypeRegistry;

      return scope;
    }

    auto buildTypeConstructor(string_view type_name, ed::Scope&& scope) -> TypeConstructor
    {
      TypeConstructor tc;

      if (m_CustomTypeRegistry)
      {
        const CustomTypeRegistry::Entry* typeReg = m_CustomTypeRegistry->get(type_name);

        if (typeReg)
        {
          tc.typeId = typeReg->compileTypeId;
          tc.typeName = type_name;
          tc.data = std::unique_ptr<ed::Scope>(new ed::Scope{std::move(scope)});
        }
      }

      return tc;
    }

    void addVariableDefinition(ed::Variable&& var, ed::Scope& to)
    {
      to.m_Variables.push_back(std::move(var));
    }

    void setMainScope(Scope&& scope)
    {
      m_MainScope = std::move(scope);
    }
    
    void pushFile(string_view file, eastl::vector<char>&& buffer)
    {
      m_FileCtxs.push_back({file, 1, std::move(buffer) });
    }
    auto getCurrentBuffer() -> eastl::vector<char>&
    {
      return m_FileCtxs.back().buffer;
    }
    void popFile()
    {
      m_FileCtxs.pop_back();
    }
    void incLine()
    {
      ++m_FileCtxs.back().line;
    }
    auto getLine() const -> unsigned int
    {
      return m_FileCtxs.back().line;
    }
    auto getCurrentFileName() const -> string_view
    { 
      return m_FileCtxs.back().fileName;
    }

    void markParsingFailed(const string& error);
    auto isParsingOk() const -> bool
    {
      return !m_ParsingFailed;
    }

    void setScopeName(Scope& scope, string&& name)
    {
      scope.m_Name = std::move(name);
    }
    void setScopeAnnotation(Scope& scope, string&& annotation)
    {
      scope.m_Annotation = annotation;
    }

    auto getErrors() const -> string_view 
    {
      return m_Errors;
    }

  private:
    struct FileContext
    {
      string_view fileName;
      unsigned int line;
      eastl::vector<char> buffer;
    };

  private:
    std::shared_ptr<CustomTypeRegistry> m_CustomTypeRegistry;
    bool m_ParsingFailed = false;
    Scope m_MainScope;
    eastl::vector<FileContext> m_FileCtxs;
    string m_Errors;
  };

}
