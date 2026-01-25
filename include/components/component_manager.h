#pragma once
#include <vector>
#include <typeindex>
#include <memory>
#include <algorithm>
#include <bitset>

#include "component_array.h"
#include "entity.h"

namespace NocEngine{
    class ComponentManager
    {
    public:
        static ComponentManager& Get() {
            static ComponentManager instance;
            return instance;
        }

        template<ValidComponent T>
        T& GetComponent(const Entity& entity) const;

        template<ValidComponent T>
        std::vector<T>& GetComponents() const;

        template<ValidComponent T>
        std::bitset<64> GetComponentBitset() const;

        template<ValidComponent T>
        bool HasComponent(const Entity& entity) const;

        template<ValidComponent T>
        T& CreateComponent(Entity& entity);

        template<ValidComponent T>
        void DestroyComponent(Entity& entity);

        void OnEntityIdUpdated(const Entity& entity, const size_t oldEntityId){
            for (auto& compArr: m_components){
                compArr->OnEntityIdUpdated(entity, oldEntityId);
            }
        }

        void OnEntityDestroyed(const Entity& entity){
            for (auto& compArr: m_components){
                compArr->OnEntityDestroyed(entity);
            }            
        }

    private:
        ComponentManager() {
            m_components.reserve(64);
            m_registered_types.reserve(64);
        }

        template<ValidComponent T>
        size_t GetOrRegisterComponentTypeId();

        template<ValidComponent T>
        ComponentArray<T>* GetComponentArray();

    private:
        std::vector<std::unique_ptr<IComponentArray>> m_components{};
        std::vector<std::type_index> m_registered_types{};
    };

    template<ValidComponent T>
    T& ComponentManager::GetComponent(const Entity& entity) const
    {
        size_t componentTypeId = GetOrRegisterComponentTypeId<T>();
        IComponentArray* basePtr = m_components.at(componentTypeId).get();
        auto* componentArray = static_cast<ComponentArray<T>*>(basePtr);
        return componentArray->GetComponent(entity);
    }

    template<ValidComponent T>
    std::vector<T>& ComponentManager::GetComponents() const
    {
        ComponentArray<T>* arrayPtr = GetComponentArray<T>();
        if (!arrayPtr){
            return {};
        }
        return arrayPtr->GetComponents();
    }

    template<ValidComponent T>
    std::bitset<64> ComponentManager::GetComponentBitset() const
    {
        std::bitset<64> componentBitset{};
        const std::type_index index{typeid(T)};
        auto it = std::find(m_registered_types.begin(), m_registered_types.end(), index);
        if (it == m_registered_types.end()) 
        {
            return componentBitset;
        }
        size_t componentTypeIndex = std::distance(m_registered_types.begin(), it); 
        componentBitset.set(componentTypeIndex);
        return componentBitset;
    }

    template<ValidComponent T>
    bool ComponentManager::HasComponent(const Entity& entity) const
    {
        return (entity.GetComponentBits() & GetComponentBitset<T>()).any();
    }


    template <ValidComponent T>
    T& ComponentManager::CreateComponent(Entity& entity)
    {
        size_t componentTypeId = GetOrRegisterComponentTypeId<T>();

        entity.m_componentBits |= GetComponentBitset<T>();

        IComponentArray* basePtr = m_components.at(componentTypeId).get();
        auto* componentArray = static_cast<ComponentArray<T>*>(basePtr);
        return componentArray->InsertComponent(entity, T{});
    }

    template<ValidComponent T>
    void ComponentManager::DestroyComponent(Entity &entity)
    {
        const std::type_index index{typeid(T)};
        auto it = std::find(m_registered_types.begin(), m_registered_types.end(), index);
        if (it == m_registered_types.end()){ // Component type doesn't exist.
            return;
        }

        entity.m_componentBits &= ~GetComponentBitset<T>();

        size_t componentTypeId = std::distance(m_registered_types.begin(), it);
        IComponentArray* basePtr = m_components.at(componentTypeId).get();
        auto* componentArray = static_cast<ComponentArray<T>*>(basePtr);
        componentArray->RemoveComponent(entity);
    }

    template<ValidComponent T>
    size_t ComponentManager::GetOrRegisterComponentTypeId()
    {
        const std::type_index index{typeid(T)};
        auto it = std::find(m_registered_types.begin(), m_registered_types.end(), index);
        if (it == m_registered_types.end()) // Register Component Type
        {
            m_registered_types.push_back(index);
            m_components.emplace_back(std::make_unique<ComponentArray<T>>()); // Every component needs an array.
            return m_registered_types.size() - 1;
        }
        return std::distance(m_registered_types.begin(), it);
    }

    template<ValidComponent T>
    ComponentArray<T>* ComponentManager::GetComponentArray() {
        const std::type_index index{typeid(T)};
        auto it = std::find(m_registered_types.begin(), m_registered_types.end(), index);
        if (it == m_registered_types.end()){
            std::cerr << "ComponentManager::GetComponentArray: Couldn't find array for specified component.\n";
            return nullptr;
        }
        const size_t arrayIndex(std::distance(m_registered_types.begin(), it));
        return m_components[arrayIndex].get();
    }


} // NocEngine
