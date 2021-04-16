#pragma once

#include <vector>
#include <string_view>
#include <string>

typedef bool (*CharPattern)(const char);

namespace clang
{
  class Attr;
}

struct AnnotationDescription
{
  std::string name;
  std::string value;
};

class AnnotationParser
{
  //  ROOT: ANNOTATION_LIST
  //  ANNOTATION_LIST: ANNOTATION , ANNOTATION_LIST
  //                 : ANNOTATION
  //  ANNOTATION: NAME = " VALUE "
  //            : NAME
  //  NAME: [a-zA-Z]*
  //  VALUE: [0-9a-zA-Z ]*

public:
  std::vector<AnnotationDescription> Parse(const clang::Attr* attrNode);

private:
  void PrepareBuffer(const clang::Attr* attrNode);

  void ROOT();

  void ANNOTATION_LIST();

  AnnotationDescription ANNOTATION();

  std::string_view NAME();

  std::string_view VALUE();

  void InsureNextCharIs(const char c);

  bool CheckNextCharIs(const char c);

  std::string_view GetSubstringByPattern(CharPattern pattern);

  void SkipSpaces();

  bool IsEndOfBuffer() const;

private:
  std::string m_FullAnnotation;
  std::string_view m_Buffer;
  size_t m_BufIndex;

  std::vector<AnnotationDescription> m_Annotations;
};