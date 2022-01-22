#pragma once

#include <string>
#include <variant>

#include <engine/types.h>
#include <engine/datablock/parser/ast.h>
#include <glm/glm.hpp>

class DataBlock
{
  friend class BlkParser;
public:
  using ValueType = Ast::AttributeType;
  struct Attribute
  {
    typedef std::variant<
      bool,
      int, int2, int3, int4,
      float, float2, float3, float4,
      mat3, mat4,
      string
    > Value;

    string name;
    string annotation;
    ValueType type = ValueType::None;
    Value as;

    inline const char* GetTypeStr() const
    {
      return Ast::GetAttributeTypeStr(type);
    }
  };

  typedef std::vector<DataBlock> BlocksArray;
  typedef std::vector<Attribute> AttributesArray;

public:
  inline const string& GetName() const
  {
    return m_Name;
  }
  inline const  string& GetAnnotation() const
  {
    return m_Annotation;
  }

  DataBlock* GetChildBlock(const  string& path);
  const DataBlock* GetChildBlock(const  string& path) const;

  size_t AddAttribute(const Attribute& attribute);
  int GetInt(const  string& name) const;
  int2 GetInt2(const  string& name) const;
  int3 GetInt3(const  string& name) const;
  int4 GetInt4(const  string& name) const;
  float GetFloat(const  string& name) const;
  float2 GetFloat2(const  string& name) const;
  float3 GetFloat3(const  string& name) const;
  float4 GetFloat4(const  string& name) const;
  bool GetBool(const  string& name) const;
  string GetText(const  string& name) const;

  int GetInt(const string& name, const int def) const;
  int2 GetInt2(const string& name, const int2& def) const;
  int3 GetInt3(const string& name, const int3& def) const;
  int4 GetInt4(const string& name, const int4& def) const;
  float GetFloat(const string& name, const float def) const;
  float2 GetFloat2(const string& name, const float2 def) const;
  float3 GetFloat3(const string& name, const float3 def) const;
  float4 GetFloat4(const string& name, const float4 def) const;
  bool GetBool(const string& name, const bool def) const;
  string GetText(const string& name, const string& def) const;

  inline const BlocksArray& GetChildBlocks() const
  {
    return m_ChildBlocks;
  }

  inline const AttributesArray& GetAttributes() const
  {
    return m_Attributes;
  }

private:
  string m_Name;
  string m_Annotation;
  BlocksArray m_ChildBlocks;
  AttributesArray m_Attributes;
};

static DataBlock EMPTY_BLOCK;
