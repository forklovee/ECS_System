#pragma once

#include <cstdint>
#include <string>

namespace NocEngine
{

class Shader
{
public:
    Shader() = delete;
    Shader(const char* vertex_shader_path, const char* fragment_shader_path);
    
    ~Shader();

    Shader(const Shader& other) = delete;
    Shader& operator=(const Shader& other) = delete;

    Shader(Shader&& other) = delete;
    Shader& operator=(Shader&& other) noexcept = delete;

    void Use() const;

private:
    const std::string getShaderSource(const char* shader_file_path) const;

private:
    uint32_t id{};
};

}