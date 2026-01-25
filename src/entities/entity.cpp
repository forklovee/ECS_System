#include "../../include/entities/entity.h"

namespace NocEngine {
    const std::bitset<64> & Entity::GetComponentBits() const {
        return m_componentBits;
    }
}
