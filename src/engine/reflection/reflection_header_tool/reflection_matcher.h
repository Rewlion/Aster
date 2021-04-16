#pragma once

#include "reflected_class.h"
#include "code_generator.h"

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

#include <optional>

constexpr const char* const ReflectionBindId = "reflected-id";

class ReflectionMatcher : public clang::ast_matchers::MatchFinder::MatchCallback {
public:
  ReflectionMatcher(const CodeGenerator& codeGen);

  virtual void run(const clang::ast_matchers::MatchFinder::MatchResult& Result) override;

  virtual void onStartOfTranslationUnit() override;

  virtual void onEndOfTranslationUnit() override;

private:
  CodeGenerator m_CodeGen;

  bool m_IsParsingClass = false;
  std::optional<ReflectedClass> m_ReflectedClass;
};
