#include "load_templates.h"

#include <engine/assert.h>
#include <engine/data/utils.h>
#include <engine/ecs/registry.h>
#include <engine/log.h>
#include <engine/types.h>
#include <engine/utils/string.h>

#include <boost/algorithm/string.hpp>

#include <algorithm>
#include <ranges>

namespace ecs
{
  namespace
  {
    struct Annotations
    {
      string name;
      TemplateParentNames extendsTemplates;
    };
  }

  auto get_template_components(const ed::Scope& tmpl) -> TemplateComponentsMap 
  {
    TemplateComponentsMap comps;
    #define ADD_TMPL_COMP(ed_type, _type) \
      case ed::ValueType::ed_type:\
      {\
        const string& name = var.name;\
        comps[name] = std::get<_type>(var.value);\
        break;\
      }

    for(const ed::Variable& var: tmpl.getVariables())
    {
      switch(var.getValueType())
      {
        ADD_TMPL_COMP(Int, int)
        ADD_TMPL_COMP(Int2, int2)
        ADD_TMPL_COMP(Int3, int3)
        ADD_TMPL_COMP(Int4, int4)
        ADD_TMPL_COMP(Float, float)
        ADD_TMPL_COMP(Float2, float2)
        ADD_TMPL_COMP(Float3, float3)
        ADD_TMPL_COMP(Float4, float4)
        ADD_TMPL_COMP(Mat4, mat4)
        ADD_TMPL_COMP(Text, string)
        ADD_TMPL_COMP(Bool, bool)

        case ed::ValueType::TypeConstructor:
        {
          const auto& tc = std::get<ed::TypeConstructor>(var.value);
          comps[var.name] = tc;
          break;
        }

        default:
        {
          logwarn("ecs: template `{}`, ignored component: `{}` ", tmpl.getName(), var.name);
        }
      }
    }

    #undef ADD_TMPL_COMP

    for (const ed::Scope& child: tmpl.getScopes())
    {
      const string_view type = child.getName();
      const string_view name = child.getAnnotation();

      if (type == "entity_id")
      {
        comps[name] = EntityId{};
      }
    }

    return comps;
  }

  static
  auto get_template_annotations(const string_view raw_str) -> Annotations
  {
    Annotations annotations;
    eastl::vector<string> strs;

    string rawStr{raw_str};
    if (!raw_str.empty())
    {
      string_view strs{rawStr.begin(), std::remove(rawStr.begin(), rawStr.end(), ' ')};     

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
            annotations.extendsTemplates.push({extendRange.begin(), extendRange.end()});
        }
      }
    }
    
    return annotations;
  }

  static
  void dump_template(const Annotations& annotations, const TemplateComponentsMap& components)
  {
    string extends = "";
    if (annotations.extendsTemplates.hasAny())
    {
      extends = "extends:[";
      for (const auto& extend: annotations.extendsTemplates)
        extends += extend + " ";
      extends += "]";
    }

    loginfo("adding template {} {}", annotations.name, extends);
  }

  void add_templates_from_ed(Registry& registry, const string_view file)
  {
    std::shared_ptr<ed::CustomTypeRegistry> customTypeRegistry = get_custom_type_registry();

    std::optional<ed::Scope> templates = ed::load_from_file(file, customTypeRegistry);
    ASSERT_FMT_RETURN(templates.has_value(), , "failed to load ecs templates from `{}`", file);

    for(const ed::Scope& tmpl: templates->getScopes())
    {
      if (tmpl.getName() != "template")
        continue;

      string_view rawAnnotations = tmpl.getAnnotation();
      auto annotations = get_template_annotations(rawAnnotations);

      if (annotations.name.empty())
      {
        logerror("template without a name has encountered");
        continue;
      }

      auto components = get_template_components(tmpl);
      if (components.empty())
      {
        logerror("ecs: template {} doesn't have a components", annotations.name);
        continue;
      }

      dump_template(annotations, components);
      registry.addTemplate(annotations.name, std::move(annotations.extendsTemplates), std::move(components));
    }
  }
}
