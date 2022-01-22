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
  inline const String& GetName() const
  {
    return m_Name;
  }
  inline const  String& GetAnnotation() const
  {
    return m_Annotation;
  }

  DataBlock* GetChildBlock(const  String& path);
  const DataBlock* GetChildBlock(const  String& path) const;

  size_t AddAttribute(const Attribute& attribute);
  int GetInt(const  String& name) const;
  Int2 GetInt2(const  String& name) const;
  Int3 GetInt3(const  String& name) const;
  Int4 GetInt4(const  String& name) const;
  float GetFloat(const  String& name) const;
  Float2 GetFloat2(const  String& name) const;
  Float3 GetFloat3(const  String& name) const;
  Float4 GetFloat4(const  String& name) const;
  bool GetBool(const  String& name) const;
  String GetText(const  String& name) const;

  int GetInt(const String& name, const int def) const;
  Int2 GetInt2(const String& name, const Int2& def) const;
  Int3 GetInt3(const String& name, const Int3& def) const;
  Int4 GetInt4(const String& name, const Int4& def) const;
  float GetFloat(const String& name, const float def) const;
  Float2 GetFloat2(const String& name, const Float2 def) const;
  Float3 GetFloat3(const String& name, const Float3 def) const;
  Float4 GetFloat4(const String& name, const Float4 def) const;
  bool GetBool(const String& name, const bool def) const;
  String GetText(const String& name, const String& def) const;

  inline const BlocksArray& GetChildBlocks() const
  {
    return m_ChildBlocks;
  }

  inline const AttributesArray& GetAttributes() const
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
