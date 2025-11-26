#include "projectile_emit.h"

#include <SDL_timer.h>

#include "../core/rect.h"
#include "../ecs/components.h"

namespace explore::system {

ProjectileEmit::ProjectileEmit() {
    _name = "ProjectileEmitSystem";

    require_component<component::ProjectileEmitter>();
    require_component<component::Transform>();
}

void ProjectileEmit::update(ecs::Registry &registry) {
    for (auto &entity : get_entities()) {
        auto &emitter{entity.get_component<component::ProjectileEmitter>()};
        const auto &transform{entity.get_component<component::Transform>()};

        if (SDL_GetTicks() - emitter.last_emission_time > emitter.interval) {
            glm::vec2 projectile_position{transform.position};
            if (entity.has_component<component::Sprite>()) {
                const auto sprite{entity.get_component<component::Sprite>()};
                projectile_position.x +=
                    ((sprite.src_rect.w * transform.scale.x) / 2.0);
                projectile_position.y +=
                    ((sprite.src_rect.h * transform.scale.y) / 2.0);
            }

            auto projectile{registry.create_entity()};
            projectile.add_component<component::Transform>(
                projectile_position, glm::vec2(1.0, 1.0), 0.0);
            projectile.add_component<component::RigidBody>(emitter.velocity);
            projectile.add_component<component::Sprite>("bullet-tex", 5u,
                                                        core::rect(0, 0, 4, 4));
            projectile.add_component<component::BoxCollider>(4u, 4u);

            emitter.last_emission_time = SDL_GetTicks();
        }
    }
}
};  // namespace explore::system
