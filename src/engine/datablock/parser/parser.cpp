#include "parser.h"

#include <cstdio>
#include <cstdlib>

extern int yyparse(Ast::Config* root);
extern void yyrestart(FILE* f);
extern FILE *yyin;

DataBlock BlkParser::parse_file(const String& path)
{
  FILE* f = fopen(path.c_str(), "r");

  if (!f)
  {
      fprintf(stderr, "can.t open a file %s", path.c_str());
      exit(-1);
  }

  yyin = f;
  Ast::Config root;
  yyrestart(f);
  yyparse(&root);
  fclose(f);

  return traverse_ast(root);
}

DataBlock BlkParser::traverse_ast(const Ast::Config& ast)
{
  return construct_block(ast.paramList);
}

DataBlock BlkParser::construct_block(const Ast::AnnotatedParam* paramList)
{
  DataBlock dbk;
  for(const Ast::AnnotatedParam* p = paramList; p != nullptr; p = p->next)
  {
    switch(p->GetType())
    {
      case Ast::ParamType::Attribute:
      {
        const Ast::Attribute* attributeNode = reinterpret_cast<const Ast::Attribute*>(p);
        DataBlock::Attribute value = construct_attribute(attributeNode);
        dbk.m_Attributes.push_back(value);
        break;
      }

      case Ast::ParamType::Block:
      {
        const Ast::Block* childBlockNode = reinterpret_cast<const Ast::Block*>(p);
        DataBlock childDbk = construct_block(childBlockNode->paramList);
        childDbk.m_Name = childBlockNode->name;
        childDbk.m_Annotation = childBlockNode->annotation;
        dbk.m_ChildBlocks.push_back(childDbk);
        break;
      }
    }
  }
  return dbk;
}

DataBlock::Attribute BlkParser::construct_attribute(const Ast::Attribute* attribute)
{
  DataBlock::Attribute value;
  value.name = attribute->name;
  value.annotation = attribute->annotation;
  value.type = attribute->type;

  switch(attribute->type)
  {
    case Ast::AttributeType::Int:
    {
      value.as = reinterpret_cast<const Ast::IntValue*>(attribute->valueNode)->value;
      break;
    }

    case Ast::AttributeType::Int2:
    {
      value.as = reinterpret_cast<const Ast::Int2Value*>(attribute->valueNode)->value;
      break;
    }

    case Ast::AttributeType::Int3:
    {
      value.as = reinterpret_cast<const Ast::Int3Value*>(attribute->valueNode)->value;
      break;
    }

    case Ast::AttributeType::Int4:
    {
      value.as = reinterpret_cast<const Ast::Int4Value*>(attribute->valueNode)->value;
      break;
    }

    case Ast::AttributeType::Float:
    {
      value.as = reinterpret_cast<const Ast::FloatValue*>(attribute->valueNode)->value;
      break;
    }

    case Ast::AttributeType::Float2:
    {
      value.as = reinterpret_cast<const Ast::Float2Value*>(attribute->valueNode)->value;
      break;
    }

    case Ast::AttributeType::Float3:
    {
      value.as = reinterpret_cast<const Ast::Float3Value*>(attribute->valueNode)->value;
      break;
    }

    case Ast::AttributeType::Float4:
    {
      value.as = reinterpret_cast<const Ast::Float4Value*>(attribute->valueNode)->value;
      break;
    }

    case Ast::AttributeType::Text:
    {
      value.as = reinterpret_cast<const Ast::TextValue*>(attribute->valueNode)->value;
      break;
    }

    case Ast::AttributeType::Mat4:
    {
      value.as = reinterpret_cast<const Ast::Mat4Value*>(attribute->valueNode)->value;
      break;
    }

    case Ast::AttributeType::Mat3:
    {
      value.as = reinterpret_cast<const Ast::Mat3Value*>(attribute->valueNode)->value;
      break;
    }

    case Ast::AttributeType::Bool:
    {
      value.as = reinterpret_cast<const Ast::BoolValue*>(attribute->valueNode)->value;
      break;
    }
    default: assert(!"Unknown attribute");
  }

  return value;
}
