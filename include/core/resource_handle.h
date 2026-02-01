#pragma once

#include <cstdint>
#include <concepts>
#include "resource.h"

namespace NocEngine {
    template<typename T>
    concept ResourceType = std::is_base_of_v<IResource, T>;

    template<ResourceType T>
        struct ResourceHandle
    {
        ResourceHandle() = delete;
        ResourceHandle(uint32_t id)
            : id(id), generation(0)
        { }
        ResourceHandle(uint32_t id, uint32_t generation)
            : id(id), generation(generation)
        { }
        uint32_t id{};
        uint32_t generation{};
    };
}