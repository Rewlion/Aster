#pragma once

#include <boost/serialization/binary_object.hpp>

namespace boost::serialization
{
  template <class Archive>
  void save(Archive& a, const ShadersSystem::ByteCodes& bc, const unsigned int version)
  {
    const size_t count = bc.size();
  	a & count
      & make_binary_object(bc.data(), bc.size() * sizeof(bc[0]));
  }

  template <class Archive>
  void load(Archive& a, ShadersSystem::ByteCodes& bc, const unsigned int version)
  {
  	size_t count = 0;
    a & count;
    bc.reserve(count);
    a & make_binary_object(bc.data(), count * sizeof(bc[0]));
  }

  template <class Archive>
  void serialize(Archive& a, ShadersSystem::ByteCodes& bc, const unsigned int version)
  {
  	split_free(a, bc, version);
  }

  template<class Archive>
  void serialize(Archive& a, vk::VertexInputBindingDescription& vib, const unsigned version)
  {
    a & make_binary_object(&vib, sizeof(vib));
  }

  template<class Archive>
  void serialize(Archive& a, vk::VertexInputAttributeDescription& via, const unsigned version)
  {
    a & make_binary_object(&via, sizeof(via));
  }

  template<class Archive>
  void serialize(Archive& a, spirv::v2::InputAssembly& ia, const unsigned version)
  {
    a & ia.buffers
      & ia.attributes;
  }

  template<class Archive>
  void serialize(Archive& a, ShadersSystem::ShaderBlob& b, const unsigned version)
  {
    a & b.name
      & b.entry
      & b.stage
      & b.data;
  }

  template<class Archive>
  void serialize(Archive& a, spirv::v2::Binding& b, const unsigned version)
  {
    a & b.type
      & b.stages
      & b.name;
  }

  template<class Archive>
  void serialize(Archive& a, spirv::v2::DescriptorSet& dset, const unsigned version)
  {
    a & dset.bindings;
  }

  template<class Archive>
  void serialize(Archive& a, spirv::v2::Reflection& r, const unsigned version)
  {
    a & r.entry
      & r.stage;
      & r.dsets;
  }

  template<class Archive>
  void serialize(Archive& a, ShadersSystem::TechniqueDeclaration& t, const unsigned version)
  {
    a & t.name
      & t.byteCode
      & t.ia
      & t.blobs
      & t.reflections;
  }

  template<class Archive>
  void serialize(Archive& a, ShadersSystem::MaterialsBin::Scope& s, const unsigned version)
  {
    a & s.name
      & s.cbufferReg
      & s.cbufferSize
      & s.byteCode;
  }

  template<class Archive>
  void serialize(Archive &a, ShadersSystem::MaterialsBin& bin, const unsigned version)
  {
    a & bin.techniques
      & bin.scopes;
  }

  template <class Archive, class T>
  void save(Archive& a, const eastl::vector<T>& vec, const unsigned int version)
  {
    const size_t count = vec.size();
    a & count
      & make_binary_object(vec.data(), count * sizeof(vec[0]));
  }

  template <class Archive, class T>
  void load(Archive& a, eastl::vector<T>& vec, const unsigned int version)
  {
    size_t count = 0;
    a & count;
    vec.reserve(count);
    a & make_binary_object(vec.data(), count * sizeof(vec[0]));
  }

  template<class Archive, class T>
  void serialize(Archive& a, eastl::vector<T>& vec, const unsigned version)
  {
    split_free(a, vec, version);
  }
}
