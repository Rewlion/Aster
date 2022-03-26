#include "load_templates.h"

#include <engine/datablock/datablock.h>
#include <engine/datablock/utils.h>
#include <engine/ecs/registry.h>
#include <engine/types.h>
#include <engine/log.h>

namespace Engine::ECS
{
  static void add_template(Registry& manager, const DataBlock& tmpl)
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

    string templateDump = tmpl.getName();
    for(auto& comp: templateDesc)
    {
      templateDump = templateDump + "\n" + "component:" + comp.name;
    }
    log("added template {}", templateDump);

    manager.addTemplate(str_hash(tmpl.getName().c_str()), templateDesc);
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
      if (tmpl.getAnnotation() == "entity_template")
        add_template(manager, tmpl);
    }
  }
}
