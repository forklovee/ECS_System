#pragma once

#include <cstdint>

namespace NocEngine
{
	class IResource
	{
	public:
		IResource() = default;
		virtual ~IResource() = default;

		IResource(const IResource& other) = delete;
		IResource(IResource&& other) noexcept
			: m_generation(other.m_generation)
		{};

		IResource& operator=(const IResource& other) = delete;
		IResource& operator=(IResource&& other) noexcept {
			m_generation = other.m_generation;
			return *this;
		}

		virtual bool IsLoaded() const = 0;
		virtual void Load(const char* file_path) = 0;
		virtual void Unload() = 0;

		uint32_t GetGeneration() const {
			return m_generation;
		}

	protected:
		uint32_t m_generation{};
	};
}