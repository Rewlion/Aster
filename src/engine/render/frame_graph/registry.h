#pragma once

#include "id.h"
#include "resource_request.hpp"

#include <engine/gapi/cmd_encoder.h>
#include <engine/gapi/resources.h>
#include <engine/utils/fixed_stack.hpp>
#include <engine/utils/resizable_vector.hpp>

#include <EASTL/vector.h>
#include <function2/function2.hpp>

#define FG_FILE_DECL __FILE__

namespace fg
{
  class Registry;

  struct exec_function_capacity : fu2::capacity_fixed<64> {};
  using ExecFunction = fu2::function_base<true, true, exec_function_capacity, false, true, void(gapi::CmdEncoder&)>;
  using BuildFunction = fu2::function<ExecFunction(Registry&)>;

  struct TextureImport
  {
    gapi::TextureHandle tex;
    gapi::TextureState initState;
  };
  using TextureProduceFunction = fu2::function<TextureImport() const>;

  class Registry
  {
    friend class Manager;
    friend class ResourceStorage;

    struct ExecState;
    struct NodeInfo;
    struct VirtualResourceInfo;
    struct TextureResource;
    struct BlobResource;
    struct SamplerResource;
    using Resource = std::variant<TextureResource, SamplerResource, BlobResource>;

    struct null_capacity : fu2::capacity_fixed<0> {};
    using BlobConstructorFunction = fu2::function_base<true, true, null_capacity, false, true, void(std::byte*) const>;

    public:
      class RpBuilder
      {
        Registry& m_Registry;
        ExecState& m_State;
        public:
          RpBuilder(Registry& reg, ExecState& state) : m_Registry(reg), m_State(state) {}
          auto addTarget(const TextureRequest, const gapi::LoadOp, const gapi::StoreOp) && -> RpBuilder&&;
          auto addTarget(const char*, const gapi::LoadOp = gapi::LoadOp::Load) && -> RpBuilder&&;
          auto addRODepth(const char*, const gapi::LoadOp = gapi::LoadOp::Load) && -> RpBuilder&&;
          auto addDepth(const TextureRequest, 
                        const gapi::LoadOp depth_load, const gapi::StoreOp depth_store,
                        const gapi::LoadOp stencil_load, const gapi::StoreOp stencil_store) && -> RpBuilder&&;
      };

    public:
      void orderMeBefore(const char* node);
      void orderMeAfter(const char* node);

      auto createTexture(const char* name, const gapi::TextureAllocationDescription&, const gapi::TextureState init_state) -> TextureRequest;
      auto importTextureProducer(const char* name, TextureProduceFunction) -> TextureRequest;
      auto modifyTexture(const char* name, const gapi::TextureState state) -> TextureRequest;
      auto readTexture(const char* name, const gapi::TextureState state) -> TextureRequest;
      auto renameTexture(const char* from, const char* to, const gapi::TextureState state) -> TextureRequest;

      template<class BlobType>
      auto createBlob(const char* name) -> BlobReadWriteRequest<BlobType>;
      template<class BlobType>
      auto modifyBlob(const char* name) -> BlobReadWriteRequest<BlobType>;
      template<class BlobType>
      auto readBlob(const char* name) -> BlobReadRequest<BlobType>;

      auto createSampler(const char* name, const gapi::SamplerAllocationDescription&) -> SamplerRequest;

      auto requestRenderPass() -> RpBuilder;

    private:
      using ModifyCb = fu2::function<void(const virt_res_id_t, NodeInfo&)>;
      using ReadCb = ModifyCb;
      using CreateCb = ModifyCb;
      auto createResourceInternal(const char* name, Resource&&, CreateCb&&) -> virt_res_id_t;
      auto modifyResourceInternal(const char* name, ModifyCb&&) -> virt_res_id_t;
      auto readResourceInternal(const char* name, ReadCb&&) -> virt_res_id_t;

      auto registerNode(const char* name, const char* file, BuildFunction build_cb) -> node_id_t;
      void reset();

    private:
      node_id_t m_CurrentExecNodeId;
      size_t m_TotalBlobsSize = 0;

      Utils::StringMap m_NodesNames;
      Utils::StringMap m_ResourcesNames;

      Utils::ResizableVector<NodeInfo, node_id_t> m_Nodes;
      Utils::ResizableVector<VirtualResourceInfo, virt_res_id_t> m_VirtResources;
      Utils::ResizableVector<Resource, res_id_t> m_Resources;

    private:
      struct ExecState
      {
        struct Texture
        {
          virt_res_id_t virtResId;
          gapi::TextureState state;
        };

        struct RenderTarget
        {
          virt_res_id_t vResId = INVALID_VIRT_RES_ID;
          gapi::LoadOp  loadOp = gapi::LoadOp::DontCare;
          gapi::StoreOp storeOp = gapi::StoreOp::DontCare;;
        };

        struct DepthTarget
        {
          virt_res_id_t vResId = INVALID_VIRT_RES_ID;
          gapi::LoadOp  depthLoadOp = gapi::LoadOp::DontCare;
          gapi::StoreOp depthStoreOp = gapi::StoreOp::DontCare;
          gapi::LoadOp  stencilLoadOp = gapi::LoadOp::DontCare;
          gapi::StoreOp stencilStoreOp = gapi::StoreOp::DontCare;;
        };

        struct RenderPass
        {
          Utils::FixedStack<RenderTarget, gapi::MAX_RENDER_TARGETS> mrt;
          DepthTarget depth;
        };

        eastl::vector<Texture> textureBeginStates;
        RenderPass renderPass;
      };

      struct NodeInfo
      {
        name_id_t nameId;
        const char* fileSrc;

        eastl::vector<virt_res_id_t> modifies;
        eastl::vector<virt_res_id_t> reads;
        eastl::vector<virt_res_id_t> creates;

        eastl::vector<node_id_t> prevNodes;

        BuildFunction build;
        ExecFunction exec;

        ExecState execState;

        void resetResourcesAccess()
        {
          modifies.clear();
          reads.clear();
          creates.clear();
        }
      };

      struct TextureResource
      {
        TextureProduceFunction producer;
        gapi::TextureAllocationDescription allocDesc;
      };

      struct SamplerResource
      {
        gapi::SamplerAllocationDescription allocDesc;
      };

      struct BlobResource
      {
        size_t size;
        size_t bufferStart;
        BlobConstructorFunction constructor;
        blob_id_t blobId;
      };

      struct VirtualResourceInfo
      {
        res_id_t resourceId = INVALID_RES_ID;

        virt_res_id_t clonnedVResId = INVALID_VIRT_RES_ID;

        node_id_t createdBy = INVALID_NODE_ID;
        node_id_t consumedBy = INVALID_NODE_ID;
        eastl::vector<node_id_t> modificationChain;
      };

      class BlobCompileTypeId
      {
        public:
          template<class T>
          static
          auto from() -> blob_id_t;

        private:
          template<class T>
          static
          auto fromInternal() -> blob_id_t;

        private:
          static blob_id_t m_LastId;
      };
  };

  template<class BlobType>
  auto Registry::createBlob(const char* name) -> BlobReadWriteRequest<BlobType>
  {
    const virt_res_id_t vResId = createResourceInternal(name, BlobResource {
      .size = sizeof(BlobType),
      .constructor = [](std::byte* storage) { new (storage) BlobType{}; },
      .blobId = BlobCompileTypeId::from<BlobType>()
    }, [](auto, auto) {});

    return {vResId};
  }

  template<class BlobType>
  auto Registry::modifyBlob(const char* name) -> BlobReadWriteRequest<BlobType>
  {
    return modifyResourceInternal(name, [](auto, auto&){});
  }

  template<class BlobType>
  auto Registry::readBlob(const char* name) -> BlobReadRequest<BlobType>
  {
    return readResourceInternal(name, [](auto, auto&){});
  }

  template<class T>
  auto Registry::BlobCompileTypeId::from() -> blob_id_t
  {
    return fromInternal<typename std::remove_cvref<T>::type>();
  }

  template<class T>
  auto Registry::BlobCompileTypeId::fromInternal() -> blob_id_t
  {
    static blob_id_t id = blob_id_t{m_LastId++};
    return id;
  }
}