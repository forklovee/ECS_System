#pragma once

#include <cstdint>

namespace NocEngine
{
	struct GPU_Texture {
		uint32_t generation{};
		virtual ~GPU_Texture() = default;
		virtual void Destroy() = 0;
		virtual bool IsValid() const = 0;
		virtual void Bind() const = 0;
	};

	struct OpenGL_Texture: public GPU_Texture {
		uint32_t texture_id{};
		~OpenGL_Texture() override {
			Destroy();
		}

		void Destroy() override {
			if (texture_id) {
				glDeleteTextures(1, &texture_id);
			}
			texture_id = 0;
		}

		bool IsValid() const override {
			return texture_id != 0;
		}

		void Bind() const override {
			if (!IsValid()) return;
			glBindTexture(GL_TEXTURE_2D, texture_id);
		}

	};
}