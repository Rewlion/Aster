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

    inline const char* getTypeStr() const
    {
      return Ast::GetAttributeTypeStr(type);
    }
  };

  typedef std::vector<DataBlock> BlocksArray;
  typedef std::vector<Attribute> AttributesArray;

public:
  inline const string& getName() const
  {
    return m_Name;
  }
  inline const  string& getAnnotation() const
  {
    return m_Annotation;
  }

  DataBlock* getChildBlock(const  string& path);
  const DataBlock* getChildBlock(const  string& path) const;

  size_t addAttribute(const Attribute& attribute);
  int    getInt(const string& name) const;
  int2   getInt2(const string& name) const;
  int3   getInt3(const string& name) const;
  int4   getInt4(const string& name) const;
  float  getFloat(const string& name) const;
  float2 getFloat2(const string& name) const;
  float3 getFloat3(const string& name) const;
  float4 getFloat4(const string& name) const;
  bool   getBool(const string& name) const;
  string getText(const string& name) const;

  int    getInt(const string& name, const int def) const;
  int2   getInt2(const string& name, const int2& def) const;
  int3   getInt3(const string& name, const int3& def) const;
  int4   getInt4(const string& name, const int4& def) const;
  float  getFloat(const string& name, const float def) const;
  float2 getFloat2(const string& name, const float2 def) const;
  float3 getFloat3(const string& name, const float3 def) const;
  float4 getFloat4(const string& name, const float4 def) const;
  bool   getBool(const string& name, const bool def) const;
  string getText(const string& name, const string& def) const;

  inline const BlocksArray& getChildBlocks() const
  {
    return m_ChildBlocks;
  }

  inline const AttributesArray& getAttributes() const
  {
    return m_Attributes;
  }

  bool isEmpty() const;

private:
  string m_Name;
  string m_Annotation;
  BlocksArray m_ChildBlocks;
  AttributesArray m_Attributes;
};

static DataBlock EMPTY_BLOCK;
