#include "rect.h"

#include "../ecs/components.h"

namespace explore::core {
SDL_Rect rect(f32 x, f32 y, u32 w, u32 h) {
    SDL_Rect r{
        static_cast<int>(x),
        static_cast<int>(y),
        static_cast<int>(w),
        static_cast<int>(h),
    };
    return r;
}

SDL_Rect rect(glm::vec2 pos, u32 w, u32 h) { return rect(pos.x, pos.y, w, h); }

SDL_Rect rect(const component::Transform &t, const component::BoxCollider &c) {
    return rect(t.position.x + c.offset.x, t.position.y + c.offset.y,
                c.width * t.scale.x, c.height * t.scale.y);
}
}  // namespace explore::core
