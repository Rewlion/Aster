#pragma once

#include "reflected_class.h"

#include <vector>

class CodeGenerator
{
public:
  CodeGenerator(const std::string& outputFolder);

  void GenerateTranslationUnitCode(const ReflectedClass& reflectedClass) const;

private:
  void GenerateHeaderFile(const ReflectedClass& reflectedClass, const std::string& header) const;
  void GenerateSourceFile(const ReflectedClass& reflectedClass, const std::string& source) const;

private:
  std::string m_RootFolder;
  std::string m_OutputFolder;
};
