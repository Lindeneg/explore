#include "render.h"

#include <SDL_rect.h>

#include "../core/game_context.h"
#include "../ecs/components.h"
#include "../managers/resource_manager.h"
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

        auto opt_texture{managers::resource::get_texture(sprite.texture_name)};
        ASSERT_RET_V(opt_texture.has_value());
        auto texture{opt_texture.value()};

        managers::screen::draw_texture(
            *texture,
            sdl::rect(transform.position, sprite.width, sprite.height));
    }
}

}  // namespace explore::system
