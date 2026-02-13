#pragma once

#include <cstdint>
#include <string>
#include <glm/ext/matrix_float4x4.hpp>

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
	void SetUniformInt(const std::string& name, int value) const;
	void SetUniformFloat(const std::string& name, float value) const;
	void SetUniformMat4(const std::string& name, const glm::mat4& mat4_value_ptr) const;

private:
    const std::string getShaderSource(const char* shader_file_path) const;

private:
    uint32_t id{};
};

}