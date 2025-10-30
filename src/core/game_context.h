#ifndef EXPLORE_CORE_CONTEXT_H_
#define EXPLORE_CORE_CONTEXT_H_

#include <SDL_timer.h>

#include "../common.h"

namespace explore::core {
/* contains game-specific information and configuration */
class GameContext {
   public:
    f64 delta_time;

    bool draw_collision_rects;
    bool capped_frame_rate;

   public:
    GameContext()
        : delta_time(0.0f),
          draw_collision_rects(false),
          capped_frame_rate(false),
          _previous_frame_time(0) {}

    void update_delta_time();

    u16 FPS() const;

   private:
    u64 _previous_frame_time;

   private:
    void _cap_frame_rate() const;
};
}  // namespace explore::core

#endif  // EXPLORE_CORE_CONTEXT_H_
