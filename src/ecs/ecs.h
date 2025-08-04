#ifndef EXPLORE_ECS_ECS_H_
#define EXPLORE_ECS_ECS_H_

#include <bitset>
#include <vector>

#include "../common.h"

static constexpr u32 MAX_COMPONENTS{32};

// use a bitset to keep track of which components
// an entity has, and also helps keep track of which entities
// a given system is interested in.
typedef std::bitset<MAX_COMPONENTS> Signature;

namespace explore::ecs {
class Component {
    // TODO
};

class Entity {
   private:
    u32 _id;

   public:
    Entity(u32 id) : _id(id) {}

   public:
    u32 get_id() const;
};

class System {
   private:
    Signature _component_signature;
    std::vector<Entity> _entities;

   public:
    System() = default;
    ~System() = default;

    void add_entity(Entity entity);
    void remove_entity(Entity entity);

    const std::vector<Entity> &get_entities() const;
    const Signature &get_signature() const;
};

class Registry {};
}  // namespace explore::ecs

#endif  // EXPLORE_ECS_ECS_H_
