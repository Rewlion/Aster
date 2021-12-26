#include "datablock.h"

#include <sstream>

const DataBlock* DataBlock::get_child_block(const String& path) const
{
  std::stringstream buf(path);
  String subPath;
  const DataBlock* dbk = this;
  while(std::getline(buf, subPath, '/'))
  {
    bool found = false;
    for(const DataBlock& child: dbk->m_ChildBlocks)
    {
      if (child.m_Name == subPath)
      {
        dbk = &child;
        found = true;
        break;
      }
    }

    if (!found)
      return &EMPTY_BLOCK;
  }
  return dbk;
}

DataBlock* DataBlock::get_child_block(const String& path)
{
  const DataBlock* constThis = const_cast<const DataBlock*>(this);
  return const_cast<DataBlock*>(constThis->get_child_block(path));
}

size_t DataBlock::add_attribute(const Attribute& attribute)
{
  const size_t id = m_Attributes.size();
  m_Attributes.push_back(attribute);
  return id;
}

int DataBlock::get_int(const String& name) const
{
  return get_int(name, 0);
}

Int2 DataBlock::get_int2(const String& name) const
{
  return get_int2(name, {0, 0});
}

Int3 DataBlock::get_int3(const String& name) const
{
  return get_int3(name, {0, 0, 0});
}

Int4 DataBlock::get_int4(const String& name) const
{
  return get_int4(name, {0, 0, 0, 0});
}

float DataBlock::get_float(const String& name) const
{
  return get_float(name, 0.0f);
}

Float2 DataBlock::get_float2(const String& name) const
{
  return get_float2(name, {0.0f, 0.0f});
}

Float3 DataBlock::get_float3(const String& name) const
{
  return get_float3(name, {0.0f, 0.0f, 0.0f});
}

Float4 DataBlock::get_float4(const String& name) const
{
  return get_float4(name, {0.0f, 0.0f, 0.0f, 0.0f});
}

bool DataBlock::get_bool(const String& name) const
{
  return get_bool(name, false);
}

String DataBlock::get_text(const String& name) const
{
  return get_text(name, "");
}

int DataBlock::get_int(const String& name, const int def) const
{
  for (const auto& a: m_Attributes)
    if (a.name == name && a.type == ValueType::Int)
      return std::get<int>(a.as);

  return def;
}

 Int2 DataBlock::get_int2(const String& name, const Int2& def) const
 {
   for (const auto& a: m_Attributes)
     if (a.name == name && a.type == ValueType::Int2)
       return std::get<Int2>(a.as);

   return def;
 }

Int3 DataBlock::get_int3(const String& name, const Int3& def) const
{
 for (const auto& a: m_Attributes)
   if (a.name == name && a.type == ValueType::Int3)
     return std::get<Int3>(a.as);

 return def;
}

Int4 DataBlock::get_int4(const String& name, const Int4& def) const
{
  for (const auto& a: m_Attributes)
    if (a.name == name && a.type == ValueType::Int4)
      return std::get<Int4>(a.as);

  return def;
}

float DataBlock::get_float(const String& name, const float def) const
{
  for (const auto& a: m_Attributes)
    if (a.name == name && a.type == ValueType::Float)
      return std::get<float>(a.as);

  return def;
}

Float2 DataBlock::get_float2(const String& name, const Float2 def) const
{
  for (const auto& a: m_Attributes)
    if (a.name == name && a.type == ValueType::Float2)
      return std::get<Float2>(a.as);

  return def;
}

Float3 DataBlock::get_float3(const String& name, const Float3 def) const
{
  for (const auto& a: m_Attributes)
    if (a.name == name && a.type == ValueType::Float3)
      return std::get<Float3>(a.as);

  return def;
}

Float4 DataBlock::get_float4(const String& name, const Float4 def) const
{
  for (const auto& a: m_Attributes)
    if (a.name == name && a.type == ValueType::Float4)
      return std::get<Float4>(a.as);

  return def;
}

bool DataBlock::get_bool(const String& name, const bool def) const
{
  for (const auto& a: m_Attributes)
    if (a.name == name && a.type == ValueType::Bool)
      return std::get<bool>(a.as);

  return def;
}

String DataBlock::get_text(const String& name, const String& def) const
{
  for (const auto& a: m_Attributes)
    if (a.name == name && a.type == ValueType::Text)
      return std::get<String>(a.as);

  return def;
}
