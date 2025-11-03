#ifndef EXPLORE_EVENTS_COLLISION_H_
#define EXPLORE_EVENTS_COLLISION_H_

#include "../ecs/ecs.h"
#include "./event.h"

namespace explore::event {

struct Collision : public Event {
    ecs::Entity a;
    ecs::Entity b;

    Collision(ecs::Entity a, ecs::Entity b) : a(a), b(b) {}
};

}  // namespace explore::event

#endif  // EXPLORE_EVENTS_COLLISION_H_
