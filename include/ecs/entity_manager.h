#pragma once

#include <bitset>
#include <cstdint>
#include <vector>
#include <queue>

namespace NocEngine {

using Entity = uint32_t;

template<typename Func>
concept EntityFuncCallback = std::invocable<Func, Entity> &&
                              std::is_void_v<std::invoke_result_t<Func, Entity>>;

class EntityManager {
  friend class ComponentManager;

  public:
    static EntityManager& Get() {
      static EntityManager entity_manager;
      return entity_manager;
    }

    Entity CreateEntity();
    bool IsAlive(Entity entity);
    void KillEntity(Entity entity);

    const std::bitset<64>& GetEntityComponentBitmask(Entity entity) const;

    template<EntityFuncCallback Func>
    void ForEach(Func&& callback);

    template<EntityFuncCallback Func>
    void ForEachWithBitmask(Func&& callback, const std::bitset<64>& bitmask);

  private:
    EntityManager() {
      m_isEntityAlive.reserve(128);
      m_componentBitmasks.reserve(128);
    }

    void SetEntityComponentBit(Entity entity, size_t bit, bool state = true);

  private:
    std::vector<std::bitset<64>> m_componentBitmasks{};
    std::vector<bool> m_isEntityAlive{};
    std::queue<Entity> m_freeIds{};
};

template<EntityFuncCallback Func>
void EntityManager::ForEach(Func&& callback) {
  for (int entityId{}; entityId < m_isEntityAlive.size(); ++entityId){
    if (!IsAlive(entityId)) continue;
    callback(entityId);
  }
}

template<EntityFuncCallback Func>
void EntityManager::ForEachWithBitmask(Func&& callback, const std::bitset<64>& bitmask){
  for (int entityId{}; entityId < m_isEntityAlive.size(); ++entityId){
    if (!IsAlive(entityId)) continue;
    if ((m_componentBitmasks[entityId] & bitmask).none()) continue;
    callback(entityId);
  }
}


} // namespace NocEngine
