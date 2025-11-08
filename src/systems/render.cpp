#include "render.h"

#include "../core/rect.h"
#include "../core/texture2d.h"
#include "../ecs/components.h"
#include "../managers/resource_manager.h"
#include "../managers/screen_manager.h"

namespace explore::system {

Render::Render() {
    _name = "RenderSystem";

    require_component<component::Transform>();
    require_component<component::Sprite>();
}

void Render::add_entity(ecs::Entity entity) {
    int z = entity.get_component<component::Sprite>().z_index;

    auto it = std::lower_bound(
        _entities.begin(), _entities.end(), z,
        [&](const ecs::Entity &e, int z_value) {
            return e.get_component<component::Sprite>().z_index < z_value;
        });

    _entities.insert(it, entity);
}

void Render::update(const manager::ScreenManager &screen_manager,
                    const manager::ResourceManager &resource_manager,
                    const SDL_Rect &camera) {
    for (const auto &entity : get_entities()) {
        const auto transform = entity.get_component<component::Transform>();
        const auto sprite{entity.get_component<component::Sprite>()};

        if (transform.scale.x == 0 && transform.scale.y == 0) continue;

        auto opt_texture{resource_manager.get_texture(sprite.texture_name)};
        ASSERT_RET_V(opt_texture.has_value());
        const core::Texture2D &texture{opt_texture->get()};

        const auto scaled_w =
            static_cast<u32>(sprite.src_rect.w * transform.scale.x);
        const auto scaled_h =
            static_cast<u32>(sprite.src_rect.h * transform.scale.y);

        const auto position{glm::vec2(transform.position.x - camera.x,
                                      transform.position.y - camera.y)};

        screen_manager.draw_texture(texture, sprite.src_rect,
                                    core::rect(position, scaled_w, scaled_h),
                                    transform.rotation);
    }
}

}  // namespace explore::system
