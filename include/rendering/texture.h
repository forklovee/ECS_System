#pragma once

#include "resource.h"
#include "stb_image.h"
#include "glm/vec2.hpp"

namespace NocEngine
{
	// Class representing loaded texture data from external image file
	class Texture: public IResource
	{
	public:
		Texture(const char* file_path)
			: IResource(file_path)
		{};

		~Texture() override {
			Unload();
		};

		Texture(Texture&& other) noexcept
			: IResource(std::move(other)),
				m_size(other.m_size), m_channels(other.m_channels), m_image_ptr(other.m_image_ptr)
		{
			other.m_image_ptr = nullptr;
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

		void Load() override {
			if (IsLoaded()) {
				return;
			}
			stbi_set_flip_vertically_on_load(true);
			m_image_ptr = stbi_load(m_file_path, &m_size.x, &m_size.y, &m_channels, 0);
			if (!m_image_ptr) {
				return;
			}
		}

		void Reload() {
			Unload();
			Load();
		}

		void Unload() override {
			if (!IsLoaded()) {
				return;
			}
			stbi_image_free(m_image_ptr);
			m_image_ptr = nullptr;
		}

		glm::ivec2 GetSize() const {
			return m_size;
		}

		int32_t GetChannels() const {
			return m_channels;
		}

		unsigned char* GetDataPtr() const {
			return m_image_ptr;
		}

	private:
		glm::ivec2 m_size{};
		int32_t m_channels{};
		stbi_uc* m_image_ptr{};
	};

}