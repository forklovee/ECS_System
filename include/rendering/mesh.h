#pragma once

#include <cstdint>
#include <fstream>

namespace NocEngine
{

// Class representing loaded mesh data from external 3d model
class MeshData
{
public:
    MeshData();
    MeshData(const char* file_path);
    ~MeshData();

private:
    void loadObjMesh(std::ifstream& file);

private:
    uint32_t VAO{};
    uint32_t VBO{};
    uint32_t EBO{};
};

}