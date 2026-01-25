#pragma once

#include "../math.h"

namespace NocEngine {

struct CComponent {
};

struct CTransform: CComponent {
  Math::Vector3<float> position{};
  Math::Vector3<float> rotation{};
  Math::Vector3<float> scale{};
};

struct CMeshRenderer: CComponent{
  size_t mesh_handle_id{};
};

struct CBoxShape: CComponent {
  Math::Vector3<float> position{};
  Math::Vector3<float> size{};
};

struct CSphereShape: CComponent {
  float radius{};
};

}
