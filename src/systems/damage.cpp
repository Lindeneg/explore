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

    //    event.a.kill();
    //    event.b.kill();
}

void Damage::update() {}
}  // namespace explore::system
