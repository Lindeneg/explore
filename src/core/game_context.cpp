#include "game_context.h"

#include <SDL2/SDL_timer.h>

#include <cmath>

namespace explore::core {
void GameContext::update_delta_time() {
    if (capped_frame_rate) {
        _cap_frame_rate();
    }

    delta_time = (static_cast<f64>(SDL_GetTicks()) -
                  static_cast<f64>(_previous_frame_time)) /
                 1000.f;

    _previous_frame_time = SDL_GetTicks();

    if (sample_fps) {
        _frame_times.push_back(delta_time);
        if (_frame_times.size() > _max_frame_samples) {
            _frame_times.pop_front();
        }
    }
}

u16 GameContext::FPS() const {
    if (!sample_fps) return 0;

    auto size{_frame_times.size()};

    if (size <= 0) return 0;

    f64 avg_dt{0};
    for (f64 t : _frame_times) {
        avg_dt += t;
    }
    avg_dt /= size;

    return std::round(1.f / avg_dt);
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
