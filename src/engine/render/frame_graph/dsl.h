#pragma once

#include "timeline.h"

#include <engine/ecs/macros.h>
#include <engine/events.h>
#include <engine/gapi/resources.h>
#include <engine/render/frame_graph/frame_graph.h>
#include <engine/utils/macros.h>

namespace fg::dsl
{
  //https://ctrpeach.io/posts/cpp20-string-literal-template-parameters/
  template<size_t N>
  struct NameWrapper {
      constexpr NameWrapper(const char (&str)[N]) {
      }
  };

  template <NameWrapper name>
  struct Name {};

  template <NameWrapper name>
  struct NameFrom {};

  template <NameWrapper name>
  struct NameTo {};

  template <NameWrapper name>
  struct ImportFnName {};

  struct NotOptional {};
  
  struct Optional {};

  template<class T>
  struct BlobType {};

  template<class name, class type>
  struct CreateBlob{};

  template<class name, class type>
  struct ReadBlob{};

  template<class name, class type>
  struct ModifyBlob{};

  struct OrderBefore{};

  template <class name, class state, class optional>
  struct ReadOptionalBuffer {};

  template<class name, class state>
  struct ModifyBuffer {};

  struct OrderAfter{};
  
  template<gapi::TextureFormat format>
  struct TextureFormat {};

  struct RelativeSize {};
  
  template<uint3>
  struct AbsSize{};

  template<class T>
  struct Extent{};

  template<uint32_t layers>
  struct ArrayLayers {};

  template<uint32_t levels>
  struct MipLevels {};

  template<gapi::TextureSamples samples>
  struct SamplesPerPixel{};

  template<gapi::TextureUsage usage>
  struct TextureUsage {};

  struct PersistentStorage {};

  template<gapi::BufferState state>
  struct BufferState {};

  template<gapi::BufferUsage usage>
  struct BufferUsage {};

  template<uint32_t size>
  struct BufferSize {};

  template<class ...T>
  struct CreateBuffer {};

  template<gapi::TextureState state>
  struct TextureState {};

  template<class ...T>
  struct CreateTexture{};

  template<class nameFrom, class nameTo>
  struct ImportTextureProducer {};

  template<class nameFrom, class nameTo, class state>
  struct RenameTexture {};

  template <fg::Timeline timeline>
  struct Timeline {};

  template<class name, class state>
  struct ModifyTexture {};

  template <class name, class state, class timeline>
  struct ReadTimelineTexture {};

  template <class name, class state, class optional>
  struct ReadOptionalTexture {};

  template<gapi::LoadOp op>
  struct LoadOp {};

  template<gapi::StoreOp op>
  struct StoreOp {};

  template<gapi::LoadOp op>
  struct DepthLoadOp {};

  template<uint32_t val>
  struct UClearColorValue {};

  template<class name, class loadOp, class storeOp, class clearColor>
  struct Target {};

  template<class name>
  struct Depth {};

  template<class name, class loadOp>
  struct RODepth {};

  template<class name, class loadOp, class storeOp>
  struct RWDepth {};

  template <class ...T>
  struct RenderPass {};

  template<class name>
  struct ExecFunction {};

  #define FG_NODE_DSL() [[clang::annotate("fg_node_dsl")]]
  #define NODE_EXEC() [[clang::annotate("fg_node_exec")]]
}

#define NODE_BEGIN(name)\
  struct FG_NODE_DSL() name\
  {

#define NODE_END() };

// RENDERPASS BEGIN
#define RP_BEGIN() fg::dsl::RenderPass<
#define RP_END() > NAME_WITH_LINE(rp);

#define TARGET_EX(name, loadOp, storeOp, uint32Clear)\
  fg::dsl::Target<fg::dsl::Name<#name>,\
                  fg::dsl::LoadOp<gapi::LoadOp:: loadOp>,\
                  fg::dsl::StoreOp<gapi::StoreOp:: storeOp>,\
                  fg::dsl::UClearColorValue<uint32Clear>>
#define TARGET(name) TARGET_EX(name, Load, Store, 0)
#define TARGET_CLEARED(name, uint32Color) TARGET_EX(name, Clear, Store, uint32Color)

#define DEPTH_RW_EX(name, loadOp, storeOp)\
  fg::dsl::RWDepth<\
    fg::dsl::Name<#name>,\
    fg::dsl::LoadOp<gapi::LoadOp:: loadOp>,\
    fg::dsl::StoreOp<gapi::StoreOp:: storeOp>>
#define DEPTH_RW(name) DEPTH_RW_EX(name, Load, Store)
#define DEPTH_RW_CLEARED(name) DEPTH_RW_EX(name, Clear, Store)

#define DEPTH_RO_EX(name, loadOp)\
  fg::dsl::RODepth<\
    fg::dsl::Name<#name>,\
    fg::dsl::LoadOp<gapi::LoadOp:: loadOp>>
#define DEPTH_RO(name) DEPTH_RO_EX(name, Load)
#define DEPTH_RO_CLEARED(name) DEPTH_RO_EX(name, Clear)

#define DEPTH_EX(name)\
  fg::dsl::Depth<\
    fg::dsl::Name<#name>>

#define DEPTH(name) DEPTH_EX(name)
// RENDERPASS END

#define ORDER_ME_AFTER(name) fg::dsl::OrderAfter name;
#define ORDER_ME_BEFORE(name) fg::dsl::OrderBefore name;

#define BUF_USAGE(usage) gapi::BufferUsage::BF_Bind ## usage
#define BUF_STATE(state) gapi::BufferState::BF_STATE_ ## state

#define CREATE_BUF_EX(name, usage, state, size, Gpu_Cpu) fg::dsl::CreateBuffer<\
  fg::dsl::Name<#name>,\
  fg::dsl::BufferUsage<usage | gapi::BufferUsage:: MACRO_CONCAT(MACRO_CONCAT(BF_,Gpu_Cpu), Visible)>,\
  fg::dsl::BufferSize<size>,\
  fg::dsl::BufferState<state>> NAME_WITH_LINE(createBuffer);

#define CREATE_CPU_BUF(name, usage, state, size) CREATE_BUF_EX(name, usage, state, size, Cpu)
#define CREATE_GPU_BUF(name, usage, state, size) CREATE_BUF_EX(name, usage, state, size, Gpu)
#define MODIFY_BUF(name, state)\
  fg::dsl::ModifyBuffer<\
    fg::dsl::Name<#name>,\
    fg::dsl::BufferState<state>> NAME_WITH_LINE(modifyBuffer);

#define READ_BUF(name, state)\
  fg::dsl::ReadOptionalBuffer<\
    fg::dsl::Name<#name>,\
    fg::dsl::NotOptional,\
    fg::dsl::BufferState<gapi::BufferState::BF_STATE_ ## state>> NAME_WITH_LINE(readBuffer);

#define READ_BUF_OPTIONAL(name, state)\
  fg::dsl::ReadOptionalBuffer<\
    fg::dsl::Name<#name>,\
    fg::dsl::Optional,\
    fg::dsl::BufferState<gapi::BufferState::BF_STATE_ ## state>> NAME_WITH_LINE(readBuffer);

#define TEX_SIZE_RELATIVE() fg::dsl::RelativeSize
#define TEX_SIZE(x,y,z) fg::dsl::AbsSize<uint3(x,y,z)>
#define TEX_MIPS(n) n
#define TEX_ARRAY_LAYERS(n) n
#define TEX_SAMPLES(n) gapi::TextureSamples::s ## n
#define TEX_USAGE(usage) gapi::TextureUsage::TEX_USAGE_ ## usage
#define TEX_USAGE2(usage1, usage2) (gapi::TextureUsage)(TEX_USAGE(usage1) | TEX_USAGE(usage2))
#define TEX_USAGE3(usage1, usage2, usage3) (gapi::TextureUsage)(TEX_USAGE2(usage1, usage2) | TEX_USAGE(usage3))
#define TEX_STATE(state) gapi::TextureState:: state
#define TEX_PERSISTENT() , fg::dsl::PersistentStorage
#define TEX_NOT_PERSISTENT() 

#define CREATE_TEX_EX(name, extent, format, mip_levels, array_layers, samples_per_pixel, usage, state, persistent_storage)\
  fg::dsl::CreateTexture<\
    fg::dsl::Name<#name>,\
    fg::dsl::Extent<extent>,\
    fg::dsl::TextureFormat<gapi::TextureFormat:: format>,\
    fg::dsl::MipLevels<mip_levels>,\
    fg::dsl::ArrayLayers<array_layers>,\
    fg::dsl::SamplesPerPixel<samples_per_pixel>,\
    fg::dsl::TextureUsage<usage>,\
    fg::dsl::TextureState<state>\
    persistent_storage\
  > NAME_WITH_LINE(createTex);

#define CREATE_TEX_2D_EX(name, size, format, mip_levels, usage, state, persisted)\
  CREATE_TEX_EX(name, size, format, TEX_MIPS(mip_levels), TEX_ARRAY_LAYERS(1), TEX_SAMPLES(1), usage, state, persisted())

#define CREATE_TEX_2D(name, size, format, usage, state)\
  CREATE_TEX_2D_EX(name, size, format, TEX_MIPS(1), usage, state, TEX_NOT_PERSISTENT)

#define CREATE_TEX_2D_PERSISTENT(name, size, format, usage, state)\
  CREATE_TEX_2D_EX(name, size, format, TEX_MIPS(1), usage, state, TEX_PERSISTENT)

#define IMPORT_TEX(name, import_fn)\
  fg::dsl::ImportTextureProducer<\
    fg::dsl::Name<#name>,\
    fg::dsl::ImportFnName<#import_fn>\
  > NAME_WITH_LINE(importTexture);

#define MODIFY_TEX(name, state)\
  fg::dsl::ModifyTexture<\
    fg::dsl::Name<#name>,\
    fg::dsl::TextureState<state>\
  > NAME_WITH_LINE(modifyTex);

#define READ_TEX(name, state)\
  fg::dsl::ReadOptionalTexture<\
    fg::dsl::Name<#name>,\
    fg::dsl::TextureState<state>,\
    fg::dsl::NotOptional\
  > NAME_WITH_LINE(readTex);

#define READ_TEX_OPTIONAL(name, state)\
  fg::dsl::ReadOptionalTexture<\
    fg::dsl::Name<#name>,\
    fg::dsl::TextureState<state>,\
    fg::dsl::Optional\
  > NAME_WITH_LINE(readOptTex);

#define READ_TEX_PREV_FRAME(name, state)\
  fg::dsl::ReadTimelineTexture<\
    fg::dsl::Name<#name>,\
    fg::dsl::TextureState<state>,\
    fg::dsl::Timeline<fg::Timeline::Previous>\
  > NAME_WITH_LINE(readPrevFrameTex);
  
#define RENAME_TEX(from, to, state)\
  fg::dsl::RenameTexture<\
    fg::dsl::NameFrom<#from>,\
    fg::dsl::NameTo<#to>,\
    fg::dsl::TextureState<state>\
  > NAME_WITH_LINE(renameTex);

#define CREATE_BLOB(name, type)\
  fg::dsl::CreateBlob<\
    fg::dsl::Name<#name>,\
    fg::dsl::BlobType<type>\
  > NAME_WITH_LINE(createBlob);

#define READ_BLOB(name, type)\
  fg::dsl::ReadBlob<\
    fg::dsl::Name<#name>,\
    fg::dsl::BlobType<type>\
  > NAME_WITH_LINE(readBlob);

#define MODIFY_BLOB(name, type)\
  fg::dsl::ModifyBlob<\
    fg::dsl::Name<#name>,\
    fg::dsl::BlobType<type>\
  > NAME_WITH_LINE(modifyBlob);

#define EXEC(func) fg::dsl::ExecFunction<fg::dsl::Name<#func>> NAME_WITH_LINE(execFunc);