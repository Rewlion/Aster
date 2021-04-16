#include "code_generator.h"
#include "reflection_templates.h"

#include <boost/algorithm/string/replace.hpp>
#include <boost/format.hpp>
#include <filesystem>
#include <exception>
#include <fstream>

namespace mustache = kainjow::mustache;

namespace
{
  std::string EraseStartingSubstring(const std::string& from, const std::string& begin)
  {
    return  from.substr(begin.length(), from.length() - begin.length());
  }
}

CodeGenerator::CodeGenerator(const std::string& outputFolder)
  : m_OutputFolder(outputFolder)
{
}

void CodeGenerator::GenerateTranslationUnitCode(const ReflectedClass& reflectedClass) const
{
  auto srcHeader = std::filesystem::path(reflectedClass.GetHeaderFile());
  srcHeader.replace_extension(".generated.h");
  auto srcSource = std::filesystem::path(reflectedClass.GetHeaderFile());
  srcSource.replace_extension(".generated.cpp");
 
  const auto generatedHeader = boost::format("%s/%s") 
                         % (m_OutputFolder + "/generated/include/")
                         % srcHeader.filename().string();

  const auto generatedSource = boost::format("%s/%s") 
                         % (m_OutputFolder + "/generated/src")
                         % srcSource.filename().string();

  std::filesystem::create_directories(std::filesystem::path(generatedHeader.str()).parent_path());
  std::filesystem::create_directories(std::filesystem::path(generatedSource.str()).parent_path());

  GenerateHeaderFile(reflectedClass, generatedHeader.str());
  GenerateSourceFile(reflectedClass, generatedSource.str());
}

std::tuple<std::string, std::string> SplitNamespaceAndName(const std::string& className)
{
  const auto pos = className.find_last_of(':');
  if (pos == std::string::npos)
    return { "", className };

  const auto name = className.substr(pos + 1);
  const auto nspace = className.substr(0, pos - 1);

  return { nspace, name };
}

void CodeGenerator::GenerateHeaderFile(const ReflectedClass& reflectedClass, const std::string& header) const
{
  std::ofstream f{ header };

  mustache::mustache classImplUsageDeclarationTmpl{ GetClassImplUsageDeclarationTemplate };
  mustache::data map;
  map.set("CLASS_TYPE", reflectedClass.m_Name);
  const std::string classImplUsageDeclaration = classImplUsageDeclarationTmpl.render(map);


  mustache::mustache headerTmpl{ HeaderTemplate };
  map = mustache::data{};
  map.set("CLASS_IMPL_DECLARATION_LIST", classImplUsageDeclaration);
  const auto [nspace, name] = SplitNamespaceAndName(reflectedClass.m_Name);
  map.set("CLASS_NAME", name);
  map.set("CLASS_NAMESPACE", nspace);
  map.set("HAS_NAMESPACE", !nspace.empty());
  std::string headerCode = headerTmpl.render(map);

  f.write(headerCode.c_str(), headerCode.size());
}

void CodeGenerator::GenerateSourceFile(const ReflectedClass& reflectedClass, const std::string& source) const
{
  std::ofstream f{ source };

  std::string attributes;
  for (int i = 0; i < reflectedClass.m_FieldDecls.size(); ++i)
  {
    mustache::mustache attributeTmpl{ ClassAttributeTemplate };
    mustache::data map;

    map.set("ATTRIBUTE_INDEX", std::to_string(i));

    const ReflectedClass::Field& f = reflectedClass.m_FieldDecls[i];

    map.set("ATTRIBUTE_TYPE_NAME", f.decl->getType().getAsString());
    map.set("CLASS_TYPE", reflectedClass.m_Name);
    map.set("CLASS_ATTRIBUTE_NAME", f.decl->getIdentifier()->getName().str());

    attributes += attributeTmpl.render(map) + "\n";
  }

  mustache::mustache getClassImplImplementationTmpl{ GetClassImplImplementationTemplate };
  mustache::data map;
  map.set("CLASS_TYPE", reflectedClass.m_Name);
  map.set("CLASS_ATTRIBUTES_NUMBER", std::to_string(reflectedClass.m_FieldDecls.size()));
  map.set("CLASS_ATTRIBUTES_LIST", attributes);
  std::string classHeader = reflectedClass.m_HeaderFile; boost::replace_all(classHeader, "\\", "\\\\");
  map.set("CLASS_HEADER", classHeader);
  std::string getClassImplImplementation = getClassImplImplementationTmpl.render(map);

  mustache::mustache classImplUsageTemplate{ ClassImplUsageTemplate };
  map = mustache::data{};
  map.set("CLASS_TYPE", reflectedClass.m_Name);
  std::string classImplUsage = classImplUsageTemplate.render(map);

  mustache::mustache sourceTmpl{ SourceTemplate };
  map = mustache::data{};
  map.set("CLASS_IMPL_USAGE_LIST", classImplUsage);
  map.set("CLASS_HEADER", classHeader);
  map.set("CLASS_IMPL_IMPLEMENTATION_LIST", getClassImplImplementation);
  std::string sourceCode = sourceTmpl.render(map);

  f.write(sourceCode.c_str(), sourceCode.size());
}