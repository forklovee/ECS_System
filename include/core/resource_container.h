#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>

#include "resource_handle.h"
#include "string_pool.h"

namespace NocEngine {

    template<ResourceType T>
    class ResourceContainer {
    public:
        ResourceContainer() = delete;
        ResourceContainer(StringPool& m_resourcePathPool)
            : m_resourcePathPool(m_resourcePathPool)
        {

        }

        T* Get(const ResourceHandle<T>& handle) const {
            uint32_t slotId{ handle.id };
            uint32_t generation{ handle.generation };
            if (slotId >= m_resources.size() || m_resources[slotId].generation != generation) {
                return nullptr;
            }
            return m_resources[slotId];
        }

        ResourceHandle<T> Load(const std::string& file_path) {
            const char* internedPath = m_resourcePathPool.intern(file_path);
            auto it = m_pathToId.find(internedPath);
            if (it != m_pathToId.end()) {
                uint32_t resourceId{it->second};
                return ResourceHandle<T>{resourceId, m_resources[resourceId].GetGeneration()};
            }

            uint32_t slotId{};
            if (!m_freeIds.empty()) {
                slotId = m_freeIds.back();
                m_freeIds.pop();
            }
            else {
                slotId = m_resources.size();
                m_resources.push_back(T());
            }

            m_resources[slotId].Load(internedPath);
            m_pathToId[internedPath] = slotId;

            return ResourceHandle<T>{slotId, m_resources[slotId].GetGeneration()};
        }

        void Unload(const ResourceHandle<T>& handle) {
            uint32_t slotId{ handle.id };
            uint32_t generation{ handle.generation };
            if (slotId >= m_resources.size() || m_resources[slotId].GetGeneration() != generation) {
                return;
            }

            auto it = m_pathToId.begin();
            for(; it != m_pathToId.end(); ++it) {
                if (it->second == slotId) {
                    break;
                }
            }

            if (it == m_pathToId.end()) {
                std::cerr << "ResourceManager::Unload: Error slot: " << slotId << "\n";
                return;
            }
            m_pathToId.erase(it);

            m_resources[slotId].Unload();
            m_freeIds.push(slotId);
        }

    private:
        std::vector<T> m_resources{};
        std::queue<uint32_t> m_freeIds{};
        std::unordered_map<const char*, uint32_t> m_pathToId{};
        StringPool& m_resourcePathPool;
    };
}
