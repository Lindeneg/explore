#ifndef EXPLORE_ECS_ECS_H_
#define EXPLORE_ECS_ECS_H_

#include <spdlog/spdlog.h>

#include <bitset>
#include <deque>
#include <memory>
#include <set>
#include <string_view>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../common.h"

static constexpr u32 MAX_COMPONENTS{32};

namespace explore::event {
class Bus;
}

namespace explore::ecs {

// use a bitset to keep track of which components
// an entity has, and also helps keep track of which entities
// a given system is interested in.
typedef std::bitset<MAX_COMPONENTS> Signature;

struct BaseComponent {
   protected:
    static u32 _next_id;
};

template <typename TComponent>
class Component : public BaseComponent {
   public:
    // assign unique id to a TComponent
    static const u32 get_id() {
        // created once per unique TComponent
        static auto id{_next_id++};
        return id;
    }
};

//////////////////////////////////////
//////////////////////////////////////
////////////// ENTITY ////////////////
//////////////////////////////////////
//////////////////////////////////////

class Entity {
   private:
    u32 _id;
    // TODO: store away from entity and instead just save an index
    std::string _name;
    // TODO: maybe make into reference instead
    class Registry *_registry;

   public:
    Entity(u32 id, std::string_view name, Registry *registry);

   public:
    [[nodiscard]] u32 get_id() const;
    [[nodiscard]] const std::string &get_name() const;
    [[nodiscard]] Registry *get_registry() const;

    void kill();

    void set_name(const std::string_view name);

    bool operator==(const Entity &other) const;
    bool operator!=(const Entity &other) const;
    bool operator<(const Entity &other) const;
    bool operator>(const Entity &other) const;

    void add_tag(const std::string &tag);
    bool has_tag(const std::string &tag) const;
    void remove_tag();

    void add_group(const std::string &group);
    bool has_group(const std::string &group) const;
    void remove_from_group();

    template <typename TComponent, typename... TArgs>
    void add_component(TArgs &&...args);

    template <typename TComponent>
    void remove_component();

    template <typename TComponent>
    TComponent &get_component() const;

    template <typename TComponent>
    bool has_component() const;
};

//////////////////////////////////////
//////////////////////////////////////
////////////// SYSTEM ////////////////
//////////////////////////////////////
//////////////////////////////////////

class System {
   private:
    Signature _component_signature;

   protected:
    std::vector<Entity> _entities;
    std::string _name;

   public:
    System() = default;
    ~System() = default;

    const std::vector<Entity> &get_entities() const;
    std::vector<Entity> &get_entities_m();
    const Signature &get_comp_signature() const;

    template <typename TComponent>
    void require_component();

    virtual void subscribe_to_events(event::Bus &event_bus) {};

    virtual void add_entity(Entity entity);
    virtual bool remove_entity(Entity entity);
};

template <typename TComponent>
void System::require_component() {
    const auto id{Component<TComponent>::get_id()};
    _component_signature.set(id);
}

class IPool {
   public:
    virtual ~IPool() {}
};

template <typename T>
class Pool : public IPool {
   private:
    std::vector<T> _data;

   public:
    Pool(u32 size = 100) { _data.resize(size); }

    virtual ~Pool() = default;

    bool empty() const { return _data.empty(); }

    u32 size() const { return _data.size(); }

    void resize(u32 n) { _data.resize(n); }

    void clear() { _data.clear(); }

    void add(T object) { _data.push_back(object); }

    void set(u32 index, T object) { _data[index] = object; }

    T &get(u32 index) { return _data[index]; }

    T &operator[](u32 index) { return _data[index]; }
};

//////////////////////////////////////
//////////////////////////////////////
///////////// REGISTRY ///////////////
//////////////////////////////////////
//////////////////////////////////////

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

    // TODO be smarter about how we store this, avoid strings
    // entity tags (one tag name per entity for now)
    // USE A HASH INSTEAD
    std::unordered_map<std::string, Entity> _entity_per_tag;
    std::unordered_map<u32, std::string> _tag_per_entity;

    // TODO be smarter about how we store this, avoid strings
    // entity groups (a set of entities per group name)
    // USE A HASH INSTEAD
    std::unordered_map<std::string, std::set<Entity>> _entities_per_group;
    std::unordered_map<u32, std::string> _group_per_entity;

    std::unordered_map<std::type_index, std::shared_ptr<explore::ecs::System>>
        _systems;

    std::deque<u32> _free_ids;

   public:
    Registry() = default;
    ~Registry() = default;

    void update();

    Entity create_entity();
    Entity create_entity(const std::string_view name);

    void add_entity_to_systems(Entity entity);
    void remove_entity_from_systems(Entity entity);

    void kill_entity(Entity entity);

    // tag management (TODO rethink this)
    void add_tag(Entity entity, const std::string &tag);
    bool has_tag(Entity entity, const std::string &tag) const;
    Entity get_by_tag(const std::string &tag) const;
    void remove_tag(Entity entity);

    // group management (TODO rethink this)
    void add_group(Entity entity, const std::string &group);
    bool has_group(Entity entity, const std::string &group) const;
    std::vector<Entity> get_by_group(const std::string &group) const;
    void remove_from_group(Entity entity);

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

    spdlog::trace("added component '{}:{}' to entity '{}:{}'", component_id,
                  typeid(TComponent).name(), entity_id, entity.get_name());
}

template <typename TComponent>
void Registry::remove_component(Entity entity) {
    const auto component_id{Component<TComponent>::get_id()};
    const auto entity_id{entity.get_id()};

    _entity_comp_signatures[entity_id].set(component_id, false);

    spdlog::trace("removed component '{}:{}' from entity '{}:{}'", component_id,
                  typeid(TComponent).name(), entity_id, entity.get_name());
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
    auto pool =
        std::static_pointer_cast<Pool<TComponent>>(_comp_pools[component_id]);
    return pool->get(entity_id);
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

template <typename TComponent, typename... TArgs>
void Entity::add_component(TArgs &&...args) {
    ASSERT_RET_V_MSG(_registry, "registry is null");
    _registry->add_component<TComponent>(*this, std::forward<TArgs>(args)...);
}

template <typename TComponent>
void Entity::remove_component() {
    ASSERT_RET_V_MSG(_registry, "registry is null");
    _registry->remove_component<TComponent>(*this);
}

template <typename TComponent>
TComponent &Entity::get_component() const {
    ASSERT_MSG(_registry, "registry is null");
    return _registry->get_component<TComponent>(*this);
}

template <typename TComponent>
bool Entity::has_component() const {
    ASSERT_RET_MSG(_registry, false, "registry is null");
    return _registry->has_component<TComponent>(*this);
}

}  // namespace explore::ecs

#endif  // EXPLORE_ECS_ECS_H_
