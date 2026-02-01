#include <iostream>
#include <stdio.h>

#include "ecs/components/components.h"
#include "ecs/component_manager.h"
#include "ecs/entity_manager.h"
#include "core/window.h"

#include "mesh.h"
#include "rendering_system.h"
#include "resource_manager.h"
#include "shader.h"

using namespace NocEngine;

int main() {
  Window window{{1280, 720}, "ECS Debug"};
  RenderingSystem renderingSystem;

  ResourceManager& resource_manager = ResourceManager::Get();
  resource_manager.Load<Texture>("../assets/images/example.jpg");

  EntityManager& em = EntityManager::Get();
  ComponentManager& cm = ComponentManager::Get();
  
  // Entities and components test creation
  for (size_t i{}; i < 1; i++) {
    Entity e = em.CreateEntity();
    cm.CreateComponent<CTransform>(e);

    if (i % 3 == 0){
      cm.CreateComponent<CBoxShape>(e);
    }
    if (i % 2 == 0){
      cm.CreateComponent<CSphereShape>(e);
    }
  }

  Shader base_shader{"../assets/shaders/base_vertex.glsl", "../assets/shaders/base_fragment.glsl"};
  MeshData mesh;

  Entity e = em.CreateEntity();
  cm.CreateComponent<CTransform>(e);
  CMeshRenderer& mr = cm.CreateComponent<CMeshRenderer>(e);
  mr.mesh_ptr = &mesh;
  mr.shader_ptr = &base_shader;

  while (!window.ShouldClose()) {
    window.PollEvents();
    window.ClearScreen();

    renderingSystem.Update();

    window.Present();
  }

  em.ForEach([em](Entity entity){
    std::cout << "Entity: " << entity << " components: " << em.GetEntityComponentBitmask(entity) << "\n";
  });

  std::cout << "Window closed.\n";
}
