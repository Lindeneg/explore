#include "movement.h"

#include "../ecs/components.h"

namespace explore::system {

Movement::Movement() {
    require_component<component::Transform>();
    require_component<component::RigidBody>();
}

void Movement::update(f32 delta_time) {
    for (const auto &entity : get_entities()) {
        auto &transform = entity.get_component<component::Transform>();
        const auto rb{entity.get_component<component::RigidBody>()};

        transform.position += (rb.velocity * delta_time);
    }
}
}  // namespace explore::system
