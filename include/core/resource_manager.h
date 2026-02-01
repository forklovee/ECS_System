#pragma once

#include <type_traits>
#include <string>
#include "resource_container.h"
#include "texture.h"

namespace NocEngine
{
	using FilePath = std::string;

	class ResourceManager
	{
	public:
		static ResourceManager& Get() {
			static ResourceManager instance;
			return instance;
		}

		template<ResourceType T>
		ResourceHandle<T> Load(const FilePath& file_path) {
			return getResourceContainer<T>().Load(file_path);
		}

	private:
		ResourceManager()
			: m_texturesContainer(m_resourcePathPool)
		{};

		template<ResourceType T>
		ResourceContainer<T>& getResourceContainer();

	private:
		StringPool m_resourcePathPool{};
		ResourceContainer<Texture> m_texturesContainer;

	};

	template<>
	inline ResourceContainer<Texture>& ResourceManager::getResourceContainer() {
		return m_texturesContainer;
	}
}