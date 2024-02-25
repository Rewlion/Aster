#include "assets_manager.h"

#include <engine/algorithm/hash.h>
#include <engine/assert.h>
#include <engine/gapi/cmd_encoder.h>
#include <engine/log.h>
#include <engine/settings.h>
#include <engine/utils/fs.h>

#include <algorithm>
#include <filesystem>
#include <memory>

namespace Engine
{
  AssetsManager assets_manager;

  void AssetsManager::init()
  {
    loadAssetsFromFs();
  }

  void AssetsManager::loadAssetsFromFs()
  {
    std::unique_ptr<gapi::CmdEncoder> encoder{gapi::allocate_cmd_encoder()};

    const ed::Scope& assets = get_app_settings().getScope("assets");
    for (const ed::Scope& asset: assets.getScopes())
    {
      const string_view assetType = asset.getName();

      if (assetType == "texture")
        loadTextureAsset(asset, *encoder);
      else if (assetType == "static_mesh")
        loadStaticMesh(asset, *encoder);
      else if (assetType == "model_asset")
        loadModelAsset(asset);
    }

    std::unique_ptr<gapi::Fence> fence{gapi::allocate_fence()};
    encoder->flush(fence.get());
    fence->wait();
  }

  ModelAsset* AssetsManager::getModel(const string& assetName)
  {
    const string_hash assetNameHash = str_hash(assetName.c_str());

    const auto it = m_ModelAssets.find(assetNameHash);
    if (it != m_ModelAssets.end())
      return &it->second;

    ASSERT(!"asset not found");

    return nullptr;
  }

  bool AssetsManager::getTexture(const string_hash assetUri, TextureAsset& asset)
  {
    const auto it = m_Textures.find(assetUri);
    if (it != m_Textures.end())
    {
      asset = it->second;
      return true;
    }

    ASSERT_FMT(false, "asset {} not found", assetUri);

    return false;
  }

  auto AssetsManager::getTexture(const string& assetUri, TextureAsset& asset) -> bool
  {
    return getTexture(str_hash(assetUri), asset);
  }

  static
  auto get_texture_asset_format(const ed::Scope& asset) -> gapi::TextureFormat
  {
    const string_view format = asset.getVariable<string_view>("format");
    return format == "r16" ? gapi::TextureFormat::R16_UNORM
                           : gapi::TextureFormat::R8G8B8A8_UNORM;
  }

  void AssetsManager::loadTextureAsset(const ed::Scope& asset, gapi::CmdEncoder& encoder)
  {
    const bool isCubeMap = asset.getVariableOr<bool>("cubemap", false);
    const string_view file = asset.getVariable<string_view>("bin");
    const string_view name = asset.getVariable<string_view>("name");
    const gapi::TextureFormat format = get_texture_asset_format(asset);

    loginfo("asset manager: loading texture: {} as {}", file, name);
    const string_hash nameHash = str_hash(name);

    TextureAsset textureAsset = isCubeMap ?
                                  loadCubeMapTexture(file,encoder, format) :
                                  loadTexture(file, encoder, format);

      m_Textures.insert({
        nameHash,
        textureAsset
      });
  }

  auto AssetsManager::uploadStaticMeshToGpu(const UnpackedStaticMesh& unpacked_mesh,
                                            gapi::CmdEncoder& encoder) -> SubmeshStack<GpuSubmesh>
  {
    SubmeshStack<GpuSubmesh> submeshes;

    for (const CpuSubmesh& sm : unpacked_mesh.cpuSubmeshes)
    {
      const auto& [vertices, indices] = sm;

      gapi::BufferAllocationDescription vbAlloc;
      const size_t verticesSize = vertices.size() * sizeof(vertices[0]);
      const size_t indicesSize = indices.size() * sizeof(indices[0]);

      GpuSubmesh gpuSubmesh;
      gpuSubmesh.vertexBuffer = gapi::allocate_buffer(verticesSize, gapi::BF_GpuVisible | gapi::BF_BindVertex);
      gpuSubmesh.indexBuffer  = gapi::allocate_buffer(indicesSize,  gapi::BF_GpuVisible | gapi::BF_BindIndex);

      encoder.writeBuffer(gpuSubmesh.vertexBuffer, vertices.data(), 0, verticesSize);
      encoder.writeBuffer(gpuSubmesh.indexBuffer, indices.data(), 0, indicesSize);
      gpuSubmesh.indexCount = indices.size();

      submeshes.push(std::move(gpuSubmesh));
    }

    return submeshes;
  }

  auto AssetsManager::createBVHForSubmeshes(const UnpackedStaticMesh& unpacked_mesh) -> SubmeshStack<BVH>
  {
    SubmeshStack<BVH> bvhs;
    for (const CpuSubmesh& sm : unpacked_mesh.cpuSubmeshes)
      bvhs.push(BVH{sm});
    
    return bvhs;
  }

  void AssetsManager::loadStaticMesh(const ed::Scope& asset, gapi::CmdEncoder& encoder)
  {
    const string_view file = asset.getVariable<string_view>("bin");
    const string_view name = asset.getVariable<string_view>("name");

    loginfo("asset manager: loading static_mesh: {} as {}", file, name);
    const string_hash nameHash = str_hash(name);

    UnpackedStaticMesh unpackedMesh = loadGltf(file, encoder);
    SubmeshStack<GpuSubmesh> gpuSubmeshes = uploadStaticMeshToGpu(unpackedMesh, encoder);
    SubmeshStack<BVH> submeshesBVH = createBVHForSubmeshes(unpackedMesh);

    m_StaticMeshes.insert({
      nameHash,
      StaticMesh{
        .gpuSubmeshes = std::move(gpuSubmeshes),
        .submeshesBVH = std::move(submeshesBVH)
      }
    });
  }

  void AssetsManager::loadModelAsset(const ed::Scope& asset)
  {
    const string_view name = asset.getVariable<string_view>("name");
    const string_view mesh = asset.getVariable<string_view>("mesh");
    const string_hash meshHash = str_hash(mesh);

    loginfo("asset manager: loading model_asset: {}", name);

    const auto meshIt = m_StaticMeshes.find(meshHash);
    ASSERT(meshIt != m_StaticMeshes.end());

    ModelAsset model;
    model.mesh = &meshIt->second;

    for (const ed::Scope& material: asset.getScopes())
    {
      if (material.getName() != "material")
        continue;
      tfx::Material m = createMaterial(material);
      model.materials.push_back(std::move(m));
    }

    m_ModelAssets.insert({
      str_hash(name),
      std::move(model)
    });
  }

  template<class T>
  static
  auto extract_material_param(const ed::Variable& var) -> tfx::ParamDescription
  {
    return tfx::ParamDescription{
      .name = var.name,
      .value = std::get<T>(var.value)
    };
  }

  template<class T>
  static
  auto push_material_param(tfx::Material& mat, const ed::Variable& var)
  {
    mat.params.push_back(extract_material_param<T>(var));
  }


  tfx::Material AssetsManager::createMaterial(const ed::Scope& mat)
  {
    tfx::Material material;
    material.technique = mat.getAnnotation();

    for (const ed::Variable& var: mat.getVariables())
    {
      switch (var.getValueType())
      {
        case ed::ValueType::Text:
        {
          if (var.annotation == "texture")
          {
            const string_view textureName = std::get<string>(var.value);
            TextureAsset asset;
            bool r = getTexture(str_hash(textureName), asset);
            ASSERT(r != false);

            material.params.push_back(tfx::ParamDescription{
              .name = var.name,
              .value = asset.texture
            });
          }
          else
            logerror("Assets: material '{}' has invalid text parameter '{}' with unsupported annotation `{}`",
              mat.getName(), var.name, var.annotation);
          break;
        }
        case ed::ValueType::Float:
        {
          push_material_param<float>(material, var);
          break;
        }
        case ed::ValueType::Float2:
        {
          push_material_param<float2>(material, var);
          break;
        }
        case ed::ValueType::Float3:
        {
          push_material_param<float3>(material, var);
          break;
        }
        case ed::ValueType::Float4:
        {
          push_material_param<float4>(material, var);
          break;
        }
        case ed::ValueType::Int:
        {
          push_material_param<int>(material, var);
          break;
        }
        case ed::ValueType::Int2:
        {
          push_material_param<int2>(material, var);
          break;
        }
        case ed::ValueType::Int3:
        {
          push_material_param<int3>(material, var);
          break;
        }
        case ed::ValueType::Int4:
        {
          push_material_param<int4>(material, var);
          break;
        }
        default:
        {
          logerror("Assets: material '{}' has parameter '{}' with unsupported type",
            mat.getName(), var.name);
        }
      }
    }

    return material;
  }
}