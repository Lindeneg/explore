#include "projectile_emit.h"

#include <SDL_timer.h>

#include "../core/rect.h"
#include "../ecs/components.h"
#include "../events/bus.h"
#include "../events/key_pressed.h"

namespace explore::system {

ProjectileEmit::ProjectileEmit() {
    _name = "ProjectileEmitSystem";

    require_component<component::ProjectileEmitter>();
    require_component<component::Transform>();
}

void ProjectileEmit::subscribe_to_events(event::Bus &event_bus) {
    event_bus.on<event::KeyPressed>(this, &ProjectileEmit::on_key_pressed);
}

void ProjectileEmit::on_key_pressed(event::KeyPressed &event) {
    if (event.symbol == SDLK_SPACE) {
        for (auto &entity : get_entities()) {
            if (entity.has_component<component::CameraFollow>()) {
                const auto &emitter{
                    entity.get_component<component::ProjectileEmitter>()};
                const auto &transform{
                    entity.get_component<component::Transform>()};
                const auto &rigidbody{
                    entity.get_component<component::RigidBody>()};

                // if parent has sprite, start the projectile in the middel
                glm::vec2 projectile_position{transform.position};
                if (entity.has_component<component::Sprite>()) {
                    const auto sprite{
                        entity.get_component<component::Sprite>()};
                    projectile_position.x +=
                        ((sprite.src_rect.w * transform.scale.x) / 2.0);
                    projectile_position.y +=
                        ((sprite.src_rect.h * transform.scale.y) / 2.0);
                }

                glm::vec2 projectile_velocity{emitter.velocity};
                i32 x_dir{0};
                i32 y_dir{0};
                if (rigidbody.velocity.x > 0) x_dir = +1;
                if (rigidbody.velocity.x < 0) x_dir = -1;
                if (rigidbody.velocity.y > 0) y_dir = +1;
                if (rigidbody.velocity.y < 0) y_dir = -1;
                projectile_velocity.x = emitter.velocity.x * x_dir;
                projectile_velocity.y = emitter.velocity.y * y_dir;

                // create new projectile and add it to world
                auto projectile{entity.get_registry()->create_entity()};
                projectile.add_group(constants::PROJECTILE_GROUP);

                projectile.add_component<component::Projectile>(
                    emitter.hit_percent_damage, emitter.duration,
                    emitter.friendly);

                projectile.add_component<component::Transform>(
                    projectile_position, glm::vec2(1.0, 1.0), 0.0);

                projectile.add_component<component::RigidBody>(
                    projectile_velocity);

                projectile.add_component<component::Sprite>(
                    "bullet-tex", 5u, core::rect(0, 0, 4, 4));

                projectile.add_component<component::BoxCollider>(4u, 4u);
            }
        }
    }
}

void ProjectileEmit::update(ecs::Registry &registry) {
    for (auto &entity : get_entities()) {
        auto &emitter{entity.get_component<component::ProjectileEmitter>()};

        if (emitter.interval == 0) continue;

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
            projectile.add_group(constants::PROJECTILE_GROUP);

            projectile.add_component<component::Projectile>(
                emitter.hit_percent_damage, emitter.duration, emitter.friendly);

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
