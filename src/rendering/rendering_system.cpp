#include "rendering_system.h"
#include "shader.h"

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>

#include <bitset>

#include "resource_manager.h"

#include "component_manager.h"
#include "components.h"
#include "entity_manager.h"
#include "mesh.h"
#include "texture.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <chrono>
#include <camera_component.h>

namespace NocEngine
{
    RenderingSystem::RenderingSystem()
        : m_activeCamera(EntityManager::Get().CreateEntity())
    {
		CTransform& transformComponent = ComponentManager::Get().CreateComponent<CTransform>(m_activeCamera);
		transformComponent.position = glm::vec3(-5.f, 5.f, -30.f);
        CCamera& cameraComponent = ComponentManager::Get().CreateComponent<CCamera>(m_activeCamera);
        
        glEnable(GL_DEPTH_TEST);
    }


    void RenderingSystem::Update()
    {
        auto start = std::chrono::high_resolution_clock::now();

        if (m_activeCamera == INVALID_ENTITY) {
            std::cerr << "RenderingSystem: No Active Camera Set!\n";
            return;
        }

        m_baseShader.Use();
        m_baseShader.SetUniformInt("albedoTexture", 0);

        CTransform& cameraTransform = ComponentManager::Get().GetComponent<CTransform>(m_activeCamera);
        glm::mat4 view = getMatrixFromCTransform(cameraTransform);

        CCamera& cameraProperties = ComponentManager::Get().GetComponent<CCamera>(m_activeCamera);
        glm::mat4 projection{};
        switch (cameraProperties.projection_type) {
            case CCamera::ProjectionType::Perspective:
                projection = glm::perspective(
                    glm::radians(cameraProperties.projection_fov),
                    1280 / 720.f,
                    0.1f, 100.f);
                break;
            case CCamera::ProjectionType::Orthographic:
                projection = glm::ortho(
                    cameraProperties.ortho_left, cameraProperties.ortho_right,
                    cameraProperties.ortho_down, cameraProperties.ortho_top,
                    cameraProperties.near_plane, cameraProperties.far_plane);
                break;
        }

        m_baseShader.SetUniformMat4("view", view);
        m_baseShader.SetUniformMat4("projection", projection);

        auto before_render = std::chrono::high_resolution_clock::now();

        EntityManager::Get().ForEachWithBitmask(
            [this](Entity entity) {
                renderEntity(entity);
            },
            getRequiredBitmask());

        auto end = std::chrono::high_resolution_clock::now();

        auto setup_time = std::chrono::duration_cast<std::chrono::milliseconds>(before_render - start).count();
        auto render_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - before_render).count();

        std::cout << "Setup: " << setup_time << "ms, Render: " << render_time << "ms, Total: "
            << (setup_time + render_time) << "ms (" << (1000.0f / (setup_time + render_time)) << " FPS)\n";
    }

    const std::bitset<64> RenderingSystem::GetRenderableEnityBitmask() const
    {
        ComponentManager& cm = ComponentManager::Get();
        return cm.GetComponentBitset<CMeshRenderer>() | cm.GetComponentBitset<CTransform>();
    }

    const Entity RenderingSystem::GetActiveCameraEntity() const
    {
        return m_activeCamera;
    }

    CCamera& RenderingSystem::GetActiveCameraComponent() const
    {
        assert(m_activeCamera != INVALID_ENTITY && "Active Camera Not Set!");
        return ComponentManager::Get().GetComponent<CCamera>(m_activeCamera);
    }

    const std::bitset<64> RenderingSystem::getRequiredBitmask() const
    {
        std::bitset<64> requiredBitmask{};
        ComponentManager& cm = ComponentManager::Get();
        requiredBitmask |= cm.GetComponentBitset<CTransform>();
        requiredBitmask |= cm.GetComponentBitset<CMeshRenderer>();
        return requiredBitmask;
    }

    void RenderingSystem::renderEntity(Entity entity)
    {
        ComponentManager& cm = ComponentManager::Get();
        CTransform& transform = cm.GetComponent<CTransform>(entity);
        CMeshRenderer& meshRenderer = cm.GetComponent<CMeshRenderer>(entity);

        if (!meshRenderer.gpu_mesh)
	        meshRenderer.gpu_mesh = load_OpenGLMesh(meshRenderer.meshdata_handle);
	    if (!meshRenderer.gpu_texture)
            meshRenderer.gpu_texture = load_OpenGLTexture(meshRenderer.texture_handle);

        if (!meshRenderer.gpu_mesh) return; // Mesh is needed for rendering. Texture is optional.

		glm::mat4 model = getMatrixFromCTransform(transform);

        m_baseShader.SetUniformMat4("model", model);

        if (meshRenderer.gpu_texture) {
            meshRenderer.gpu_texture->Bind();
        }

        meshRenderer.gpu_mesh->Draw();
    }

    GPU_Mesh* RenderingSystem::getGPUMesh(const ResourceHandle<MeshData>& meshdata_handle)
    {
        auto it = m_gpuMeshes.find(meshdata_handle.id);
        if (it == m_gpuMeshes.end()) return nullptr;
	    if (it->second->generation == meshdata_handle.generation) return it->second.get();
        return nullptr;
    }

    GPU_Mesh* RenderingSystem::load_OpenGLMesh(const ResourceHandle<MeshData>& meshdata_handle)
    {
        if (GPU_Mesh* gpu_mesh = getGPUMesh(meshdata_handle)){
            return gpu_mesh;
        }

        // If already exists, unload old texture
        uint32_t meshdata_id = meshdata_handle.id;
        if (m_gpuMeshes.contains(meshdata_id)) {
            m_gpuMeshes[meshdata_id]->Destroy();
        }

        MeshData* mesh_data = ResourceManager::Get().Get<MeshData>(meshdata_handle);
        if (!mesh_data) {
            return nullptr;
        }

        auto gpu_mesh = std::make_unique<OpenGL_Mesh>();
	    gpu_mesh->generation = meshdata_handle.generation;

        glGenVertexArrays(1, &gpu_mesh->VAO);
        glBindVertexArray(gpu_mesh->VAO);

        glGenBuffers(1, &gpu_mesh->VBO);
        glBindBuffer(GL_ARRAY_BUFFER, gpu_mesh->VBO);
        glBufferData(GL_ARRAY_BUFFER, 
            mesh_data->vertices.size() * sizeof(float),
            mesh_data->vertices.data(),
            GL_STATIC_DRAW);

        // vertex positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // vertex texture coords
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);

        if (mesh_data->use_indices) {
            glGenBuffers(1, &gpu_mesh->EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gpu_mesh->EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                mesh_data->indices.size() * sizeof(uint32_t),
                mesh_data->indices.data(),
                GL_STATIC_DRAW);
            gpu_mesh->index_count = mesh_data->indices.size();
        }
        else {
		    gpu_mesh->index_count = mesh_data->vertices.size() / 5; // 5 floats per vertex (3 for position, 2 for tex coords)
        }

        // Unbind Vertex Array & Buffers
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	    m_gpuMeshes[meshdata_id] = std::move(gpu_mesh);
	    return m_gpuMeshes[meshdata_id].get();
    }

    GPU_Texture* RenderingSystem::getGPUTexture(const ResourceHandle<Texture>& texture_handle)
    {
        auto it = m_gpuTextures.find(texture_handle.id);
        if (it == m_gpuTextures.end()) return nullptr;
        if (it->second->generation == texture_handle.generation) return it->second.get();
        return nullptr;
    }

    GPU_Texture* RenderingSystem::load_OpenGLTexture(const ResourceHandle<Texture>& texture_handle)
    {
        if (GPU_Texture* gpu_texture = getGPUTexture(texture_handle)) {
            return gpu_texture;
        }

	    // If already exists, unload old texture
	    uint32_t texture_id = texture_handle.id;
        if (m_gpuTextures.contains(texture_id)) {
		    m_gpuTextures[texture_id]->Destroy();
        }

	    Texture* texture = ResourceManager::Get().Get<Texture>(texture_handle);
        if (!texture) {
            return nullptr;
        }

        if (texture->GetSize().length() == 0.f)
	        texture->Reload(); // Ensure texture is loaded, has data, size, etc.

	    auto gpu_texture = std::make_unique<OpenGL_Texture>();
	    gpu_texture->generation = texture_handle.generation;

	    glGenTextures(1, &gpu_texture->texture_id);
	    glBindTexture(GL_TEXTURE_2D, gpu_texture->texture_id);

	    GLint format = texture->GetChannels() == 4 ? GL_RGBA : GL_RGB;
	    glTexImage2D(GL_TEXTURE_2D, 0, format, 
            texture->GetSize().x, texture->GetSize().y, 0, format, GL_UNSIGNED_BYTE,
            texture->GetDataPtr());

	    texture->Unload(); // Free CPU memory after uploading to GPU

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
        glBindTexture(GL_TEXTURE_2D, 0);

        m_gpuTextures[texture_id] = std::move(gpu_texture);
	    return m_gpuTextures[texture_id].get();
    }

    glm::mat4 RenderingSystem::getMatrixFromCTransform(const CTransform& transform_component) const
    {
        glm::mat4 matrix = glm::mat4(1.0f);
        matrix = glm::translate(matrix, transform_component.position);
        matrix = glm::rotate(matrix, transform_component.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        matrix = glm::rotate(matrix, transform_component.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        matrix = glm::rotate(matrix, transform_component.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        matrix = glm::scale(matrix, transform_component.scale);
        return matrix;
    }

}