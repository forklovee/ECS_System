#include <iostream>
#include <stdio.h>

#include "math.h"
#include "rendering/shader.h"

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>

#include "components/components.h"
#include "components/component_manager.h"
#include "entities/entity_manager.h"
#include "core/window.h"

using namespace NocEngine;

int main() {
  Window window{{1280, 720}, "ECS Debug"};

  EntityManager& em = EntityManager::Get();
  ComponentManager& cm = ComponentManager::Get();
  
  // Entities and components test creation
  for (size_t i{}; i < 1024; i++) {
    Entity& e = em.CreateEntity();
    cm.CreateComponent<CTransform>(e);

    if (i % 3 == 0){
      cm.CreateComponent<CBoxShape>(e);
    }
    if (i % 2 == 0){
      cm.CreateComponent<CSphereShape>(e);
    }
  }

  // Naive renderer implementation (for now, since systems are yet to be implemented...)
  uint32_t VAO, VBO, EBO;
  Shader base_shader{"../assets/shaders/base_vertex.glsl", "../assets/shaders/base_fragment.glsl"};
  {
    float vertices[] = {
          0.5f,  0.5f, 0.0f,  // top right
          0.5f, -0.5f, 0.0f,  // bottom right
          -0.5f, -0.5f, 0.0f,  // bottom left
          -0.5f,  0.5f, 0.0f   // top left 
    };

    uint32_t indices[] = {
      0, 1, 3,
      1, 2, 3
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Unbind Vertex Array & Buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
  

  while (!window.ShouldClose()) {
    em.Update();

    window.PollEvents();
    window.ClearScreen();

    base_shader.Use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    window.Present();
  }

  const std::vector<Entity*> entities = em.GetAllEntities();
  std::cout << "Created Entities (" << entities.size() << ")\n";
  for (Entity* e: entities) {
    std::cout << "Entity: " << e->GetId() << ", signature: " << e->GetComponentBits() << "\n";
  }

  std::cout << "Window closed.\n";

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}
