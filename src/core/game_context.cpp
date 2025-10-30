#include "game_context.h"

#include <SDL2/SDL_timer.h>

#include <algorithm>
#include <cmath>

namespace explore::core {
void GameContext::update_delta_time() {
    if (capped_frame_rate) {
        _cap_frame_rate();
    }

    const f64 dt{(static_cast<f64>(SDL_GetTicks()) -
                  static_cast<f64>(_previous_frame_time)) /
                 1000.f};

    // clamp value (if running in debugger dt will be messed up)
    delta_time = std::min(dt, constants::MAXIMUM_DT);

    _previous_frame_time = SDL_GetTicks();
}

u16 GameContext::FPS() const {
    // TODO: sample dt's and take an over over N frames
    return std::round(1 / delta_time);
}

void GameContext::_cap_frame_rate() const {
    const i32 time_to_wait{static_cast<i32>(
        constants::FRAME_TARGET - (SDL_GetTicks() - _previous_frame_time))};
    // only delay if too fast
    if (time_to_wait > 0 && time_to_wait <= constants::FRAME_TARGET) {
        SDL_Delay(time_to_wait);
    }
}
}  // namespace explore::core
