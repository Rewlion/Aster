#include "template_utils.h"
#include "ecs.h"

#include <engine/utils/string.h>

namespace ecs
{
  auto add_sub_template(const EntityId eid, string_view tmpl) -> EntityId
  {
    auto& ecs = get_registry();

    string_view currentTmpl = ecs.getEntityTemplateName(eid);
    if (currentTmpl == "")
      return eid;

    bool hasThisSubTmpl = Utils::find_view_substr(currentTmpl, ":",
      [&tmpl](const string_view view){ return view == tmpl; });

    if (!hasThisSubTmpl)
    {
      string newTmplName = fmt::format("{}:{}", currentTmpl, tmpl);
      
      TemplateParentNames parents;
      Utils::for_each_view_substr(newTmplName, ":",
        [&parents](const string_view t){
          parents.push(string{t});
          return true;
      });

      ecs.addTemplate(newTmplName, std::move(parents), {});
      return ecs.recreateEntity(eid, newTmplName, {});
    }

    return eid;
  }

  auto remove_sub_template(const EntityId eid, string_view tmpl_to_remove) -> EntityId
  {
    auto& ecs = get_registry();

    string_view currentTmpl = ecs.getEntityTemplateName(eid);
    if (currentTmpl == "")
      return eid;

    bool hasThisSubTmpl = Utils::find_view_substr(currentTmpl, ":",
      [&tmpl_to_remove](const string_view view){ return view == tmpl_to_remove; });

    if (hasThisSubTmpl)
    {
      string newTmplName;
      TemplateParentNames parents;

      Utils::for_each_view_substr(currentTmpl, ":",
        [&tmpl_to_remove, &newTmplName, &parents](const string_view t){
          if (t != tmpl_to_remove)
          {
            parents.push(string{t});
            if (newTmplName.empty())
              newTmplName = t;
            else
              newTmplName = fmt::format("{}:{}", newTmplName, t);
          }
          return true;
      });

      if (parents.getSize() == 1)
        parents.clear();

      ecs.addTemplate(newTmplName, std::move(parents), {});
      return ecs.recreateEntity(eid, newTmplName, {});
    }

    return eid;
  }
}
