#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <variant>
#include <string>
#include <vector>

namespace Ast
{
  enum class AttributeType
  {
    None,
    Int,
    Int2,
    Int3,
    Int4,
    Float,
    Float2,
    Float3,
    Float4,
    Text,
    Mat3,
    Mat4,
    Bool
  };

  inline const char* GetAttributeTypeStr(const AttributeType t)
  {
    switch(t)
    {
      case AttributeType::None: return "None";
      case AttributeType::Int: return "int";
      case AttributeType::Int2: return "int2";
      case AttributeType::Int3: return "int3";
      case AttributeType::Int4: return "int4";
      case AttributeType::Float: return "float";
      case AttributeType::Float2: return "float2";
      case AttributeType::Float3: return "float3";
      case AttributeType::Float4: return "float4";
      case AttributeType::Text: return "text";
      case AttributeType::Mat4: return "mat4";
      case AttributeType::Mat3: return "mat3";
      case AttributeType::Bool: return "bool";
      default: assert(!"Unknown attribute"); return "";
    }
  }

  enum class ParamType
  {
    None,
    Attribute,
    Block
  };

  struct Node
  {

  };

  struct AnnotatedParam;
  struct Config: public Node
  {
    AnnotatedParam* paramList;
    bool isValid = true;
    std::string blkFile;
  };

  struct AnnotatedParam: public Node
  {
    std::string annotation = "";
    std::string name;
    AnnotatedParam* next = nullptr;

    virtual ParamType GetType() const { return ParamType::None; }
  };

  struct AttributeValue;
  struct Attribute: public AnnotatedParam
  {
    AttributeType type;
    AttributeValue* valueNode = nullptr;

    virtual ParamType GetType() const { return ParamType::Attribute; }
  };

  struct Block: public AnnotatedParam
  {
    AnnotatedParam* paramList = nullptr;

    virtual ParamType GetType() const { return ParamType::Block; }
  };

  struct AttributeValue: public Node
  {
    virtual AttributeType GetType() const { return AttributeType::None; }

    virtual std::string ToStr() const { return ""; }
    const char* GetTypeStr() const { return GetAttributeTypeStr(GetType()); }
  };

  struct Mat4Value: public AttributeValue
  {
    virtual AttributeType GetType() const { return AttributeType::Mat4; }
    virtual std::string ToStr() const { return glm::to_string(value); }
    glm::mat4 value;
  };

  struct Mat3Value: public AttributeValue
  {
    virtual AttributeType GetType() const { return AttributeType::Mat3; }
    virtual std::string ToStr() const { return glm::to_string(value); }
    glm::mat3 value;
  };

  struct Float4Value: public AttributeValue
  {
    virtual AttributeType GetType() const { return AttributeType::Float4; }
    virtual std::string ToStr() const { return glm::to_string(value); }
    glm::vec4 value;
  };

  struct Float3Value: public AttributeValue
  {
    virtual AttributeType GetType() const { return AttributeType::Float3; }
    virtual std::string ToStr() const { return glm::to_string(value); }
    glm::vec3 value;
  };

  struct Float2Value: public AttributeValue
  {
    virtual AttributeType GetType() const { return AttributeType::Float2; }
    virtual std::string ToStr() const { return glm::to_string(value); }
    glm::vec2 value;
  };

  struct FloatValue: public AttributeValue
  {
    virtual AttributeType GetType() const { return AttributeType::Float; }
    virtual std::string ToStr() const { return std::to_string(value); }
    float value;
  };

  struct Int4Value: public AttributeValue
  {
    virtual AttributeType GetType() const { return AttributeType::Int4; }
    virtual std::string ToStr() const { return glm::to_string(value); }
    glm::ivec4 value;
  };

  struct Int3Value: public AttributeValue
  {
    virtual AttributeType GetType() const { return AttributeType::Int3; }
    virtual std::string ToStr() const { return glm::to_string(value); }
    glm::ivec3 value;
  };

  struct Int2Value: public AttributeValue
  {
    virtual AttributeType GetType() const { return AttributeType::Int2; }
    virtual std::string ToStr() const { return glm::to_string(value); }
    glm::ivec2 value;
  };

  struct IntValue: public AttributeValue
  {
    virtual AttributeType GetType() const { return AttributeType::Int; }
    virtual std::string ToStr() const { return std::to_string(value); }
    int value;
  };

  struct TextValue: public AttributeValue
  {
    virtual AttributeType GetType() const { return AttributeType::Text; }
    virtual std::string ToStr() const { return value; }
    std::string value;
  };

  struct BoolValue: public AttributeValue
  {
    virtual AttributeType GetType() const { return AttributeType::Bool; }
    virtual std::string ToStr() const { return value? "true" : "false"; }
    bool value;
  };
}
