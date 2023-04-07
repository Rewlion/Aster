#pragma once

#include <string>
#include <variant>

#include <engine/types.h>

#include <EASTL/vector.h>
#include <glm/glm.hpp>

class DataBlock
{
  friend class BlkParser;
public:
  struct Attribute
  {
    enum class Type
    {
      Bool,
      Int, Int2, Int3, Int4,
      Float, Float2, Float3, Float4,
      Mat3, Mat4,
      Text,
    };

    typedef std::variant<
      bool,
      int, int2, int3, int4,
      float, float2, float3, float4,
      mat3, mat4,
      string
    > Value;

    string name;
    string annotation;
    Type type = Type::Bool;
    Value as;

    inline const char* getTypeStr() const
    {
      switch(type)
      {
        case Type::Int: return "int";
        case Type::Int2: return "int2";
        case Type::Int3: return "int3";
        case Type::Int4: return "int4";
        case Type::Float: return "float";
        case Type::Float2: return "float2";
        case Type::Float3: return "float3";
        case Type::Float4: return "float4";
        case Type::Text: return "text";
        case Type::Mat4: return "mat4";
        case Type::Mat3: return "mat3";
        case Type::Bool: return "bool";
      }
    }
  };

  typedef eastl::vector<DataBlock> BlocksArray;
  typedef eastl::vector<Attribute> AttributesArray;

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

  auto getAttribute(const string_view name, const Attribute::Type) const -> const Attribute*;
  auto getAttribute(const string_view name, const Attribute::Type) -> const Attribute*;

  inline const BlocksArray& getChildBlocks() const
  {
    return m_ChildBlocks;
  }

  inline const AttributesArray& getAttributes() const
  {
    return m_Attributes;
  }

  bool isEmpty() const;

  void insertBlock(const DataBlock& bk) { m_ChildBlocks.push_back(std::move(bk)); }
  void insertAttribute(const Attribute& attr) { m_Attributes.push_back(std::move(attr)); }

private:
  string m_Name;
  string m_Annotation;
  BlocksArray m_ChildBlocks;
  AttributesArray m_Attributes;
};

static DataBlock EMPTY_BLOCK;
