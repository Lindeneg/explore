#ifndef EXPLORE_SYSTEMS_PROJECTILE_EMIT_H_
#define EXPLORE_SYSTEMS_PROJECTILE_EMIT_H_

#include "../ecs/ecs.h"

namespace explore::event {
class Bus;
class KeyPressed;
}  // namespace explore::event

namespace explore::system {
class ProjectileEmit : public ecs::System {
   public:
    ProjectileEmit();

    virtual void subscribe_to_events(event::Bus &event_bus) override;

    void on_key_pressed(event::KeyPressed &event);

    void update(ecs::Registry &registry);
};
}  // namespace explore::system

#endif  // EXPLORE_SYSTEMS_PROJECTILE_EMIT_H_
