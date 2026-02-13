#include "shader.h"
#include "window.h"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>


namespace NocEngine
{

Shader::Shader(const char* vertex_shader_path, const char* fragment_shader_path)
{
    uint32_t vertex_shader;
    std::string vertex_source = getShaderSource(vertex_shader_path);
    const char* vertex_source_cstr = vertex_source.c_str();
    
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_source_cstr, NULL);
    glCompileShader(vertex_shader);

    int success;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char vertex_shader_info_log[512];
        glGetShaderInfoLog(vertex_shader, 512, NULL, vertex_shader_info_log);
        std::cerr << "Shader::Shader: Vertex shader compilation failed!:\n" << vertex_shader_info_log << "\n";
    }

    uint32_t fragment_shader;
    std::string fragment_source = getShaderSource(fragment_shader_path);
    const char* fragment_source_cstr = fragment_source.c_str();

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_source_cstr, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char fragment_shader_info_log[512];
        glGetShaderInfoLog(fragment_shader, 512, NULL, fragment_shader_info_log);
        std::cerr << "Shader::Shader: Fragment shader compilation failed!:\n" << fragment_shader_info_log << "\n";
    }

    id = glCreateProgram();
    glAttachShader(id, vertex_shader);
    glAttachShader(id, fragment_shader);
    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success){
        char link_info_log[512];
        glGetProgramInfoLog(id, 512, NULL, link_info_log);
        std::cerr << "Shader::Shader: Shader linking failed!:\n" << link_info_log << "\n";
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

Shader::~Shader()
{
    glDeleteProgram(id);
}

void Shader::Use() const
{
    glUseProgram(id);
}

void Shader::SetUniformInt(const std::string& name, int value) const
{
    uint32_t location = glGetUniformLocation(id, name.c_str());
	glUniform1i(location, value);
}

void Shader::SetUniformFloat(const std::string& name, float value) const
{
    uint32_t location = glGetUniformLocation(id, name.c_str());
    glUniform1f(location, value);
}

void Shader::SetUniformMat4(const std::string& name, const glm::mat4& mat4_ref) const
{
    uint32_t location = glGetUniformLocation(id, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat4_ref));
}

const std::string Shader::getShaderSource(const char* shader_file_path) const
{
    std::ifstream file(shader_file_path);
    if (!file.is_open()){
        std::cerr << "Shader::getShaderSource: File " << shader_file_path << " failed to open!\n";
        return "";
    }

    std::stringstream source{};
    source << file.rdbuf();
    return source.str();
}

}