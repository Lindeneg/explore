#ifndef EXPLORE_SYSTEMS_CAMERA_MOVEMENT_H_
#define EXPLORE_SYSTEMS_CAMERA_MOVEMENT_H_

#include <SDL_rect.h>

#include "../ecs/ecs.h"

namespace explore::core {
class GameContext;
}

namespace explore::system {
class CameraMovement : public ecs::System {
   public:
    CameraMovement();

    void update(SDL_Rect &camera, const core::GameContext &game_context);
};
}  // namespace explore::system

#endif  // EXPLORE_SYSTEMS_CAMERA_MOVEMENT_H_
