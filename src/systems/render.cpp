#include "render.h"

#include <SDL_rect.h>

#include "../core/game_context.h"
#include "../core/texture2d.h"
#include "../ecs/components.h"
#include "../managers/resource_manager.h"
#include "../managers/screen_manager.h"

namespace explore::system {

Render::Render() {
    require_component<component::Transform>();
    require_component<component::Sprite>();
}

void Render::add_entity(ecs::Entity entity) {
    spdlog::debug("RENDER ADD ENTITY {}", entity.get_name());

    int z = entity.get_component<component::Sprite>().z_index;

    auto it = std::lower_bound(
        _entities.begin(), _entities.end(), z,
        [&](const ecs::Entity &e, int z_value) {
            return e.get_component<component::Sprite>().z_index < z_value;
        });

    _entities.insert(it, entity);
}

void Render::update(const core::GameContext &game_context) {
    for (const auto &entity : get_entities()) {
        const auto transform = entity.get_component<component::Transform>();
        const auto sprite{entity.get_component<component::Sprite>()};

        if (transform.scale.x == 0 && transform.scale.y == 0) continue;

        auto opt_texture{managers::resource::get_texture(sprite.texture_name)};
        ASSERT_RET_V(opt_texture.has_value());
        auto texture{opt_texture.value()};

        u32 scaled_w{sprite.width};
        u32 scaled_h{sprite.height};
        if (sprite.width == 0 && sprite.height == 0) {
            scaled_w = texture->get_width();
            scaled_h = texture->get_height();
        }
        scaled_w = static_cast<u32>(scaled_w * transform.scale.x);
        scaled_h = static_cast<u32>(scaled_h * transform.scale.y);

        managers::screen::draw_texture(
            *texture, sprite.src_rect,
            sdl::rect(transform.position, scaled_w, scaled_h),
            transform.rotation);
    }
}

}  // namespace explore::system
