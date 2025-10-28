#include "registry.h"

#include <spdlog/spdlog.h>

#include "ecs.h"

namespace explore::ecs::registry {

void Registry::update() {
    for (auto entity : _entities_add_queue) {
        add_entity_to_systems(entity);
    }
    _entities_add_queue.clear();
}

Entity Registry::create_entity() { return create_entity(_default_entity_name); }

Entity Registry::create_entity(const std::string_view &name) {
    const u32 entity_id{_entity_count++};
    Entity entity{entity_id};

    _entities_add_queue.insert(entity);

    if (entity_id >= _entity_comp_signatures.size()) {
        _entity_comp_signatures.resize(entity_id + 1);
    }

    spdlog::trace("entity added: name->'{}' id->'{}'", name, entity_id);

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

void Registry::kill_entity() {}

}  // namespace explore::ecs::registry
