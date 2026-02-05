#pragma once

#include <cstdint>

namespace NocEngine
{
	class IResource
	{
	public:
		IResource() = delete;
		IResource(const char* file_path)
			: m_file_path(file_path)
		{};
		virtual ~IResource() = default;

		IResource(const IResource& other) = delete;
		IResource(IResource&& other) noexcept
			: m_file_path(other.m_file_path), m_generation(other.m_generation)
		{
			other.m_file_path = nullptr;
		};

		IResource& operator=(const IResource& other) = delete;
		IResource& operator=(IResource&& other) noexcept {
			m_file_path = other.m_file_path;
			m_generation = other.m_generation;
			other.m_file_path = nullptr;
			return *this;
		}

		virtual bool IsLoaded() const = 0;
		virtual void Load() = 0;
		virtual void Unload() = 0;

		uint32_t GetGeneration() const {
			return m_generation;
		}

	protected:
		const char* m_file_path{ nullptr };
		uint32_t m_generation{};
	};
}