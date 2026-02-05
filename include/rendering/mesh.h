#pragma once

#include <cstdint>
#include <vector>

namespace NocEngine
{

// Struct representing loaded mesh data from external 3d model
class MeshData : public IResource
{
public:
    MeshData(const char* file_path)
        : IResource(file_path)
    {};

    bool IsLoaded() const override {
		return !vertices.empty();
    }
    void Load() override {
        // Temporary, just for testing purposes
        // Plane Mesh
        vertices = {
          0.5f,  0.5f, 0.0f,     1.f, 1.f, // top right
          0.5f, -0.5f, 0.0f,     1.f, 0.f, // bottom right
          -0.5f, -0.5f, 0.0f,    0.f, 0.f, // bottom left
          -0.5f,  0.5f, 0.0f,    0.f, 1.f  // top left 
        };

        indices = {
        0, 1, 3,
        1, 2, 3
        };

        use_indices = true;
    }

    void Unload() override {
        vertices.clear();
        indices.clear();
		use_indices = false;
    }

    std::vector<float> vertices{};
    std::vector<uint32_t> indices{};
    bool use_indices{};
};

}