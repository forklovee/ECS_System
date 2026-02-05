#pragma once

// OpenGL includes
#define GLFW_INCLUDE_NONE
#include <glad/glad.h>

#include <cstdint>

namespace NocEngine
{
	struct GPU_Mesh {
		uint32_t generation{};
		virtual ~GPU_Mesh() = default;
		virtual void Destroy() = 0;
		virtual bool IsValid() const = 0;
		virtual void Draw() const = 0;
	};

	struct OpenGL_Mesh: public GPU_Mesh {
		uint32_t VAO{};
		uint32_t VBO{};
		uint32_t EBO{};
		uint32_t index_count{};

		OpenGL_Mesh()
			: VAO(0), VBO(0), EBO(0)
		{
		}
		OpenGL_Mesh(uint32_t VAO, uint32_t VBO, uint32_t EBO)
			: VAO(0), VBO(0), EBO(0)
		{
		}
		~OpenGL_Mesh() override {
			Destroy();
		}

		void Destroy() {
			if (VAO != 0) {
				glDeleteVertexArrays(1, &VAO);
			}
			if (VBO != 0) {
				glDeleteBuffers(1, &VBO);
			}
			if (EBO != 0) {
				glDeleteBuffers(1, &EBO);
			}
			VAO = 0;
			VBO = 0;
			EBO = 0;
		}

		bool IsValid() const {
			return VAO != 0 && VBO != 0 && EBO != 0;
		}

		void Draw() const {
			if (!IsValid()) return;
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	};
}