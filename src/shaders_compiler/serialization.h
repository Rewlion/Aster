#pragma once

#include <boost/serialization/binary_object.hpp>

namespace boost::serialization
{

  template<class Archive>
  void serialize(Archive& a, gapi::VertexInputDescription::Buffer& vib, const unsigned version)
  {
    a & make_binary_object(&vib, sizeof(vib));
  }

  template<class Archive>
  void serialize(Archive& a, gapi::VertexInputDescription::Attribute& via, const unsigned version)
  {
    a & make_binary_object(&via, sizeof(via));
  }

  template<class Archive>
  void serialize(Archive& a, gapi::VertexInputDescription& ia, const unsigned version)
  {
    a & ia.attributes
      & ia.buffers;
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
  void serialize(Archive& a, vk::DescriptorSetLayoutBinding& binding, const unsigned version)
  {
    a & binding.binding
      & binding.descriptorType
      & binding.descriptorCount
      & binding.stageFlags;

    binding.pImmutableSamplers = nullptr;
  }

  template<class Archive>
  void serialize(Archive& a, spirv::v2::DescriptorSetBinding& binding, const unsigned version)
  {
    a & binding.vk
      & binding.resourceType;
  }

  template<class Archive>
  void serialize(Archive& a, tfx::RenderState& st, const unsigned version)
  {
    a & st.ia
      & st.cullMode
      & st.topology
      & make_binary_object(&st.depthStencil, sizeof(st.depthStencil))
      & make_binary_object(&st.blending, sizeof(st.blending));
  }

    template<class Archive>
    void serialize(Archive& a, spirv::v2::Reflection& r, const unsigned version)
    {
      a & r.descriptorSets;
    }

  template<class Archive>
  void serialize(Archive& a, ShadersSystem::TechniqueDeclaration& t, const unsigned version)
  {
    a & t.name
      & t.byteCode
      & t.renderState
      & t.blobs
      & t.dsets;
  }

  template<class Archive>
  void serialize(Archive& a, ShadersSystem::MaterialsBin::Scope& s, const unsigned version)
  {
    a & s.name
      & s.cbufferReg
      & s.cbufferSize
      & s.byteCode;
  }

  template<class Archive, class T>
  void serialize(Archive &a, vk::Flags<T>& flags, const unsigned version)
  {
    a & make_binary_object(&flags, sizeof(flags));
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
    a & count;
    for (size_t i = 0 ; i < count; ++i)
      a & vec[i];
  }

  template <class Archive, class T>
  void load(Archive& a, eastl::vector<T>& vec, const unsigned int version)
  {
    size_t count = 0;
    a & count;
    vec.resize(count);
    for (size_t i = 0 ; i < count; ++i)
      a & vec[i];
  }

  template<class Archive, class T>
  void serialize(Archive& a, eastl::vector<T>& vec, const unsigned version)
  {
    split_free(a, vec, version);
  }

  template <class Archive>
  void save(Archive& a, const eastl::vector<ShadersSystem::ShByteCode>& vec, const unsigned int version)
  {
    const size_t count = vec.size();
    a & count
      & make_binary_object(vec.data(), count * sizeof(vec[0]));
  }

  typedef std::variant<int,float> KK;

  template <class Archive>
  void load(Archive& a, eastl::vector<ShadersSystem::ShByteCode>& vec, const unsigned int version)
  {
    size_t count = 0;
    a & count;
    vec.resize(count);
    a & make_binary_object(vec.data(), count * sizeof(vec[0]));
  }

  template<class Archive>
  void serialize(Archive& a, eastl::vector<ShadersSystem::ShByteCode>& vec, const unsigned version)
  {
    split_free(a, vec, version);
  }
}
