#ifndef EXPLORE_SYSTEMS_DAMAGE_H_
#define EXPLORE_SYSTEMS_DAMAGE_H_

#include "../ecs/ecs.h"

namespace explore::event {
class Bus;
class Collision;
}  // namespace explore::event

namespace explore::system {
class Damage : public ecs::System {
   public:
    Damage();

    virtual void subscribe_to_events(event::Bus &event_bus) override;

    void on_collision(event::Collision &event);

    void update();

   private:
    void projectile_player_hit(ecs::Entity projectile, ecs::Entity player);
    void projectile_enemy_hit(ecs::Entity projectile, ecs::Entity enemy);
};
}  // namespace explore::system

#endif  // EXPLORE_SYSTEMS_DAMAGE_H_
