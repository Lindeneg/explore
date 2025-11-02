#include "collision.h"

#include "../ecs/components.h"

namespace explore::system {

Collision::Collision() {
    require_component<component::Transform>();
    require_component<component::BoxCollider>();
}

void Collision::update(f32) {
    const auto &entities = get_entities();
    const size_t count = entities.size();

    // TODO: N^2 is fine for now; optimizations can come later
    for (size_t i = 0; i < count; ++i) {
        ecs::Entity a = entities[i];
        const auto &ta = a.get_component<component::Transform>();
        const auto &ca = a.get_component<component::BoxCollider>();

        SDL_Rect ra = make_rect(ta, ca);

        for (size_t j = i + 1; j < count; ++j) {
            ecs::Entity b = entities[j];
            const auto &tb = b.get_component<component::Transform>();
            const auto &cb = b.get_component<component::BoxCollider>();

            SDL_Rect rb = make_rect(tb, cb);

            if (aabb_intersect(ra, rb)) {
                spdlog::debug("Collision detected between '{}' and '{}'",
                              a.get_name(), b.get_name());
            }
        }
    }
}

SDL_Rect Collision::make_rect(const component::Transform &t,
                              const component::BoxCollider &c) {
    return sdl::rect(t.position.x + c.offset.x, t.position.y + c.offset.y,
                     c.width * t.scale.x, c.height * t.scale.y);
}

bool Collision::aabb_intersect(const SDL_Rect &a, const SDL_Rect &b) {
    return (a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h &&
            a.y + a.h > b.y);
}
};  // namespace explore::system
