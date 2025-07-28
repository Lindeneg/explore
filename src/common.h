#ifndef COMMON_H
#define COMMON_H

#include <SDL2/SDL_pixels.h>

#include <algorithm>

namespace explore {
/* contains game-specific information and configuration */
struct GameContext {
    float delta_time;
    bool draw_collision_rects;
};

using Color = SDL_Color;
}  // namespace explore

namespace explore::constants {
/* target FPS for fixed updates */
constexpr int FPS{60};

/* target time in ms for each frame */
constexpr int FRAME_TARGET{1000 / FPS};

/* maximum delta time (useful if running in debugger) */
constexpr float MAXIMUM_DT{0.05f};

/* used to produce epsilon relative
 * to value of some given operands */
constexpr float REL_EPSILON{1e-8f};

/* absolute epsilon value */
constexpr float ABS_EPSILON{1e-12f};

/* approximation of PI */
constexpr double PI{3.14159265359};

/* twice the pi, double the fun! */
constexpr double PI2{PI * 2};
}  // namespace explore::constants

/* collection of sdl colors */
namespace explore::color {
constexpr Color white{255, 255, 255, 255};
constexpr Color black{0, 0, 0, 255};
constexpr Color red{255, 0, 0, 255};
constexpr Color green{0, 255, 0, 255};
constexpr Color blue{0, 0, 255, 255};
constexpr Color yellow{255, 255, 0, 255};
constexpr Color cyan{0, 255, 255, 255};
constexpr Color magenta{255, 0, 255, 255};
constexpr Color gray{128, 128, 128, 255};
constexpr Color light_gray{211, 211, 211, 255};
constexpr Color dark_gray{64, 64, 64, 255};
constexpr Color orange{255, 165, 0, 255};
constexpr Color pink{255, 192, 203, 255};
constexpr Color purple{128, 0, 128, 255};
constexpr Color brown{165, 42, 42, 255};
constexpr Color light_blue{173, 216, 230, 255};
constexpr Color light_green{144, 238, 144, 255};
constexpr Color dark_red{139, 0, 0, 255};
constexpr Color dark_green{0, 100, 0, 255};
constexpr Color dark_blue{0, 0, 139, 255};
}  // namespace explore::color

template <typename T>
static T explore_abs(T x) noexcept {
    return (x < 0 ? -x : x);
}

template <typename T>
static bool explore_equal_rel(T a, T b) noexcept {
    return explore_abs(a - b) <= std::max(explore_abs(a), explore_abs(b)) *
                                     explore::constants::REL_EPSILON;
}

namespace explore::utils {
template <typename T>
bool equal(T a, T b) noexcept {
    if (explore_abs(a - b) <= constants::ABS_EPSILON) {
        return true;
    }
    return explore_equal_rel(a, b);
}

template <typename T>
bool greater_or_equal(T a, T b) noexcept {
    if (equal(a, b)) {
        return true;
    }
    return a > b;
}

template <typename T>
bool less_or_equal(T a, T b) noexcept {
    if (equal(a, b)) {
        return true;
    }
    return a < b;
}

template <typename T>
bool greater(T a, T b) noexcept {
    if (equal<T>(a, b)) {
        return false;
    }
    return a > b;
}

template <typename T>
bool lesser(T a, T b) noexcept {
    if (equal(a, b)) {
        return false;
    }
    return a < b;
}

template <typename T>
T lerp(T a, T b, T t) noexcept {
    return a + t * (b - a);
}

template <typename T>
T to_radians(T degrees) noexcept {
    return degrees * constants::PI / 180.f;
}

template <typename T>
T to_degrees(T radians) noexcept {
    return radians * 180.f / constants::PI;
}
}  // namespace explore::utils

#endif  // COMMON_H
