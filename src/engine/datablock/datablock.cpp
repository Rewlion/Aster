#include "datablock.h"

#include <sstream>

const DataBlock* DataBlock::GetChildBlock(const String& path) const
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

DataBlock* DataBlock::GetChildBlock(const String& path)
{
  const DataBlock* constThis = const_cast<const DataBlock*>(this);
  return const_cast<DataBlock*>(constThis->GetChildBlock(path));
}

size_t DataBlock::AddAttribute(const Attribute& attribute)
{
  const size_t id = m_Attributes.size();
  m_Attributes.push_back(attribute);
  return id;
}

int DataBlock::GetInt(const String& name) const
{
  return GetInt(name, 0);
}

Int2 DataBlock::GetInt2(const String& name) const
{
  return GetInt2(name, {0, 0});
}

Int3 DataBlock::GetInt3(const String& name) const
{
  return GetInt3(name, {0, 0, 0});
}

Int4 DataBlock::GetInt4(const String& name) const
{
  return GetInt4(name, {0, 0, 0, 0});
}

float DataBlock::GetFloat(const String& name) const
{
  return GetFloat(name, 0.0f);
}

Float2 DataBlock::GetFloat2(const String& name) const
{
  return GetFloat2(name, {0.0f, 0.0f});
}

Float3 DataBlock::GetFloat3(const String& name) const
{
  return GetFloat3(name, {0.0f, 0.0f, 0.0f});
}

Float4 DataBlock::GetFloat4(const String& name) const
{
  return GetFloat4(name, {0.0f, 0.0f, 0.0f, 0.0f});
}

bool DataBlock::GetBool(const String& name) const
{
  return GetBool(name, false);
}

String DataBlock::GetText(const String& name) const
{
  return GetText(name, "");
}

int DataBlock::GetInt(const String& name, const int def) const
{
  for (const auto& a: m_Attributes)
    if (a.name == name && a.type == ValueType::Int)
      return std::get<int>(a.as);

  return def;
}

 Int2 DataBlock::GetInt2(const String& name, const Int2& def) const
 {
   for (const auto& a: m_Attributes)
     if (a.name == name && a.type == ValueType::Int2)
       return std::get<Int2>(a.as);

   return def;
 }

Int3 DataBlock::GetInt3(const String& name, const Int3& def) const
{
 for (const auto& a: m_Attributes)
   if (a.name == name && a.type == ValueType::Int3)
     return std::get<Int3>(a.as);

 return def;
}

Int4 DataBlock::GetInt4(const String& name, const Int4& def) const
{
  for (const auto& a: m_Attributes)
    if (a.name == name && a.type == ValueType::Int4)
      return std::get<Int4>(a.as);

  return def;
}

float DataBlock::GetFloat(const String& name, const float def) const
{
  for (const auto& a: m_Attributes)
    if (a.name == name && a.type == ValueType::Float)
      return std::get<float>(a.as);

  return def;
}

Float2 DataBlock::GetFloat2(const String& name, const Float2 def) const
{
  for (const auto& a: m_Attributes)
    if (a.name == name && a.type == ValueType::Float2)
      return std::get<Float2>(a.as);

  return def;
}

Float3 DataBlock::GetFloat3(const String& name, const Float3 def) const
{
  for (const auto& a: m_Attributes)
    if (a.name == name && a.type == ValueType::Float3)
      return std::get<Float3>(a.as);

  return def;
}

Float4 DataBlock::GetFloat4(const String& name, const Float4 def) const
{
  for (const auto& a: m_Attributes)
    if (a.name == name && a.type == ValueType::Float4)
      return std::get<Float4>(a.as);

  return def;
}

bool DataBlock::GetBool(const String& name, const bool def) const
{
  for (const auto& a: m_Attributes)
    if (a.name == name && a.type == ValueType::Bool)
      return std::get<bool>(a.as);

  return def;
}

String DataBlock::GetText(const String& name, const String& def) const
{
  for (const auto& a: m_Attributes)
    if (a.name == name && a.type == ValueType::Text)
      return std::get<String>(a.as);

  return def;
}
