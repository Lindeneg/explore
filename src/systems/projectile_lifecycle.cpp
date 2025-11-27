#include "projectile_lifecycle.h"

#include <SDL_timer.h>

#include "../ecs/components.h"

namespace explore::system {

ProjectileLifecycle::ProjectileLifecycle() {
    _name = "ProjectileLifecycleSystem";

    require_component<component::Projectile>();
}

void ProjectileLifecycle::update() {
    for (auto &entity : get_entities_m()) {
        auto &projectile{entity.get_component<component::Projectile>()};

        if (SDL_GetTicks() - projectile.start_time > projectile.duration) {
            entity.kill();
        }
    }
}
};  // namespace explore::system
