#include "camera_movement.h"

#include "../core/game_context.h"
#include "../ecs/components.h"

namespace explore::system {

CameraMovement::CameraMovement() {
    require_component<component::CameraFollow>();
    require_component<component::Transform>();
}

void CameraMovement::update(SDL_Rect &camera,
                            const core::GameContext &game_context) {
    for (auto &entity : get_entities()) {
        const auto &transform = entity.get_component<component::Transform>();

        if (transform.position.x + (camera.w / 2.f) < game_context.map_width) {
            camera.x = transform.position.x - (game_context.window_width / 2.f);
        }

        if (transform.position.y + (camera.h / 2.f) < game_context.map_height) {
            camera.y =
                transform.position.y - (game_context.window_height / 2.f);
        }

        camera.x = camera.x < 0 ? 0 : camera.x;
        camera.y = camera.y < 0 ? 0 : camera.y;

        camera.x = camera.x > camera.w ? camera.w : camera.x;
        camera.y = camera.y > camera.h ? camera.h : camera.y;
    }
}

}  // namespace explore::system
