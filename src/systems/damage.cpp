#include "damage.h"

#include <spdlog/spdlog.h>

#include "../ecs/components.h"
#include "../events/bus.h"
#include "../events/collision.h"

namespace explore::system {

Damage::Damage() {
    _name = "DamageSystem";
    require_component<component::BoxCollider>();
}

void Damage::subscribe_to_events(event::Bus &event_bus) {
    event_bus.on<event::Collision>(this, &Damage::on_collision);
}

void Damage::on_collision(event::Collision &event) {
    spdlog::trace("collision between '{}:{}' and '{}:{}'", event.a.get_id(),
                  event.a.get_name(), event.b.get_id(), event.b.get_name());

    if (event.a.has_group(constants::PROJECTILE_GROUP) &&
        event.b.has_tag(constants::PLAYER_TAG)) {
        projectile_hit(event.a, event.b, true);
    }

    if (event.b.has_group(constants::PROJECTILE_GROUP) &&
        event.a.has_tag(constants::PLAYER_TAG)) {
        projectile_hit(event.b, event.a, true);
    }

    if (event.a.has_group(constants::PROJECTILE_GROUP) &&
        event.b.has_group(constants::ENEMY_GROUP)) {
        projectile_hit(event.a, event.b, false);
    }

    if (event.b.has_group(constants::PROJECTILE_GROUP) &&
        event.a.has_group(constants::ENEMY_GROUP)) {
        projectile_hit(event.b, event.a, false);
    }
}

void Damage::projectile_hit(ecs::Entity projectile, ecs::Entity entity,
                            bool must_be_unfriendly) {
    const auto &proj{projectile.get_component<component::Projectile>()};

    if (must_be_unfriendly && proj.friendly) return;
    if (!must_be_unfriendly && !proj.friendly) return;

    auto &health{entity.get_component<component::Health>()};

    health.hp_percent -= proj.hit_percent_damage;

    if (health.hp_percent <= 0) {
        entity.kill();
    }

    projectile.kill();
}
void Damage::update() {}
}  // namespace explore::system
