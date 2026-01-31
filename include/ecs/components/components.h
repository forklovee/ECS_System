#pragma once

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <cstdint>

namespace NocEngine {

struct CComponent {
};

struct CTransform: CComponent {
  glm::vec3 position{};
  glm::vec3 rotation{};
  glm::vec3 scale{};
};

struct CMeshRenderer: CComponent{
  struct MeshData* mesh_ptr;
  class Shader* shader_ptr;

  uint32_t VAO{};
  uint32_t VBO{};
  uint32_t EBO{};

  // :(
  // No separation of systems and data
  // I know... I feel bad about this, but will fix it later!!!
  ~CMeshRenderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
  }
};

struct CBoxShape: CComponent {
  glm::vec3 position{};
  glm::vec3 size{};
};

struct CSphereShape: CComponent {
  float radius{};
};

}
