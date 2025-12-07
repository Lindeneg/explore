#include "ecs.h"

#include <spdlog/spdlog.h>

#include <algorithm>

namespace explore::ecs {

u32 BaseComponent::_next_id{0};

//////////////////////////////////////
//////////////////////////////////////
////////////// ENTITY ////////////////
//////////////////////////////////////
//////////////////////////////////////

Entity::Entity(u32 id, std::string_view name, Registry *registry)
    : _id(id), _name(name), _registry(registry) {}

u32 Entity::get_id() const { return _id; }

const std::string &Entity::get_name() const { return _name; }

Registry *Entity::get_registry() const { return _registry; }

void Entity::kill() { _registry->kill_entity(*this); }

void Entity::set_name(const std::string_view name) { _name = name; }

bool Entity::operator==(const Entity &other) const { return _id == other._id; }

bool Entity::operator!=(const Entity &other) const { return _id != other._id; }

bool Entity::operator<(const Entity &other) const { return _id < other._id; }

bool Entity::operator>(const Entity &other) const { return _id > other._id; }

//////////////////////////////////////
//////////////////////////////////////
////////////// SYSTEM ////////////////
//////////////////////////////////////
//////////////////////////////////////

void System::add_entity(Entity entity) { _entities.push_back(entity); }

bool System::remove_entity(Entity entity) {
    auto iter{
        std::remove_if(_entities.begin(), _entities.end(),
                       [&entity](Entity other) { return entity == other; })};
    if (iter != _entities.end()) {
        spdlog::trace("removed entity '{}:{}' from '{}'", entity.get_id(),
                      entity.get_name(), _name);
        _entities.erase(iter, _entities.end());
        return true;
    }
    return false;
}

const std::vector<Entity> &System::get_entities() const { return _entities; }

std::vector<Entity> &System::get_entities_m() { return _entities; }

const Signature &System::get_comp_signature() const {
    return _component_signature;
}

//////////////////////////////////////
//////////////////////////////////////
///////////// REGISTRY ///////////////
//////////////////////////////////////
//////////////////////////////////////

void Registry::update() {
    for (auto entity : _entities_add_queue) {
        add_entity_to_systems(entity);
    }
    _entities_add_queue.clear();

    for (auto entity : _entities_kill_queue) {
        const u32 id{entity.get_id()};
        remove_entity_from_systems(entity);
        _entity_comp_signatures[id].reset();

        for (auto pool : _comp_pools) {
            pool->remove_entity_from_pool(id);
        }

        _free_ids.push_back(id);

        remove_from_group(entity);
        remove_tag(entity);
    }
    _entities_kill_queue.clear();
}

Entity Registry::create_entity() { return create_entity(default_entity_name); }

Entity Registry::create_entity(const std::string_view entity_name) {
    u32 entity_id{};
    if (_free_ids.empty()) {
        entity_id = _entity_count++;
    } else {
        entity_id = _free_ids.front();
        _free_ids.pop_front();
    }

    Entity entity{entity_id, entity_name, this};

    _entities_add_queue.insert(entity);

    if (entity_id >= _entity_comp_signatures.size()) {
        _entity_comp_signatures.resize(entity_id + 1);
    }

    spdlog::trace("entity added: id->'{}' name->'{}'", entity_id, entity_name);

    return entity;
}

void Entity::add_tag(const std::string &tag) {
    ASSERT_RET_V_MSG(_registry, "registry is null");
    _registry->add_tag(*this, tag);
}

bool Entity::has_tag(const std::string &tag) const {
    ASSERT_RET_MSG(_registry, false, "registry is null");
    return _registry->has_tag(*this, tag);
}

void Entity::remove_tag() {
    ASSERT_RET_V_MSG(_registry, "registry is null");
    _registry->remove_tag(*this);
}

void Entity::add_group(const std::string &group) {
    ASSERT_RET_V_MSG(_registry, "registry is null");
    _registry->add_group(*this, group);
}

bool Entity::has_group(const std::string &group) const {
    ASSERT_RET_MSG(_registry, false, "registry is null");
    return _registry->has_group(*this, group);
}

void Entity::remove_from_group() {
    ASSERT_RET_V_MSG(_registry, "registry is null");
    _registry->remove_from_group(*this);
}

void Registry::add_tag(Entity entity, const std::string &tag) {
    _entity_per_tag.emplace(tag, entity);
    _tag_per_entity.emplace(entity.get_id(), tag);
}

bool Registry::has_tag(Entity entity, const std::string &tag) const {
    if (_tag_per_entity.find(entity.get_id()) == _tag_per_entity.end()) {
        return false;
    }
    return _entity_per_tag.find(tag)->second == entity;
}

Entity Registry::get_by_tag(const std::string &tag) const {
    return _entity_per_tag.at(tag);
}

void Registry::remove_tag(Entity entity) {
    auto tagged_entity{_tag_per_entity.find(entity.get_id())};
    if (tagged_entity != _tag_per_entity.end()) {
        auto tag{tagged_entity->second};
        _entity_per_tag.erase(tag);
        _tag_per_entity.erase(tagged_entity);
    }
}

void Registry::add_group(Entity entity, const std::string &group) {
    _entities_per_group.emplace(group, std::set<Entity>());
    _entities_per_group[group].emplace(entity);
    _group_per_entity.emplace(entity.get_id(), group);
}

bool Registry::has_group(Entity entity, const std::string &group) const {
    auto it{_entities_per_group.find(group)};
    if (it == _entities_per_group.end()) {
        return false;
    }
    const auto &group_entities{it->second};
    return group_entities.find(entity) != group_entities.end();
}

std::vector<Entity> Registry::get_by_group(const std::string &group) const {
    auto it{_entities_per_group.find(group)};
    if (it == _entities_per_group.end()) {
        return std::vector<Entity>();
    }
    const auto &entities_set{it->second};
    return std::vector<Entity>(entities_set.begin(), entities_set.end());
}

void Registry::remove_from_group(Entity entity) {
    auto grouped_entity{_group_per_entity.find(entity.get_id())};
    if (grouped_entity != _group_per_entity.end()) {
        auto group{_entities_per_group.find(grouped_entity->second)};
        if (group != _entities_per_group.end()) {
            auto entity_in_group{group->second.find(entity)};
            if (entity_in_group != group->second.end()) {
                group->second.erase(entity_in_group);
            }
        }
        _group_per_entity.erase(grouped_entity);
    }
}

void Registry::add_entity_to_systems(Entity entity) {
    const auto entity_id{entity.get_id()};
    const auto &entity_comp_signature{_entity_comp_signatures[entity_id]};

    for (auto &system : _systems) {
        const auto &system_comp_signature{system.second->get_comp_signature()};
        bool system_interested_in_entity{
            (entity_comp_signature & system_comp_signature) ==
            system_comp_signature};

        if (system_interested_in_entity) {
            system.second->add_entity(entity);
        }
    }
}

void Registry::remove_entity_from_systems(Entity entity) {
    for (auto system : _systems) {
        system.second->remove_entity(entity);
    }
    spdlog::trace("entity removed: id->'{}' name->'{}'", entity.get_id(),
                  entity.get_name());
}

void Registry::kill_entity(Entity entity) {
    _entities_kill_queue.insert(entity);
}

}  // namespace explore::ecs

