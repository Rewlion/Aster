#pragma once

#include "registry.h"
#include "resource_storage.h"
#include "timeline.h"

#include <engine/render/consts.h>

namespace fg
{
  class Manager
  {
    public:
      void setRenderSize(const uint2);

      void registerNode(const char* name, const char* file, BuildFunction build_cb);

      void execNewFrame();
      void setClosingNode(const char* node) { m_ClosingNode = node; }

      auto getTexture(const virt_res_id_t, const Timeline = Timeline::Current) -> gapi::TextureHandle;
      auto getTextureAllocDescription(const virt_res_id_t) -> const gapi::TextureAllocationDescription&;

      auto getSampler(const virt_res_id_t) -> gapi::SamplerHandler;
      auto getSamplerAllocDescription(const virt_res_id_t) -> const gapi::SamplerAllocationDescription&;

      auto getBuffer(const virt_res_id_t) -> gapi::BufferHandler;
      auto getBufferAllocDescription(const virt_res_id_t) -> const gapi::BufferAllocationDescription&;

      auto getBlob(const virt_res_id_t) -> std::byte*;

      void imGuiDrawFramegraph();
      auto getCurFrameTexture(const char* name) -> gapi::TextureViewWithState;

    private:
      auto getResourceId(const virt_res_id_t) -> res_id_t;
      auto getStorage(const Timeline = Timeline::Current) -> ResourceStorage&;
      void compile();
      void validateResources();
      void orderNodes();
      void placeBlobs();
      void flushResources();
      void execNodes();

    private:
      enum class State
      {
        Empty,
        ExecReady
      };

      Registry m_Registry;
      ResourceStorage m_ResourceStorages[Engine::Render::FRAMES_COUNT];
      PersistentResourceStorage m_PersistentResourceStorage;
      uint64_t m_iFrame = 0;

      const char* m_ClosingNode = nullptr;

      State m_State = State::Empty;
      eastl::vector<node_id_t> m_NodesOrder;
      size_t m_BlobsSize = 0;

      gapi::TextureAllocationDescription m_DefaultTexAllocDesc;
      gapi::SamplerAllocationDescription m_DefaultSamplerAllocDesc;
      gapi::BufferAllocationDescription m_DefaultBufferAllocDesc;
  };
}
