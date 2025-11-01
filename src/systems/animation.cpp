#include "animation.h"

#include <SDL_timer.h>

#include "../ecs/components.h"

namespace explore::system {

Animation::Animation() {
    require_component<component::Sprite>();
    require_component<component::Animation>();
}

void Animation::update(f32 delta_time) {
    for (auto &entity : get_entities()) {
        auto &animation{entity.get_component<component::Animation>()};
        auto &sprite{entity.get_component<component::Sprite>()};

        animation.current_frame =
            static_cast<u32>((SDL_GetTicks() - animation.start_time) *
                             animation.speed_rate / 1000) %
            animation.num_frames;
        sprite.src_rect.x = animation.current_frame * sprite.width;
    }
}
};  // namespace explore::system
