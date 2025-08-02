#ifndef EXPLORE_CORE_CONTEXT_H_
#define EXPLORE_CORE_CONTEXT_H_

#include "../common.h"

namespace explore::core {
/* contains game-specific information and configuration */
struct GameContext {
    f32 delta_time;
    bool draw_collision_rects;
};
}  // namespace explore::core

#endif  // EXPLORE_CORE_CONTEXT_H_
