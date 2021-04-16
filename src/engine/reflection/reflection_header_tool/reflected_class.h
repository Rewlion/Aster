#pragma once

#include "annotation_parser.h"

#include "clang/ASTMatchers/ASTMatchers.h"

#include "mustache.hpp"
#include <vector>
#include <cstring>

class ReflectedClass
{
  friend class CodeGenerator;

  struct Field
  {
    const clang::FieldDecl* decl;
    std::vector<AnnotationDescription> annotations;
  };

public:
  ReflectedClass(const clang::CXXRecordDecl* decl, const std::string& headerFile);

  const ReflectedClass& operator=(const ReflectedClass& rvl);

  void AddField(const clang::FieldDecl* decl);

private:
 const clang::CXXRecordDecl* m_RecordDecl;
 std::string m_HeaderFile;
 std::string m_Name;

  std::vector<AnnotationDescription> m_ClassAnnotations;
  std::vector<Field> m_FieldDecls;
};
