#ifndef EXPLORE_CORE_CONTEXT_H_
#define EXPLORE_CORE_CONTEXT_H_

#include "../common.h"

namespace explore::core {
/* contains game-specific information and configuration */
class GameContext {
   public:
    f32 delta_time;
    bool draw_collision_rects;
    bool capped_frame_rate;

   public:
    GameContext()
        : delta_time(0.0f),
          draw_collision_rects(false),
          capped_frame_rate(false),
          _previous_frame_time(0) {}

    void update_delta_time();

   private:
    u32 _previous_frame_time;

   private:
    void _cap_frame_rate() const;
};
}  // namespace explore::core

#endif  // EXPLORE_CORE_CONTEXT_H_
