#include "class.h"
#include "field.h"

#include <engine/assert.h>
#include <engine/types.h>

#include <cstring>

namespace
{
  template<class T>
  void copy_helper(void* dst, const void* src)
  {
    T* dstT = reinterpret_cast<T*>(dst);
    const T* srcT = reinterpret_cast<const T*>(src);

    *dstT = *srcT;
  }
}

void Class::setFieldValue(const ClassField* field, void* field_owner, const void* value) const
{
  void* dst = reinterpret_cast<uint8_t*>(field_owner) + field->offset;

  switch (field->type)
  {
    case ClassField::Type::Int:  std::memcpy(dst, value, sizeof(int));  break;
    case ClassField::Type::Int2: std::memcpy(dst, value, sizeof(int2)); break;
    case ClassField::Type::Int3: std::memcpy(dst, value, sizeof(int3)); break;
    case ClassField::Type::Int4: std::memcpy(dst, value, sizeof(int4)); break;

    case ClassField::Type::Float:  std::memcpy(dst, value, sizeof(float));  break;
    case ClassField::Type::Float2: std::memcpy(dst, value, sizeof(float2)); break;
    case ClassField::Type::Float3: std::memcpy(dst, value, sizeof(float3)); break;
    case ClassField::Type::Float4: std::memcpy(dst, value, sizeof(float4)); break;

    case ClassField::Type::String:
    {
      copy_helper<string>(dst, value);
      break;
    }

    default:
    {
      ASSERT_FMT(false, "unimplemented reflection set value. Field: [name:{}, type:{}]", field->name, (int)field->type);
    }
  }
}

void Class::getFieldValue(const ClassField* field, const void* field_owner, void* value) const
{
  const void* src = reinterpret_cast<const uint8_t*>(field_owner) + field->offset;

  switch (field->type)
  {
    case ClassField::Type::Int:  std::memcpy(value, src,  sizeof(int));  break;
    case ClassField::Type::Int2: std::memcpy(value, src, sizeof(int2)); break;
    case ClassField::Type::Int3: std::memcpy(value, src, sizeof(int3)); break;
    case ClassField::Type::Int4: std::memcpy(value, src, sizeof(int4)); break;

    case ClassField::Type::Float:  std::memcpy(value, src,  sizeof(float));  break;
    case ClassField::Type::Float2: std::memcpy(value, src, sizeof(float2)); break;
    case ClassField::Type::Float3: std::memcpy(value, src, sizeof(float3)); break;
    case ClassField::Type::Float4: std::memcpy(value, src, sizeof(float4)); break;

    case ClassField::Type::String:
    {
      copy_helper<string>(value, src);
      break;
    }

    default:
    {
      ASSERT_FMT(false, "unimplemented reflection set value. Field: [name:{}, type:{}]", field->name, (int)field->type);
    }
  }
}
