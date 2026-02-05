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

namespace NocEngine
{
    void RenderingSystem::Update()
{
    EntityManager::Get().ForEachWithBitmask(
        [this](Entity entity) {
            renderEntity(entity);
        },
        getRequiredBitmask());
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

    GPU_Mesh* mesh = load_OpenGLMesh(meshRenderer.meshdata_handle);
	GPU_Texture* texture = load_OpenGLTexture(meshRenderer.texture_handle);

    if (!mesh) return; // Mesh is needed for rendering. Texture is optional.

    m_baseShader.Use();
	m_baseShader.SetUniformInt("albedoTexture", 0);
    if (texture) {
        texture->Bind();
    }

	mesh->Draw();
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

    glGenBuffers(1, &gpu_mesh->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gpu_mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
        mesh_data->indices.size() * sizeof(uint32_t),
        mesh_data->indices.data(),
        GL_STATIC_DRAW);
	gpu_mesh->index_count = mesh_data->indices.size();

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

}