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
    }

    if (event.b.has_group(constants::PROJECTILE_GROUP) &&
        event.a.has_tag(constants::PLAYER_TAG)) {
        projectile_player_hit(event.b, event.a);
    }

    if (event.a.has_group(constants::PROJECTILE_GROUP) &&
        event.b.has_group(constants::ENEMY_GROUP)) {
        projectile_enemy_hit(event.a, event.b);
    }

    if (event.b.has_group(constants::PROJECTILE_GROUP) &&
        event.a.has_group(constants::ENEMY_GROUP)) {
        projectile_enemy_hit(event.b, event.a);
    }
}

void Damage::projectile_player_hit(ecs::Entity projectile, ecs::Entity player) {
    const auto &proj{projectile.get_component<component::Projectile>()};

    if (proj.friendly) return;

    auto &player_health{player.get_component<component::Health>()};

    player_health.hp_percent -= proj.hit_percent_damage;

    if (player_health.hp_percent <= 0) {
        player.kill();
    }

    projectile.kill();
}

void Damage::projectile_enemy_hit(ecs::Entity projectile, ecs::Entity enemy) {
    const auto &proj{projectile.get_component<component::Projectile>()};
    if (!proj.friendly) return;

    auto &enemy_health{enemy.get_component<component::Health>()};

    enemy_health.hp_percent -= proj.hit_percent_damage;

    if (enemy_health.hp_percent <= 0) {
        enemy.kill();
    }

    projectile.kill();
}

void Damage::update() {}
}  // namespace explore::system
