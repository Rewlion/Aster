#include "reflected_class.h"

ReflectedClass::ReflectedClass(const clang::CXXRecordDecl* decl, const std::string& headerFile)
  : m_RecordDecl(decl)
  , m_HeaderFile(headerFile)
  , m_Name(decl->getQualifiedNameAsString())
{
  for (auto& attr : decl->getAttrs())
  {
    if (attr->getKind() == clang::attr::Annotate)
    {
      m_ClassAnnotations = AnnotationParser().Parse(attr);
      break;
    }
  }
}

const ReflectedClass& ReflectedClass::operator=(const ReflectedClass& rvl)
{
  m_RecordDecl = rvl.m_RecordDecl;
  m_HeaderFile = rvl.m_HeaderFile;
  m_Name = rvl.m_Name;
  m_ClassAnnotations = rvl.m_ClassAnnotations;
  m_FieldDecls = rvl.m_FieldDecls;

  return *this;
}

void ReflectedClass::AddField(const clang::FieldDecl* decl)
{
  Field f;
  f.decl = decl;

  for (auto& attr : decl->getAttrs())
  {
    if (attr->getKind() == clang::attr::Annotate)
    {
      f.annotations = AnnotationParser().Parse(attr);
      break;
    }
  }

  m_FieldDecls.emplace_back(f);
}
