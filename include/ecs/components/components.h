#pragma once

#include <glm/vec3.hpp>
#include <cstdint>

#include "resource_handle.h"
#include "texture.h"
#include "mesh.h"

class MeshData;
class Shader;

namespace NocEngine {

struct CComponent {
};

struct CTransform: CComponent {
  glm::vec3 position{};
  glm::vec3 rotation{};
  glm::vec3 scale{};
};

struct CMeshRenderer: CComponent{
	ResourceHandle<MeshData> meshdata_handle;
	ResourceHandle<Texture> texture_handle;
};

struct CBoxShape: CComponent {
  glm::vec3 position{};
  glm::vec3 size{};
};

struct CSphereShape: CComponent {
  float radius{};
};

}
