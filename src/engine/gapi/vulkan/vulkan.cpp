#include "vulkan.h"
#include "backend.h"
#include "cmd_encoder.h"
#include "frame_gc.h"
#include "resources.h"
#include "result.h"

#include <engine/gapi/vulkan/cache/pipelines_storage.h>
#include <engine/gapi/vulkan/cache/renderpass_storage.h>
#include <engine/gapi/gapi.h>

#include <memory>

namespace gapi
{
  extern void                  (*gapi_wait_gpu_idle)();
  extern void                  (*gapi_shutdown)();
  extern TextureHandle         (*gapi_get_backbuffer)();
  extern BufferHandler         (*gapi_allocate_buffer)(const size_t size, const int usage, const char* name);
  extern void                  (*gapi_free_buffer)(const BufferHandler buffer);
  extern void                  (*gapi_free_texture)(const TextureHandle texture);
  extern void                  (*gapi_free_sampler)(const SamplerHandler sampler);
  extern void                  (*gapi_free_pipeline_resources)();
  extern void*                 (*gapi_map_buffer)(const BufferHandler buffer, const int flags);
  extern void                  (*gapi_unmap_buffer)(const BufferHandler buffer);
  extern TextureHandle         (*gapi_allocate_texture)(const TextureAllocationDescription& allocDesc);
  extern SamplerHandler        (*gapi_allocate_sampler)(const SamplerAllocationDescription& allocDesc);
  extern void                  (*gapi_acquire_backbuffer)();
  extern ShaderModuleHandler   (*gapi_add_module)(void* blob);
  extern PipelineLayoutHandler (*gapi_add_pipeline_layout)(void* dsets);
  extern gapi::CmdEncoder*     (*gapi_allocate_cmd_encoder)();
  extern Fence*                (*gapi_allocate_fence)();
  extern CmdPromise*           (*gapi_allocate_cmd_promise)();
  extern void                  (*gapi_wait_fence)(Fence* fence);
  extern void                  (*gapi_present_backbuffer_and_finalize_frame)();
  extern uint3                 (*gapi_get_texture_extent)(const TextureHandle);
  extern void                  (*gapi_push_validation_context)(string&&);
  extern void                  (*gapi_pop_validation_context)();
}

namespace gapi::vulkan
{
  static Backend backend;
  static std::unique_ptr<Device> device;
  static FrameGarbageCollector frameGc;
  static PipelinesStorage pipelinesStorage;
  static RenderPassStorage rpStorage;

  void wait_gpu_idle()
  {
    //loginfo("vulkan: wait gpu idle");
    VK_CHECK(device->getDevice().waitIdle());
  }

  void shutdown()
  {
    wait_gpu_idle();
    frameGc.clearAllFrames();
    device.reset();
  }

  auto get_device() -> vk::Device&
  {
    return device->getDevice();
  }

  auto get_backbuffer() -> TextureHandle
  {
    return device->getBackbuffer();
  }

  auto allocate_buffer(const size_t size, const int usage, const char* name) -> BufferHandler
  {
    return device->allocateBuffer(size, usage, name);
  }

  void free_buffer(const BufferHandler buffer)
  {
    device->freeBuffer(buffer);
  }

  void free_texture(const TextureHandle texture)
  {
    device->freeTexture(texture);
  }

  void free_sampler(const SamplerHandler sampler)
  {
    device->freeSampler(sampler);
  }

  void free_pipeline_resources()
  {
    loginfo("vulkan: clearing pipeline resources");
    pipelinesStorage.clear();
  }

  void* map_buffer(const BufferHandler buffer, const int flags)
  {
    return device->mapBuffer(buffer, flags);
  }

  void unmap_buffer(const BufferHandler buffer)
  {
    device->unmapBuffer(buffer);
  }

  auto allocate_texture(const TextureAllocationDescription& allocDesc) -> TextureHandle
  {
    return device->allocateTexture(allocDesc);
  }

  auto allocate_sampler(const SamplerAllocationDescription& allocDesc) -> SamplerHandler
  {
    return device->allocateSampler(allocDesc);
  }

  void acquire_backbuffer()
  {
    auto res = device->getDevice().createFenceUnique({});
    VK_CHECK_RES(res);
    VK_CHECK(device->getDevice().resetFences(1, &res.value.get()));
    vk::Fence f = res.value.get();
    frameGc.addWaitFence(std::move(res.value));
    device->acquireBackbuffer(f);
  }

  auto add_module(void* blob) -> ShaderModuleHandler
  {
    const ShadersSystem::ShaderBlob* b = reinterpret_cast<const ShadersSystem::ShaderBlob*>(blob);
    return pipelinesStorage.addModule(*b);
  }

  auto add_pipeline_layout(void* ds) -> PipelineLayoutHandler
  {
    const eastl::vector<spirv::v2::DescriptorSet>* dsets =
      reinterpret_cast<const eastl::vector<spirv::v2::DescriptorSet>*>(ds);
    return pipelinesStorage.addPipelineLayout(*dsets);
  }

  auto allocate_cmd_encoder() -> gapi::CmdEncoder*
  {
    vk::UniqueCommandPool cmdPool = device->allocateCmdPool();
    return new CmdEncoder(*device, frameGc, rpStorage, pipelinesStorage, std::move(cmdPool));
  }

  auto allocate_fence() -> Fence*
  {
    return device->allocateFence();
  }

  auto allocate_cmd_promise() -> CmdPromise*
  {
    return device->allocateCmdPromise();
  }

  void wait_fence(Fence* fence)
  {
    VulkanFence* f = reinterpret_cast<VulkanFence*>(fence);
    VK_CHECK(device->getDevice().waitForFences(1, &f->fence.get(), true, ~0));
    delete f;
  }

  void present_backbuffer_and_finalize_frame()
  {
    device->presentSurfaceImage();
    frameGc.nextFrame();
  }

  auto get_texture_extent(const TextureHandle h) -> uint3
  {
    return device->getTextureExtent(h);
  }

  TextureHandle texture_2d_srv_stub;
  TextureHandle texture_3d_srv_stub;
  TextureHandle texture_cube_srv_stub;
  BufferHandler buffer_uav_stub;

  auto get_resource_stub(ShadersSystem::ResourceType type) -> ResourceHandler
  {
    switch (type)
    {
      case ShadersSystem::ResourceType::Texture2D:    return (ResourceHandler)texture_2d_srv_stub;
      case ShadersSystem::ResourceType::Texture3D:    return (ResourceHandler)texture_3d_srv_stub;
      case ShadersSystem::ResourceType::TextureCube:  return (ResourceHandler)texture_cube_srv_stub;
      case ShadersSystem::ResourceType::RWTexture2D : return (ResourceHandler)texture_2d_srv_stub;
      case ShadersSystem::ResourceType::RWTexture3D : return (ResourceHandler)texture_3d_srv_stub;
      case ShadersSystem::ResourceType::Buffer:
      case ShadersSystem::ResourceType::RWBuffer:
      case ShadersSystem::ResourceType::RWStructuredBuffer: return (ResourceHandler)buffer_uav_stub;
      default: ASSERT_FMT(false, "unsupported resource type {}", (int)type); return (ResourceHandler)texture_2d_srv_stub;
    }
  }

  void allocate_srv_stubs()
  {
    constexpr size_t imgDepth = 16;
    constexpr size_t rgba8 = 4;
    constexpr size_t side = 2;
    constexpr size_t img2dSize = rgba8 * side * side * 1;
    constexpr size_t img3dSize = rgba8 * side * side * imgDepth;
    constexpr size_t cubeLayers = 6;
    constexpr size_t imgCubeSize = rgba8 * side * side * cubeLayers;

    constexpr uint3 extent2d {side, side, 1};
    constexpr uint3 extent3d {side, side, imgDepth};
    constexpr uint3 extentCube {side, side, imgDepth};

    TextureAllocationDescription allocDesc {
      .format = TextureFormat::R8G8B8A8_UNORM,
      .extent = extent2d,
      .mipLevels = 1,
      .arrayLayers = 1,
      .samplesPerPixel = TextureSamples::s1,
      .usage = TEX_USAGE_SRV
    };
    texture_2d_srv_stub = device->allocateTexture(allocDesc);

    allocDesc.extent = extent3d;
    texture_3d_srv_stub = device->allocateTexture(allocDesc);

    allocDesc.arrayLayers = cubeLayers;
    allocDesc.extent = extent2d;
    allocDesc.usage |= TEX_USAGE_CUBE_MAP;
    texture_cube_srv_stub = device->allocateTexture(allocDesc);

    int imgData[img3dSize]{0};

    BufferTextureCopy copy {
      .bufferOffset = 0,
      .bufferRowLength = 0,
      .bufferTextureHeight = 0,
      .textureSubresource = TextureSubresourceLayers {
        .aspects = ASPECT_COLOR,
        .mipLevel = 0,
        .baseArrayLayer = 0,
        .layerCount = 1
      },
      .textureOffset = uint3{0,0,0},
      .extent = extent2d
    };

    std::unique_ptr<gapi::CmdEncoder> encoder{allocate_cmd_encoder()};

    encoder->transitTextureState(texture_2d_srv_stub, gapi::TextureState::Undefined, gapi::TextureState::TransferDst);
    encoder->transitTextureState(texture_3d_srv_stub, gapi::TextureState::Undefined, gapi::TextureState::TransferDst);
    encoder->transitTextureState(texture_cube_srv_stub, gapi::TextureState::Undefined, gapi::TextureState::TransferDst);

    encoder->copyBufferToTexture(imgData, img2dSize, texture_2d_srv_stub, &copy, 1);
    copy.extent = extent3d;
    encoder->copyBufferToTexture(imgData, img3dSize, texture_3d_srv_stub, &copy, 1);
    copy.textureSubresource.layerCount = cubeLayers;
    copy.extent = extent2d;
    encoder->copyBufferToTexture(imgData, imgCubeSize, texture_cube_srv_stub, &copy, 1);

    encoder->transitTextureState(texture_2d_srv_stub, gapi::TextureState::TransferDst, gapi::TextureState::ShaderRead);
    encoder->transitTextureState(texture_3d_srv_stub, gapi::TextureState::TransferDst, gapi::TextureState::ShaderRead);
    encoder->transitTextureState(texture_cube_srv_stub, gapi::TextureState::TransferDst, gapi::TextureState::ShaderRead);
    encoder->flush();

    uint bufStub = 0;
    buffer_uav_stub = gapi::allocate_buffer(sizeof(bufStub), gapi::BufferUsage::BF_BindUAV | gapi::BufferUsage::BF_GpuVisible, "buf_stub");
    encoder->writeBuffer(buffer_uav_stub, &bufStub, 0, sizeof(bufStub));
    encoder->insertGlobalBufferBarrier(gapi::BufferState::BF_STATE_TRANSFER_DST,
      (gapi::BufferState)(gapi::BufferState::BF_STATE_SRV | gapi::BufferState::BF_STATE_UAV_RW));
  }

  void free_cmd_promise(VulkanCmdPromise* promise)
  {
    frameGc.add(std::move(promise->event));
  }

  void push_validation_context(string&& msg)
  {
    backend.pushDbgContext(std::move(msg));
  }

  void pop_validation_context()
  {
    backend.popDbgContext();
  }

  void init()
  {
    gapi_wait_gpu_idle = wait_gpu_idle;
    gapi_shutdown = shutdown;
    gapi_get_backbuffer = get_backbuffer;
    gapi_allocate_buffer = allocate_buffer;
    gapi_free_buffer = free_buffer;
    gapi_free_texture = free_texture;
    gapi_free_sampler = free_sampler;
    gapi_free_pipeline_resources = free_pipeline_resources;
    gapi_allocate_texture = allocate_texture;
    gapi_allocate_sampler = allocate_sampler;
    gapi_map_buffer = map_buffer;
    gapi_unmap_buffer = unmap_buffer;
    gapi_acquire_backbuffer = acquire_backbuffer;
    gapi_add_module = add_module;
    gapi_add_pipeline_layout = add_pipeline_layout;
    gapi_allocate_cmd_encoder = allocate_cmd_encoder;
    gapi_allocate_fence = allocate_fence;
    gapi_allocate_cmd_promise = allocate_cmd_promise;
    gapi_wait_fence = wait_fence;
    gapi_present_backbuffer_and_finalize_frame = present_backbuffer_and_finalize_frame;
    gapi_get_texture_extent = get_texture_extent;
    gapi_push_validation_context = push_validation_context;
    gapi_pop_validation_context = pop_validation_context;

    backend.init();
    device.reset(backend.createDevice(&frameGc));
    frameGc.init(device.get());
    rpStorage.init(device.get());
    pipelinesStorage.init(device.get());

    allocate_srv_stubs();
  }

}