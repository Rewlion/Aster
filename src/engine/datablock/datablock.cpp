#include "datablock.h"

#include <sstream>

const DataBlock* DataBlock::GetChildBlock(const string& path) const
{
  std::stringstream buf(path);
  string subPath;
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

DataBlock* DataBlock::GetChildBlock(const string& path)
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

int DataBlock::GetInt(const string& name) const
{
  return GetInt(name, 0);
}

int2 DataBlock::GetInt2(const string& name) const
{
  return GetInt2(name, {0, 0});
}

int3 DataBlock::GetInt3(const string& name) const
{
  return GetInt3(name, {0, 0, 0});
}

int4 DataBlock::GetInt4(const string& name) const
{
  return GetInt4(name, {0, 0, 0, 0});
}

float DataBlock::GetFloat(const string& name) const
{
  return GetFloat(name, 0.0f);
}

float2 DataBlock::GetFloat2(const string& name) const
{
  return GetFloat2(name, {0.0f, 0.0f});
}

float3 DataBlock::GetFloat3(const string& name) const
{
  return GetFloat3(name, {0.0f, 0.0f, 0.0f});
}

float4 DataBlock::GetFloat4(const string& name) const
{
  return GetFloat4(name, {0.0f, 0.0f, 0.0f, 0.0f});
}

bool DataBlock::GetBool(const string& name) const
{
  return GetBool(name, false);
}

string DataBlock::GetText(const string& name) const
{
  return GetText(name, "");
}

int DataBlock::GetInt(const string& name, const int def) const
{
  for (const auto& a: m_Attributes)
    if (a.name == name && a.type == ValueType::Int)
      return std::get<int>(a.as);

  return def;
}

 int2 DataBlock::GetInt2(const string& name, const int2& def) const
 {
   for (const auto& a: m_Attributes)
     if (a.name == name && a.type == ValueType::Int2)
       return std::get<int2>(a.as);

   return def;
 }

int3 DataBlock::GetInt3(const string& name, const int3& def) const
{
 for (const auto& a: m_Attributes)
   if (a.name == name && a.type == ValueType::Int3)
     return std::get<int3>(a.as);

 return def;
}

int4 DataBlock::GetInt4(const string& name, const int4& def) const
{
  for (const auto& a: m_Attributes)
    if (a.name == name && a.type == ValueType::Int4)
      return std::get<int4>(a.as);

  return def;
}

float DataBlock::GetFloat(const string& name, const float def) const
{
  for (const auto& a: m_Attributes)
    if (a.name == name && a.type == ValueType::Float)
      return std::get<float>(a.as);

  return def;
}

float2 DataBlock::GetFloat2(const string& name, const float2 def) const
{
  for (const auto& a: m_Attributes)
    if (a.name == name && a.type == ValueType::Float2)
      return std::get<float2>(a.as);

  return def;
}

float3 DataBlock::GetFloat3(const string& name, const float3 def) const
{
  for (const auto& a: m_Attributes)
    if (a.name == name && a.type == ValueType::Float3)
      return std::get<float3>(a.as);

  return def;
}

float4 DataBlock::GetFloat4(const string& name, const float4 def) const
{
  for (const auto& a: m_Attributes)
    if (a.name == name && a.type == ValueType::Float4)
      return std::get<float4>(a.as);

  return def;
}

bool DataBlock::GetBool(const string& name, const bool def) const
{
  for (const auto& a: m_Attributes)
    if (a.name == name && a.type == ValueType::Bool)
      return std::get<bool>(a.as);

  return def;
}

string DataBlock::GetText(const string& name, const string& def) const
{
  for (const auto& a: m_Attributes)
    if (a.name == name && a.type == ValueType::Text)
      return std::get<string>(a.as);

  return def;
}

bool DataBlock::IsEmpty() const
{
  return this == &EMPTY_BLOCK;
}
