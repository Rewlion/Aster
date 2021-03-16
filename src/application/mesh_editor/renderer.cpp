#include "renderer.h"
#include "camera.h"
#include "scene.h"

#include <engine/utils/mesh_generation.h>
#include <engine/rendering/vulkan/fileutils.h>

namespace
{
  Vulkan::VertexInputDeclaration GetSceneLinesVID()
  {
    Vulkan::VertexInputDeclaration vid;

    vid.AddBindingDescription(0, sizeof(glm::vec3));
    vid.AddAttributeDescription(vk::Format::eR32G32B32Sfloat, 0, 0, 0);

    return vid;
  }

  Vulkan::VertexInputDeclaration GetStaticMeshVID()
  {
    Vulkan::VertexInputDeclaration vid;

    vid.AddBindingDescription(0, sizeof(Editor::StaticMeshVertex));
    vid.AddAttributeDescription(vk::Format::eR32G32B32Sfloat, 0, 0, offsetof(Editor::StaticMeshVertex, position));
    vid.AddAttributeDescription(vk::Format::eR32G32B32Sfloat, 0, 1, offsetof(Editor::StaticMeshVertex, normal));

    return vid;
  }
}

namespace Rendering
{

  SceneRenderer::SceneRenderer(Vulkan::Core* vkCore)
    : m_VkCore(vkCore)
  {
    std::vector<glm::vec3> vertices = Utils::GenerateWirePlane(20, 20);
    m_SceneLines.vertices = m_VkCore->AllocateDeviceBuffer(vertices.data(), vertices.size() * sizeof(glm::vec3), vk::BufferUsageFlagBits::eVertexBuffer);
    m_SceneLines.vertexCount = vertices.size();

    auto sceneLinesVert = m_VkCore->CreateShader(Vulkan::ReadFile("../data/shaders/spirv/mesh_editor/sceneLines.vert.spv"));
    auto sceneLinesFrag = m_VkCore->CreateShader(Vulkan::ReadFile("../data/shaders/spirv/mesh_editor/sceneLines.frag.spv"));
    m_SceneLinesProgram = std::make_unique<Vulkan::ShaderProgram>(*m_VkCore, std::move(sceneLinesVert), std::move(sceneLinesFrag));

    auto staticMeshVert = m_VkCore->CreateShader(Vulkan::ReadFile("../data/shaders/spirv/mesh_editor/static_mesh.vert.spv"));
    auto staticMeshFrag = m_VkCore->CreateShader(Vulkan::ReadFile("../data/shaders/spirv/mesh_editor/static_mesh.frag.spv"));
    m_StaticMeshProgram = std::make_unique<Vulkan::ShaderProgram>(*m_VkCore, std::move(staticMeshVert), std::move(staticMeshFrag));
  }

  void SceneRenderer::Render(Editor::Scene& scene, const Editor::Camera& camera)
  {
    Vulkan::RenderGraph* rg = m_VkCore->BeginFrame();

    rg->AddRenderSubpass()
      //.AddDepthOnlyAttachment("DEPTH_ATTACHMENT")
      .AddExistOutputColorAttachment(BACKBUFFER_RESOURCE_ID)
      .SetRenderCallback([&](Vulkan::FrameContext& ctx) 
    {
      SyncObjects(scene);
      RenderEditorWires(ctx, camera);
      RenderSceneObjects(ctx, camera, scene);
    });
  
    m_VkCore->EndFrame();
  }

  void SceneRenderer::RenderEditorWires(Vulkan::FrameContext& ctx, const Editor::Camera& camera)
  {
    //Vulkan::Pipeline* pipeline = ctx.GetPipeline(*m_SceneLinesProgram, GetSceneLinesVID(), vk::PrimitiveTopology::eLineList, Vulkan::EnableDepthTest);
    Vulkan::Pipeline* pipeline = ctx.GetPipeline(*m_SceneLinesProgram, GetSceneLinesVID(), vk::PrimitiveTopology::eLineList, Vulkan::DisableDepthTest);
    ctx.commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline->GetPipeline());

    struct
    {
      glm::mat4 Projection;
      glm::mat4 View;
    } cameraResource{
        camera.GetProjection(),
        camera.GetView()
    };

    Vulkan::UniformsAccessor* uniforms = ctx.GetUniformsAccessor(*m_SceneLinesProgram);
    uniforms->SetUniformBuffer("Camera", &cameraResource);
    std::vector<vk::DescriptorSet> descriptorSets = uniforms->GetUpdatedDescriptorSets();

    ctx.commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipeline->GetLayout(), 0, descriptorSets.size(), descriptorSets.data(), 0, nullptr);

    vk::DeviceSize offset = 0;
    ctx.commandBuffer.bindVertexBuffers(0, 1, &m_SceneLines.vertices.GetBuffer(), &offset);

    ctx.commandBuffer.draw(m_SceneLines.vertexCount, 1, 0, 0);
  }

  void SceneRenderer::RenderSceneObjects(Vulkan::FrameContext& ctx, const Editor::Camera& camera, Editor::Scene& scene)
  {
    std::vector<Editor::Scene::Object>& objects = scene.GetObjects();

    //Vulkan::Pipeline* pipeline = ctx.GetPipeline(*m_StaticMeshProgram, GetStaticMeshVID(), vk::PrimitiveTopology::eLineList, Vulkan::EnableDepthTest);
    Vulkan::Pipeline* pipeline = ctx.GetPipeline(*m_StaticMeshProgram, GetStaticMeshVID(), vk::PrimitiveTopology::eTriangleList, Vulkan::DisableDepthTest);
    ctx.commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline->GetPipeline());

    struct
    {
      glm::mat4 Projection;
      glm::mat4 View;
      glm::vec3 CameraWorldPosition;
    } cameraResource{
        camera.GetProjection(),
        camera.GetView(),
        camera.GetWorldPosition()
    };

    Vulkan::UniformsAccessor* uniforms = ctx.GetUniformsAccessor(*m_StaticMeshProgram);
    uniforms->SetUniformBuffer("Camera", &cameraResource);
    std::vector<vk::DescriptorSet> descriptorSets = uniforms->GetUpdatedDescriptorSets();

    ctx.commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipeline->GetLayout(), 0, descriptorSets.size(), descriptorSets.data(), 0, nullptr);

    for (const auto& obj : objects)
    {
      auto [vertBuffer, indexBuffer, indexCount] = obj.GetStaticMeshBuffers();

      if (indexCount == 0)
        continue;

      vk::DeviceSize offset = 0;
      ctx.commandBuffer.bindVertexBuffers(0, 1, &vertBuffer, &offset);
      ctx.commandBuffer.bindIndexBuffer(indexBuffer, 0, vk::IndexType::eUint32);
      ctx.commandBuffer.drawIndexed(indexCount, 1, 0, 0, 0);
    }
  }

  void SceneRenderer::SyncObjects(Editor::Scene& scene)
  {
    std::vector<Editor::Scene::Object>& objects = scene.GetObjects();

    for (auto& obj : objects)
    {
      Editor::HalfedgeMesh& mesh = obj.GetMesh();
      if (mesh.IsRebuildRequired())
      {
        std::string errMsg = mesh.RebuildStaticMesh();
        if (errMsg != "")
          std::printf("Failed to render object(id:%lld). Error:%s\n", obj.GetId(), errMsg.c_str());

        const auto& [vertices, indices] = mesh.GetStaticMeshData();
        Vulkan::Buffer vertexBuffer = m_VkCore->AllocateDeviceBuffer(vertices.data(), sizeof(vertices[0]) * vertices.size(), vk::BufferUsageFlagBits::eVertexBuffer);
        Vulkan::Buffer indexBuffer = m_VkCore->AllocateDeviceBuffer(indices.data(), sizeof(indices[0]) * indices.size(), vk::BufferUsageFlagBits::eIndexBuffer);

        obj.UpdateBuffers(std::move(vertexBuffer), std::move(indexBuffer), indices.size());
      }
    }
  }
}