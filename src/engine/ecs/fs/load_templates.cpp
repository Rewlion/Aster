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

  eastl::vector<TemplateComponentDescription> get_template_components(const DataBlock& tmpl)
  {
    eastl::vector<TemplateComponentDescription> comps;
    #define ADD_TMPL_COMP(blk_type, _type) \
      case DataBlock::ValueType::blk_type:\
      {\
        comps.push_back({.name = attr.name.c_str(), .type = #_type});\
        break;\
      }

    for(auto& attr: tmpl.getAttributes())
    {
      switch(attr.type)
      {
        ADD_TMPL_COMP(Int, int)
        ADD_TMPL_COMP(Int2, int2)
        ADD_TMPL_COMP(Int3, int3)
        ADD_TMPL_COMP(Int4, int4)
        ADD_TMPL_COMP(Float, float)
        ADD_TMPL_COMP(Float2, float2)
        ADD_TMPL_COMP(Float3, float3)
        ADD_TMPL_COMP(Float4, float4)
        ADD_TMPL_COMP(Text, string)
        ADD_TMPL_COMP(Bool, bool)
        default:
          logwarn("ecs: template {}: can't add template's attribute {}: unknown type {}",
            tmpl.getName(), attr.name, attr.type, attr.getTypeStr());
      }
    }

    #undef ADD_TMPL_COMP

    return comps;
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

  static void dump_template(const Annotations& annotations, const eastl::vector<TemplateComponentDescription>& components)
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
      string desc = fmt::format("\n  component: {} {}", comp.type, comp.name);
      comps = comps + desc;
    }
    loginfo("adding template {}, {} {}", annotations.name, extends, comps);
  }

  void add_templates_from_blk(Registry& registry, const string& blkPath)
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
      registry.addTemplate(annotations.name, std::move(components), annotations.extendsTemplates);
    }
  }
}
