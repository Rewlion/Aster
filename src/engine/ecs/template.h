#pragma once

#include "core_types.h"
#include "hash.h"
#include "template_component.h"
#include "registered_component.h"
#include "entity_component_init.h"
#include "archetype.h"

#include <engine/utils/fixed_stack.hpp>
#include <engine/platform/memory.h>

#include <EASTL/functional.h>
#include <EASTL/unique_ptr.h>

namespace ecs
{
  using TemplateParentNames = Utils::FixedStack<string, 10>;

  struct TemplateDescription
  {
    TemplateDescription() = default;
    TemplateDescription(const string& name,
                        TemplateParentNames&& parents,
                        TemplateComponentsMap&& comps_map);

    template_id_t tmplId;
    string name;
    TemplateParentNames parents;
    TemplateComponentsMap compsMap;
  };

  struct Template
  {
    eastl::unique_ptr<std::byte> initBuffer;
    archetype_id_t archetypeId;
    eastl::vector_map<registered_component_id_t, const TemplateComponent*> regCompToTmplComp;
  };

  class Templates
  {
    public:
      auto getDescription(const name_hash_t name) -> TemplateDescription*;
      auto getDescription(const string_view name) -> TemplateDescription*;

      auto addTemplateDesc(TemplateDescription&& desc,
                           RegisteredComponents& registered_comps) -> bool;

      auto hasTemplateDescription(string_view name) const -> bool;

      auto getTemplate(const template_id_t id) -> const Template&;

      auto requestTemplate(const string_view name,
                           Archetypes& archetypes,
                           const RegisteredComponents& registered_comps) -> template_id_t;

      auto hasPendingRegistration() const -> bool;

      void markRegistered();

    private:
      void iterateOverTemplateDescHierarchy(const TemplateDescription& desc,
                                            eastl::function<void(const TemplateDescription&)> cb);

      auto createTemplate(const TemplateDescription& tmpl_desc,
                          Archetypes& archetypes,
                          const RegisteredComponents& registered_comps) -> template_id_t;

      auto registerNewComponents(RegisteredComponents& registered_comps,
                                 const TemplateComponentsMap& comps_map) -> bool;

    private:
      eastl::vector_map<name_hash_t, TemplateDescription> m_DescMap;
      eastl::vector<Template> m_Templates;
      bool m_HasPendingRegistration = false;
  };
};
