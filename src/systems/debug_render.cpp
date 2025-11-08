#include "debug_render.h"

#include "../core/rect.h"
#include "../ecs/components.h"
#include "../managers/screen_manager.h"

namespace explore::system {

DebugRender::DebugRender() {
    _name = "DebugRenderSystem";

    require_component<component::Transform>();
    require_component<component::BoxCollider>();
}

void DebugRender::update(manager::ScreenManager &screen_manager) {
    for (const auto &entity : get_entities()) {
        const auto &transform = entity.get_component<component::Transform>();
        const auto &box_collider{
            entity.get_component<component::BoxCollider>()};

        const auto rect = core::rect(transform, box_collider);
        screen_manager.draw_rect_outline(rect, color::green);
    }
}
}  // namespace explore::system
