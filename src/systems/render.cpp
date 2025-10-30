#include "render.h"

#include <SDL_rect.h>

#include "../core/game_context.h"
#include "../ecs/components.h"
#include "../managers/screen_manager.h"

namespace explore::system {

Render::Render() {
    require_component<component::Transform>();
    require_component<component::Sprite>();
}

void Render::update(const core::GameContext &game_context) {
    for (const auto &entity : get_entities()) {
        const auto transform = entity.get_component<component::Transform>();
        const auto sprite{entity.get_component<component::Sprite>()};

        SDL_Rect rect{static_cast<int>(transform.position.x),
                      static_cast<int>(transform.position.y),
                      static_cast<int>(sprite.width),
                      static_cast<int>(sprite.height)};

        managers::screen::draw_rect(rect);
    }
}

}  // namespace explore::system
