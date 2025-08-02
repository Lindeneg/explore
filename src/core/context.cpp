#include "context.h"

#include <SDL2/SDL_timer.h>

#include <algorithm>

namespace explore::core {
void GameContext::update_delta_time() {
    if (capped_frame_rate) {
        cap_frame_rate();
    }
    const f32 dt = (static_cast<f32>(SDL_GetTicks()) -
                    static_cast<f32>(previous_frame_time)) /
                   1000.f;

    // clamp value (if running in debugger dt will be messed up)
    delta_time = std::min(dt, explore::constants::MAXIMUM_DT);

    previous_frame_time = SDL_GetTicks();
}

void GameContext::cap_frame_rate() {
    const i32 time_to_wait = static_cast<i32>(
        explore::constants::FRAME_TARGET -
        (static_cast<u32>(SDL_GetTicks()) - previous_frame_time));
    // only delay if too fast
    if (time_to_wait > 0 && time_to_wait < explore::constants::FRAME_TARGET) {
        SDL_Delay(time_to_wait);
    }
}
}  // namespace explore::core
