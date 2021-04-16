#include "reflection_header_tool.h"
#include "reflection_matcher.h"

#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/CompilationDatabase.h"
#include "llvm/Support/CommandLine.h"

using namespace clang::tooling;

int ReflectionHeaderTool::Parse(const Options& opts)
{
  m_Options = opts;

  int argc = m_Options.clangToolArgv.size();
  std::string errMsg;
  std::unique_ptr<FixedCompilationDatabase> cdb{ FixedCompilationDatabase::loadFromCommandLine(argc, m_Options.clangToolArgv.data(), errMsg) };

  ClangTool Tool(*cdb, m_Options.filesToParse);

  for (const std::string& inc : m_Options.includePaths)
  {
    const auto opt = std::string("-I") + inc;
    Tool.appendArgumentsAdjuster(getInsertArgumentAdjuster(opt.c_str(), ArgumentInsertPosition::BEGIN));
  }

  clang::ast_matchers::MatchFinder finder;
  CodeGenerator codeGen{m_Options.outputFolder };
  ReflectionMatcher matcher{ codeGen };

  const auto classMatcher = clang::ast_matchers::cxxRecordDecl(
    clang::ast_matchers::hasAttr(
      clang::attr::Annotate
    )
  ).bind(ReflectionBindId);

  const auto fieldMatcher = clang::ast_matchers::fieldDecl(
    clang::ast_matchers::hasAttr(
      clang::attr::Annotate
    )
  ).bind(ReflectionBindId);

  finder.addMatcher(classMatcher, &matcher);
  finder.addMatcher(fieldMatcher, &matcher);

  return Tool.run(newFrontendActionFactory(&finder).get());
}
