#include "load_templates.h"

#include <engine/datablock/datablock.h>
#include <engine/datablock/utils.h>
#include <engine/ecs/registry.h>
#include <engine/types.h>
#include <engine/log.h>

namespace Engine::ECS
{
  static void AddTemplate(Registry& ecs, const DataBlock& tmpl)
  {
    eastl::vector<ComponentDescription> templateDesc;

    for(auto& attr: tmpl.GetAttributes())
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
          logwarn("ecs: template {}: can't add template's attribute {}: unknown type {}",
            tmpl.GetName(), attr.name, attr.type, attr.GetTypeStr());
      }
    }

    string templateDump = tmpl.GetName();
    for(auto& comp: templateDesc)
    {
      templateDump = templateDump + "\n" + "component:" + comp.name;
    }
    log("added template {}", templateDump);

    ecs.add_template(str_hash(tmpl.GetName().c_str()), templateDesc);
  }

  void AddTemplatesFromBlk(Registry& ecs, const string& blkPath)
  {
    DataBlock blk;
    if (!LoadBlkFromFile(&blk, blkPath.c_str()))
    {
      logerror("failed to load templates from {}", blkPath);
      return;
    }

    for(const auto& tmpl: blk.GetChildBlocks())
    {
      if (tmpl.GetAnnotation() == "entity_template")
        AddTemplate(ecs, tmpl);
    }
  }
}
