#include "load_templates.h"

#include <engine/datablock/datablock.h>
#include <engine/datablock/utils.h>
#include <engine/ecs/registry.h>
#include <engine/log.h>
#include <engine/types.h>

#include <boost/algorithm/string.hpp>

#include <algorithm>
#include <ranges>

namespace Engine::ECS
{
  namespace
  {
    struct Annotations
    {
      string name;
      eastl::vector<string> extendsTemplates;
    };
  }

  eastl::vector<ComponentDescription> get_template_components(const DataBlock& tmpl)
  {
    eastl::vector<ComponentDescription> templateDesc;

    for(auto& attr: tmpl.getAttributes())
    {
      switch(attr.type)
      {
        case DataBlock::ValueType::Int:
        {
          const auto comp = DESCRIBE_COMPONENT(attr.name.c_str(), int);
          templateDesc.push_back(comp);
          break;
        }
        case DataBlock::ValueType::Int2:
        {
          templateDesc.push_back(DESCRIBE_COMPONENT(attr.name.c_str(), int2));
          break;
        }
        case DataBlock::ValueType::Int3:
        {
          templateDesc.push_back(DESCRIBE_COMPONENT(attr.name.c_str(), int3));
          break;
        }
        case DataBlock::ValueType::Int4:
        {
          templateDesc.push_back(DESCRIBE_COMPONENT(attr.name.c_str(), int4));
          break;
        }
        case DataBlock::ValueType::Float:
        {
          templateDesc.push_back(DESCRIBE_COMPONENT(attr.name.c_str(), float));
          break;
        }
        case DataBlock::ValueType::Float2:
        {
          templateDesc.push_back(DESCRIBE_COMPONENT(attr.name.c_str(), float2));
          break;
        }
        case DataBlock::ValueType::Float3:
        {
          templateDesc.push_back(DESCRIBE_COMPONENT(attr.name.c_str(), float3));
          break;
        }
        case DataBlock::ValueType::Float4:
        {
          templateDesc.push_back(DESCRIBE_COMPONENT(attr.name.c_str(), float4));
          break;
        }
        case DataBlock::ValueType::Text:
        {
          templateDesc.push_back(DESCRIBE_COMPONENT(attr.name.c_str(), string));
          break;
        }
        case DataBlock::ValueType::Bool:
        {
          templateDesc.push_back(DESCRIBE_COMPONENT(attr.name.c_str(), bool));
          break;
        }
        default:
          logwarn("manager: template {}: can't add template's attribute {}: unknown type {}",
            tmpl.getName(), attr.name, attr.type, attr.getTypeStr());
      }
    }

    return templateDesc;
  }

  static Annotations get_template_annotations(string& raw_str)
  {
    Annotations annotations;
    eastl::vector<string> strs;

    if (!raw_str.empty())
    {
      string_view strs{raw_str.begin(), std::remove(raw_str.begin(), raw_str.end(), ' ')};     

      for (const auto& strRange: std::views::split(strs, ';'))
      {
        string_view str{strRange.begin(), strRange.end()};

        if (str.starts_with("name:"))
        {
          const size_t delimiter = sizeof("name:")-1;
          annotations.name = str.substr(delimiter, str.length() - delimiter);
        }
        else if (str.starts_with("extends:"))
        {
          const size_t delimiter = sizeof("extends:")-1;
          string_view extends = str.substr(delimiter, str.length() - delimiter);
          for (const auto& extendRange: std::views::split(extends, ','))
            annotations.extendsTemplates.push_back({extendRange.begin(), extendRange.end()});
        }
      }
    }
    
    return annotations;
  }

  static void dump_template(const Annotations& annotations, const eastl::vector<ComponentDescription>& components)
  {
    string extends = "";
    if (!annotations.extendsTemplates.empty())
    {
      extends = "extends:[";
      for (const auto& extend: annotations.extendsTemplates)
        extends += extend + " ";
      extends += "]";
    }

    string comps;
    for(auto& comp: components)
    {
      comps = comps + "component: " + comp.name + "\n";
    }
    loginfo("adding template {}, {}\n{}", annotations.name, extends, comps);
  }

  void add_templates_from_blk(Registry& manager, const string& blkPath)
  {
    DataBlock blk;
    if (!load_blk_from_file(&blk, blkPath.c_str()))
    {
      logerror("failed to load templates from {}", blkPath);
      return;
    }

    for(const auto& tmpl: blk.getChildBlocks())
    {
      if (tmpl.getName() != "template")
        continue;

      string rawAnnotations = tmpl.getAnnotation();
      auto annotations = get_template_annotations(rawAnnotations);

      if (annotations.name.find('.') != annotations.name.npos)
      {
        logerror("template has invalid name `{}`,  `.` is not allowed for template names", annotations.name);
        continue;
      }

      if (annotations.name.empty())
      {
        logerror("template without a name has encountered");
        continue;
      }

      auto components = get_template_components(tmpl);
      dump_template(annotations, components);
      manager.addTemplate(annotations.name, std::move(components), annotations.extendsTemplates);
    }
  }
}
