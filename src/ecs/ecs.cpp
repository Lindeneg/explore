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
        spdlog::debug("removed entity '{}:{}' from '{}'", entity.get_id(),
                      entity.get_name(), _name);
        _entities.erase(iter, _entities.end());
        return true;
    }
    return false;
}

const std::vector<Entity> &System::get_entities() const { return _entities; }

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
        _free_ids.push_back(id);
        _entity_comp_signatures[id].reset();
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
}

void Registry::kill_entity(Entity entity) {
    _entities_kill_queue.insert(entity);
}

}  // namespace explore::ecs

