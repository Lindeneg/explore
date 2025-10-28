#include "ecs.h"

#include <spdlog/spdlog.h>

#include <algorithm>

namespace explore::ecs {

u32 BaseComponent::_next_id{0};

// ENTITY IMPLEMENTATION //

u32 Entity::get_id() const { return _id; }

const std::string &Entity::get_name() const { return _name; }

void Entity::set_name(const std::string_view name) { _name = name; }

bool Entity::operator==(const Entity &other) const { return _id == other._id; }

bool Entity::operator!=(const Entity &other) const { return _id != other._id; }

bool Entity::operator<(const Entity &other) const { return _id < other._id; }

bool Entity::operator>(const Entity &other) const { return _id > other._id; }

// SYSTEM IMPLEMENTATION //

void System::add_entity(Entity entity) { _entities.push_back(entity); }

bool System::remove_entity(Entity entity) {
    auto iter{
        std::remove_if(_entities.begin(), _entities.end(),
                       [&entity](Entity other) { return entity == other; })};
    if (iter == _entities.end()) {
        spdlog::warn("tried to remove non-existent entity {0:d} from registry",
                     entity.get_id());
        return false;
    }
    _entities.erase(iter, _entities.end());
    return true;
}

const std::vector<Entity> &System::get_entities() const { return _entities; }

const Signature &System::get_comp_signature() const {
    return _component_signature;
}

}  // namespace explore::ecs

