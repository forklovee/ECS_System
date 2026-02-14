#include <iostream>
#include <stdio.h>

#include "ecs/components/components.h"
#include "ecs/component_manager.h"
#include "ecs/entity_manager.h"
#include "core/window.h"

#include "mesh.h"
#include "rendering_system.h"
#include "resource_manager.h"

using namespace NocEngine;

int main() {
  Window window{{1280, 720}, "ECS Debug"};
  RenderingSystem renderingSystem;

  ResourceManager& resource_manager = ResourceManager::Get();

  EntityManager& em = EntityManager::Get();
  ComponentManager& cm = ComponentManager::Get();
  
  // Entities and components creation test
  for (size_t i{}; i < 500; i++) {
    Entity e = em.CreateEntity();
    CTransform& tc = cm.CreateComponent<CTransform>(e);

    CMeshRenderer& mr = cm.CreateComponent<CMeshRenderer>(e);
    mr.meshdata_handle = resource_manager.Load<NocEngine::MeshData>("");
    mr.texture_handle = resource_manager.Load<Texture>("assets/images/example.jpg");

    if (i % 3 == 0){
      cm.CreateComponent<CBoxShape>(e);
    }
    if (i % 2 == 0){
      cm.CreateComponent<CSphereShape>(e);
    }
  }

  auto entity_transform_lambda = [&](Entity e) {
      const uint8_t cols{ 24 };
      const uint8_t row{ e % cols };

      CTransform& tc = cm.GetComponent<CTransform>(e);

      tc.position.x = -static_cast<float>(cols)*.5f + (row * 1.5f);
      tc.position.y = 8.0 - ((e / cols) * 1.5f);
      tc.rotation += glm::vec3{ 0.01f, 0.01f, 0.01f };
  };

  std::bitset<64> renderableMeshesBitmask{ renderingSystem.GetRenderableEnityBitmask() };

  while (!window.ShouldClose()) {
    window.PollEvents();
    window.ClearScreen();

    em.ForEachWithBitmask(
        entity_transform_lambda,
        renderableMeshesBitmask
    );

    renderingSystem.Update();

    window.Present();
  }

  em.ForEach([em](Entity entity){
    std::cout << "Entity: " << entity << " components: " << em.GetEntityComponentBitmask(entity) << "\n";
  });

  std::cout << "Window closed.\n";
}
