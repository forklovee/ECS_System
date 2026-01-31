#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/vec2.hpp"

#include <string>

namespace NocEngine {

class Window {
public:
  Window();
  Window(glm::vec2 size, const char *title);
  ~Window();

  Window(const Window &other) = delete;
  Window &operator=(const Window &other) = delete;
  Window(Window &&other) = delete;
  Window &operator=(Window &&other) = delete;

  void ClearScreen() const;
  void Present() const;
  void PollEvents() const;

  bool ShouldClose() const { return glfwWindowShouldClose(m_window); };
  GLFWwindow* GetHandle() const { return m_window; };

private:
  void init();

  static void error_callback(int error, const char *description);
  static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
  static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

private:
  GLFWwindow *m_window;
  std::string m_title{};
  glm::vec2 m_size{};
};

} // namespace NocEngine
