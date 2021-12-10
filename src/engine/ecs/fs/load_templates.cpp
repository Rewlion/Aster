#include "load_templates.h"

#include <engine/datablock/datablock.h>
#include <engine/datablock/utils.h>
#include <engine/ecs/registry.h>
#include <engine/types.h>
#include <engine/log.h>

static void add_template(Registry& ecs, const DataBlock& tmpl)
{
  eastl::vector<ComponentDescription> templateDesc;

  for(auto& attr: tmpl.get_attributes())
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
        templateDesc.push_back(DESCRIBE_COMPONENT(attr.name.c_str(), Int2));
        break;
      }
      case DataBlock::ValueType::Int3:
      {
        templateDesc.push_back(DESCRIBE_COMPONENT(attr.name.c_str(), Int3));
        break;
      }
      case DataBlock::ValueType::Int4:
      {
        templateDesc.push_back(DESCRIBE_COMPONENT(attr.name.c_str(), Int4));
        break;
      }
      case DataBlock::ValueType::Float:
      {
        templateDesc.push_back(DESCRIBE_COMPONENT(attr.name.c_str(), float));
        break;
      }
      case DataBlock::ValueType::Float2:
      {
        templateDesc.push_back(DESCRIBE_COMPONENT(attr.name.c_str(), Float2));
        break;
      }
      case DataBlock::ValueType::Float3:
      {
        templateDesc.push_back(DESCRIBE_COMPONENT(attr.name.c_str(), Float3));
        break;
      }
      case DataBlock::ValueType::Float4:
      {
        templateDesc.push_back(DESCRIBE_COMPONENT(attr.name.c_str(), Float4));
        break;
      }
      case DataBlock::ValueType::Text:
      {
        templateDesc.push_back(DESCRIBE_COMPONENT(attr.name.c_str(), String));
        break;
      }
      case DataBlock::ValueType::Bool:
      {
        templateDesc.push_back(DESCRIBE_COMPONENT(attr.name.c_str(), bool));
        break;
      }
      default:
        logwarn("ecs: template {}: can't add template's attribute {}: unknown type {}",
          tmpl.get_name(), attr.name, attr.type, attr.GetTypeStr());
    }
  }

  String templateDump = tmpl.get_name();
  for(auto& comp: templateDesc)
  {
    templateDump = templateDump + "\n" + "component:" + comp.name;
  }
  log("added template {}", templateDump);

  ecs.add_template(str_hash(tmpl.get_name().c_str()), templateDesc);
}

void add_templates_from_blk(Registry& ecs, const String& blkPath)
{
  DataBlock blk;
  if (!load_blk_from_file(&blk, blkPath.c_str()))
  {
    logerror("failed to load templates from {}", blkPath);
    return;
  }

  for(const auto& tmpl: blk.get_child_blocks())
  {
    if (tmpl.get_annotation() == "entity_template")
      add_template(ecs, tmpl);
  }
}