#ifndef EXPLORE_CORE_RECT_H_
#define EXPLORE_CORE_RECT_H_

#include <SDL_rect.h>

#include "../common.h"

namespace explore::component {
class Transform;
class BoxCollider;
}  // namespace explore::component

namespace explore::core {
SDL_Rect rect(f32 x, f32 y, u32 w, u32 h);
SDL_Rect rect(glm::vec2 pos, u32 w, u32 h);
SDL_Rect rect(const component::Transform &t, const component::BoxCollider &c);
}  // namespace explore::core

#endif  // EXPLORE_CORE_RECT_H_
