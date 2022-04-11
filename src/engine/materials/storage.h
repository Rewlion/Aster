#pragma once

#include <engine/algorithm/hash.h>
#include <engine/assert.h>

#include <EASTL/vector_map.h>
#include <EASTL/functional.h>

#include <memory>

namespace Engine
{
  using MaterialCtor = eastl::function<Material*()>;

  class MaterialsStorage
  {
    using MaterialsMap = eastl::vector_map<string_hash, MaterialCtor>;

    public:

      template<class T>
      static bool registerMaterial(const string_hash nameHash)
      {
        MaterialsMap& mats = getMaterials();

        mats.insert({
          nameHash,
          [](){return new T{};}
        });

        return true;
      }

      static Material* constructMaterial(const string& name)
      {
        const string_hash materialHash = str_hash(name.c_str());

        MaterialsMap& mats = getMaterials();
        const auto it = mats.find(materialHash);
        ASSERT(it != mats.end());

        return it->second();
      }

    private:
      static MaterialsMap& getMaterials()
      {
        static MaterialsMap map = MaterialsMap{};
        return map;
      }

    private:
      MaterialsMap m_RegisteredMaterials;
  };

  #define REGISTER_MATERIAL(type) const bool __ ## type ## __reg = Engine::MaterialsStorage::registerMaterial<type>(str_hash(#type))
}
