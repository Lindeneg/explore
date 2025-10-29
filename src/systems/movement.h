#ifndef EXPLORE_SYSTEMS_MOVEMENT_H_
#define EXPLORE_SYSTEMS_MOVEMENT_H_

#include "../ecs/ecs.h"

namespace explore::core {
class GameContext;
}

namespace explore::system {
class Movement : public ecs::System {
   public:
    Movement();

    void update(f32 delta_time);
};
}  // namespace explore::system

#endif  // EXPLORE_SYSTEMS_MOVEMENT_H_
