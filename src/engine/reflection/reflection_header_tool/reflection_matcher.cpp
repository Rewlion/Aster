#include "reflection_matcher.h"

ReflectionMatcher::ReflectionMatcher(const CodeGenerator& codeGen)
  : m_CodeGen(codeGen)
{
}

void ReflectionMatcher::run(const clang::ast_matchers::MatchFinder::MatchResult& Result)
{
  if (const auto* n = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(ReflectionBindId))
  {
    if (!m_ReflectedClass)
    {
      llvm::StringRef header = Result.SourceManager->getFilename(n->getLocation());
      m_ReflectedClass = ReflectedClass{ n, header.data() };
      m_IsParsingClass = true;
    }
    else
    {
      std::printf("Warning: multiple reflected classes inside one header are not supported. %s will be ignored", n->getQualifiedNameAsString().c_str());
      m_IsParsingClass = false;
    }
  }

  if (m_IsParsingClass)
  {
    if (const auto* n = Result.Nodes.getNodeAs<clang::FieldDecl>(ReflectionBindId))
    {
      m_ReflectedClass->AddField(n);
    }
  }
}

void ReflectionMatcher::onStartOfTranslationUnit()
{
  m_ReflectedClass = std::nullopt;
  m_IsParsingClass = false;
}

void ReflectionMatcher::onEndOfTranslationUnit()
{
  if (m_ReflectedClass.has_value())
    m_CodeGen.GenerateTranslationUnitCode(*m_ReflectedClass);
}