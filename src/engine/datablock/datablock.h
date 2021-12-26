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
      int, Int2, Int3, Int4,
      float, Float2, Float3, Float4,
      Mat3, Mat4,
      String
    > Value;

    String name;
    String annotation;
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
  inline const String& get_name() const
  {
    return m_Name;
  }
  inline const  String& get_annotation() const
  {
    return m_Annotation;
  }

  DataBlock* get_child_block(const  String& path);
  const DataBlock* get_child_block(const  String& path) const;

  size_t add_attribute(const Attribute& attribute);
  int get_int(const  String& name) const;
  Int2 get_int2(const  String& name) const;
  Int3 get_int3(const  String& name) const;
  Int4 get_int4(const  String& name) const;
  float get_float(const  String& name) const;
  Float2 get_float2(const  String& name) const;
  Float3 get_float3(const  String& name) const;
  Float4 get_float4(const  String& name) const;
  bool get_bool(const  String& name) const;
  String get_text(const  String& name) const;

  int get_int(const String& name, const int def) const;
  Int2 get_int2(const String& name, const Int2& def) const;
  Int3 get_int3(const String& name, const Int3& def) const;
  Int4 get_int4(const String& name, const Int4& def) const;
  float get_float(const String& name, const float def) const;
  Float2 get_float2(const String& name, const Float2 def) const;
  Float3 get_float3(const String& name, const Float3 def) const;
  Float4 get_float4(const String& name, const Float4 def) const;
  bool get_bool(const String& name, const bool def) const;
  String get_text(const String& name, const String& def) const;

  inline const BlocksArray& get_child_blocks() const
  {
    return m_ChildBlocks;
  }

  inline const AttributesArray& get_attributes() const
  {
    return m_Attributes;
  }

private:
  String m_Name;
  String m_Annotation;
  BlocksArray m_ChildBlocks;
  AttributesArray m_Attributes;
};

static DataBlock EMPTY_BLOCK;
