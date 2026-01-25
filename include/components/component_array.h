#pragma once

#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>

#include "entities/entity.h"
#include "components.h"

namespace NocEngine {
    template<typename T>
    concept ValidComponent = std::is_base_of_v<CComponent, T>;

    // Necessary interface to store component arrays in ComponentManager.
    class IComponentArray {
    public:
        virtual ~IComponentArray() = default;
        virtual void OnEntityIdUpdated(const Entity& entity, const size_t oldEntityId) = 0;
        virtual void OnEntityDestroyed(const Entity& entity) = 0;
    };

    template<ValidComponent T>
    class ComponentArray : public IComponentArray {
        typedef size_t EntityId;
        typedef size_t ComponentId;
        static constexpr ComponentId INVALID = std::numeric_limits<ComponentId>::max();
        public:
            bool HasComponent(const Entity& entity) const {
                const size_t entityId = entity.GetId();
                return (entityId < m_entityToComponent.size() && m_entityToComponent[entityId] != INVALID);
            }

            bool HasComponent(const size_t entityId) const {
                return (entityId < m_entityToComponent.size() && m_entityToComponent[entityId] != INVALID);
            }

            T& GetComponent(const Entity& entity) {
                const size_t entityId = entity.GetId();
                const size_t componentId = m_entityToComponent[entityId];
                return m_components[componentId];
            }

            std::vector<T>& GetComponents() const {
                return m_components;
            }

            T& InsertComponent(const Entity& entity, T&& component) {
                const size_t entityId = entity.GetId();
                if (m_entityToComponent.size() <= entityId){
                    m_entityToComponent.resize(entityId+1, INVALID);
                }
                if (HasComponent(entityId)){
                    return GetComponent(entity);
                }

                const size_t componentId = m_components.size();
                m_components.emplace_back(std::forward<T>(component));
                m_entityToComponent[entityId] = componentId;
                return m_components.at(componentId);
            }

            void RemoveComponent(const Entity& entity) {
                const size_t entityId = entity.GetId();
                if (entityId >= m_entityToComponent.size()){
                    std::cerr << "ComponentArray::RemoveComponent: entity does not exist.\n";
                    return;
                }

                const size_t componentId = m_entityToComponent[entityId];
                const size_t lastComponentId = m_components.size() - 1;

                auto it = std::find(m_entityToComponent.begin(), m_entityToComponent.end(), lastComponentId);
                if (it == m_entityToComponent.end()){
                    return;
                }
                const size_t lastComponentEntityId = std::distance(m_entityToComponent.begin(), it);
                if (componentId != lastComponentId){
                    // Swap removed component with last component
                    m_components[componentId] = std::move(m_components[lastComponentId]);
                    m_entityToComponent[lastComponentEntityId] = componentId;
                }
                m_entityToComponent[entityId] = INVALID; // Invalidate entity.
                m_components.pop_back();
            }

            void OnEntityIdUpdated(const Entity& entity, const size_t oldEntityId) override {
                const size_t entityId = entity.GetId();
                if (HasComponent(oldEntityId)){
                    m_entityToComponent[entityId] = m_entityToComponent[oldEntityId];
                    m_entityToComponent[oldEntityId] = INVALID;
                }
            }


            void OnEntityDestroyed(const Entity& entity) override {
                const size_t entityId = entity.GetId();
                if (HasComponent(entity)){
                    RemoveComponent(entity);
                }
            }

        public:
            ComponentArray() {
                m_components.reserve(64);
                m_entityToComponent.reserve(64);
            }

        private:
            std::vector<T> m_components{};
            std::vector<ComponentId> m_entityToComponent{};
    };

}