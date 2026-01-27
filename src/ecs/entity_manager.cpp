#include "entity_manager.h"
#include "component_manager.h"
#include <iostream>

namespace NocEngine {

Entity EntityManager::CreateEntity() {
    Entity newEntityId{};
    if (!m_freeIds.empty()){
        newEntityId = m_freeIds.front();
        m_freeIds.pop();
        m_isEntityAlive[newEntityId] = true;
        m_componentBitmasks[newEntityId].reset();
    }
    else{
        newEntityId = m_isEntityAlive.size();
        m_isEntityAlive.push_back(true);
        m_componentBitmasks.push_back({});
    }
    return newEntityId;
}

bool EntityManager::IsAlive(Entity entity){
    return entity >= m_isEntityAlive.size() || m_isEntityAlive[entity];
}

void EntityManager::KillEntity(Entity entity) {
    if (entity >= m_isEntityAlive.size()){
        std::cerr << "EntityManager::KillEntity: Entity " << entity << " does not exist.\n";
        return;
    }
    m_isEntityAlive[entity] = false;
    m_componentBitmasks[entity].reset();
    ComponentManager::Get().OnEntityDestroyed(entity);
}

const std::bitset<64>& EntityManager::GetEntityComponentBitmask(Entity entity) const{
    return m_componentBitmasks[entity];
}

void EntityManager::SetEntityComponentBit(Entity entity, size_t bit, bool state)
{
    if (!IsAlive(entity)) {
        std::cerr << "EntityManager::SetEntityComponentBit: Entity " << entity << " is dead.\n";
        return;
    }
    m_componentBitmasks[entity].set(bit, state);
}


}