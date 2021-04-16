#include "annotation_parser.h"

#include "clang/ASTMatchers/ASTMatchers.h"

namespace
{
  constexpr bool EndingIsExpected = true;

  bool IsChar(const char c)
  {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
  }

  bool IsDigit(const char c)
  {
    return c >= '0' && c <= '9';
  }

  bool IsDigitOrChar(const char c)
  {
    return IsChar(c) || IsDigit(c);
  }
}

std::vector<AnnotationDescription> AnnotationParser::Parse(const clang::Attr* attrNode)
{
  m_Annotations.clear();
  PrepareBuffer(attrNode);

  if (!m_Buffer.empty())
    ROOT();

  return m_Annotations;
}

void AnnotationParser::PrepareBuffer(const clang::Attr* attrNode)
{
  llvm::raw_string_ostream ss(m_FullAnnotation);
  attrNode->printPretty(ss, clang::PrintingPolicy{ clang::LangOptions{} });

  const std::string_view attrView{ m_FullAnnotation };

  const char* attributePrefix = " [[clang::annotate(\"__has_reflection;";
  if (false == attrView.starts_with(attributePrefix))
    throw std::runtime_error(
      std::string{ "Error: attribute annotation:" } + m_FullAnnotation + " has invalid start"
    );

  const size_t annotationListBegin = attrView.find(';') + 1;
  const size_t annotationListLen = (attrView.length() - annotationListBegin) - 4;

  m_Buffer = attrView.substr(annotationListBegin, annotationListLen);
  m_BufIndex = 0;
}

void AnnotationParser::ROOT()
{
  ANNOTATION_LIST();

  if (IsEndOfBuffer() == false)
    throw std::runtime_error("Error: unexpected ending");
}

void AnnotationParser::ANNOTATION_LIST()
{
  do
  {
    m_Annotations.push_back(ANNOTATION());
  } while (CheckNextCharIs(','));
}

AnnotationDescription AnnotationParser::ANNOTATION()
{
  AnnotationDescription ad;

  ad.name = std::string(NAME());

  if (CheckNextCharIs('='))
  {
    InsureNextCharIs('"');
    ad.value = std::string(VALUE());
    InsureNextCharIs('"');
  }
  else
    --m_BufIndex;

  return ad;
}

std::string_view AnnotationParser::NAME()
{
  return GetSubstringByPattern(IsChar);
}

std::string_view AnnotationParser::VALUE()
{
  return GetSubstringByPattern(IsDigitOrChar);
}

void AnnotationParser::InsureNextCharIs(const char c)
{
  SkipSpaces();

  if (m_Buffer[m_BufIndex] != c)
    throw std::runtime_error(
      std::string("Error: expected char:") + c + " at position:" + std::to_string(m_BufIndex) + " (" + m_Buffer[m_BufIndex] + ")"
    );

  ++m_BufIndex;
}

bool AnnotationParser::CheckNextCharIs(const char c)
{
  SkipSpaces();

  if (IsEndOfBuffer())
    return false;
  else
    return m_Buffer[m_BufIndex++] == c;
}

std::string_view AnnotationParser::GetSubstringByPattern(CharPattern pattern)
{
  SkipSpaces();

  const size_t begin = m_BufIndex;

  if (!pattern(m_Buffer[m_BufIndex]))
    throw std::runtime_error(
      std::string("Error: unexpected char at position:" + std::to_string(m_BufIndex) + " (" + m_Buffer[m_BufIndex] + ")")
    );

  while (!IsEndOfBuffer() && (pattern(m_Buffer[m_BufIndex])))
    ++m_BufIndex;

  const size_t end = m_BufIndex;

  return m_Buffer.substr(begin, end - begin);
}

void AnnotationParser::SkipSpaces()
{
  while (!IsEndOfBuffer() && m_Buffer[m_BufIndex] == ' ')
    ++m_BufIndex;
}

bool AnnotationParser::IsEndOfBuffer() const
{
  return m_BufIndex == m_Buffer.length();
}
