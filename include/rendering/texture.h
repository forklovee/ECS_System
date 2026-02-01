#pragma once

#include "resource.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace NocEngine
{
	class Texture: public IResource
	{
	public:
		Texture() = default;
		virtual ~Texture() override {
			Unload();
		};

		Texture(Texture&& other) noexcept
			: IResource(std::move(other)),
				m_size(other.m_size), m_channels(other.m_channels), m_image_ptr(other.m_image_ptr)
		{

		};

		Texture& operator=(Texture&& other) noexcept {
			if (this != &other) {
				Unload();
				IResource::operator=(std::move(other));
				m_size = other.m_size;
				m_channels = other.m_channels;
				m_image_ptr = other.m_image_ptr;
				other.m_image_ptr = nullptr;
			}
			return *this;
		}

		bool IsLoaded() const override {
			return m_image_ptr != nullptr;
		}

		void Load(const char* file_path) override {
			m_image_ptr = stbi_load(file_path, &m_size.x, &m_size.y, &m_channels, 0);
			if (!m_image_ptr) {
				return;
			}
			std::cout << "Texture: " << file_path << " loaded!\n";
		}

		void Unload() override {
			if (!IsLoaded()) {
				return;
			}
			stbi_image_free(m_image_ptr);
			m_image_ptr = nullptr;
			std::cout << "Texture: unloaded!\n";
		}

	private:
		void loadTexture(const char* image_path);

	private:
		glm::i32vec2 m_size{};
		int32_t m_channels{};
		stbi_uc* m_image_ptr{};
	};

}