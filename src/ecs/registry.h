#ifndef EXPLORE_ECS_REGISTRY_H_
#define EXPLORE_ECS_REGISTRY_H_

#include <memory>
#include <set>
#include <string_view>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

#include "ecs.h"

namespace explore::ecs::registry {

class Registry {
   private:
    constexpr static const std::string_view default_entity_name = "default";

    u32 _entity_count{0};

    // each pool contains all data for a certain component type
    std::vector<std::shared_ptr<explore::ecs::IPool>> _comp_pools;
    // vector of component signatures, the signature lets us know
    // which components are turned on for an entity
    std::vector<explore::ecs::Signature> _entity_comp_signatures;

    std::set<explore::ecs::Entity> _entities_add_queue;
    std::set<explore::ecs::Entity> _entities_kill_queue;

    std::unordered_map<std::type_index, std::shared_ptr<explore::ecs::System>>
        _systems;

   public:
    Registry() = default;
    ~Registry() = default;

    void update();

    Entity create_entity();
    Entity create_entity(const std::string_view name);

    void add_entity_to_systems(Entity entity);

    void kill_entity();

    template <typename TComponent, typename... TArgs>
    void add_component(Entity entity, TArgs &&...args);

    template <typename TComponent>
    void remove_component(Entity entity);

    template <typename TComponent>
    bool has_component(Entity entity);

    template <typename TComponent>
    TComponent &get_component(Entity entity);

    template <typename TSystem, typename... TArgs>
    void add_system(TArgs &&...args);

    template <typename TSystem>
    bool remove_system();

    template <typename TSystem>
    bool has_system();

    template <typename TSystem>
    TSystem &get_system();
};

template <typename TComponent, typename... TArgs>
void Registry::add_component(Entity entity, TArgs &&...args) {
    const auto component_id{Component<TComponent>::get_id()};
    const auto entity_id{entity.get_id()};

    if (component_id >= _comp_pools.size()) {
        _comp_pools.resize(component_id + 1, nullptr);
    }

    if (!_comp_pools[component_id]) {
        std::shared_ptr<Pool<TComponent>> new_pool =
            std::make_shared<Pool<TComponent>>();
        _comp_pools[component_id] = new_pool;
    }

    std::shared_ptr<Pool<TComponent>> comp_pool{
        std::static_pointer_cast<Pool<TComponent>>(_comp_pools[component_id])};

    if (entity_id >= comp_pool->size()) {
        comp_pool->resize(_entity_count);
    }

    TComponent component{std::forward<TArgs>(args)...};

    comp_pool->set(entity_id, component);
    _entity_comp_signatures[entity_id].set(component_id, true);
}

template <typename TComponent>
void Registry::remove_component(Entity entity) {
    const auto component_id{Component<TComponent>::get_id()};
    const auto entity_id{entity.get_id()};

    _entity_comp_signatures[entity_id].set(component_id, false);
}

template <typename TComponent>
bool Registry::has_component(Entity entity) {
    const auto component_id{Component<TComponent>::get_id()};
    const auto entity_id{entity.get_id()};
    return _entity_comp_signatures[entity_id].test(component_id);
}

template <typename TComponent>
TComponent &Registry::get_component(Entity entity) {
    const auto component_id{Component<TComponent>::get_id()};
    const auto entity_id{entity.get_id()};
    const Pool<TComponent> &pool{_comp_pools[component_id]};
    return pool[entity_id];
}

template <typename TSystem, typename... TArgs>
void Registry::add_system(TArgs &&...args) {
    std::shared_ptr<TSystem> new_system{
        std::make_shared<TSystem>(std::forward<TArgs>(args)...)};
    _systems.insert(
        std::make_pair(std::type_index(typeid(TSystem)), new_system));
}

template <typename TSystem>
bool Registry::remove_system() {
    auto system{_systems.find(std::type_index(typeid(TSystem)))};
    _systems.erase(system);
}

template <typename TSystem>
bool Registry::has_system() {
    return _systems.find(std::type_index(typeid(TSystem))) != _systems.end();
}

template <typename TSystem>
TSystem &Registry::get_system() {
    auto system{_systems.find(std::type_index(typeid(TSystem)))};
    // TODO: what if the system isn't found? maybe reference is wrong here
    // std::optional could be better inside a std::reference_wrapper
    // then return std::nullopt if system is not found
    return *(std::static_pointer_cast<TSystem>(system->second));
}

}  // namespace explore::ecs::registry

#endif  // EXPLORE_ECS_REGISTRY_H_
