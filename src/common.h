#ifndef EXPLORE_COMMON_H
#define EXPLORE_COMMON_H

#include <SDL2/SDL_pixels.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef unsigned char uchar;

#define ASSERT(expr)                                                         \
    do {                                                                     \
        if (!(expr)) {                                                       \
            fprintf(stderr,                                                  \
                    "[ASSERT ERROR] [%s:%d:%s] Expression '%s' was false\n", \
                    __FILE__, __LINE__, __func__, #expr);                    \
            std::abort();                                                    \
        }                                                                    \
    } while (0)

#define ASSERT_MSG(expr, fmt, ...)                                           \
    do {                                                                     \
        if (!(expr)) {                                                       \
            fprintf(stderr, "[ASSERT ERROR] [%s:%d:%s] " fmt "\n", __FILE__, \
                    __LINE__, __func__, ##__VA_ARGS__);                      \
            std::abort();                                                    \
        }                                                                    \
    } while (0)

#define ASSERT_RET(expr, ret)                                                  \
    do {                                                                       \
        if (!(expr)) {                                                         \
            fprintf(stderr,                                                    \
                    "[ASSERT WARNING] [%s:%d:%s] Expression '%s' was false\n", \
                    __FILE__, __LINE__, __func__, #expr);                      \
            return (ret);                                                      \
        }                                                                      \
    } while (0)

#define ASSERT_RET_V(expr)                                                     \
    do {                                                                       \
        if (!(expr)) {                                                         \
            fprintf(stderr,                                                    \
                    "[ASSERT WARNING] [%s:%d:%s] Expression '%s' was false\n", \
                    __FILE__, __LINE__, __func__, #expr);                      \
            return;                                                            \
        }                                                                      \
    } while (0)

#define ASSERT_RET_MSG(expr, ret, fmt, ...)                                    \
    do {                                                                       \
        if (!(expr)) {                                                         \
            fprintf(stderr, "[ASSERT WARNING] [%s:%d:%s] " fmt "\n", __FILE__, \
                    __LINE__, __func__, ##__VA_ARGS__);                        \
            return (ret);                                                      \
        }                                                                      \
    } while (0)

#define ASSERT_RET_V_MSG(expr, fmt, ...)                                       \
    do {                                                                       \
        if (!(expr)) {                                                         \
            fprintf(stderr, "[ASSERT WARNING] [%s:%d:%s] " fmt "\n", __FILE__, \
                    __LINE__, __func__, ##__VA_ARGS__);                        \
            return;                                                            \
        }                                                                      \
    } while (0)

namespace explore {
/* contains game-specific information and configuration */
struct GameContext {
    f32 delta_time;
    bool draw_collision_rects;
};

using Color = SDL_Color;
}  // namespace explore

namespace explore::constants {
/* target FPS for fixed updates */
const i32 FPS{60};

/* target time in ms for each frame */
const i32 FRAME_TARGET{1000 / FPS};

/* maximum delta time (useful if running in debugger) */
const f32 MAXIMUM_DT{0.05f};

/* used to produce epsilon relative
 * to value of some given operands */
const f32 REL_EPSILON{1e-8f};

/* absolute epsilon value */
const f32 ABS_EPSILON{1e-12f};

/* approximation of PI */
const f64 PI{3.14159265359};

/* twice the pi, double the fun! */
const f64 PI2{PI * 2};
}  // namespace explore::constants

/* collection of sdl colors */
namespace explore::color {
const Color white{255, 255, 255, 255};
const Color black{0, 0, 0, 255};
const Color red{255, 0, 0, 255};
const Color green{0, 255, 0, 255};
const Color blue{0, 0, 255, 255};
const Color yellow{255, 255, 0, 255};
const Color cyan{0, 255, 255, 255};
const Color magenta{255, 0, 255, 255};
const Color gray{128, 128, 128, 255};
const Color light_gray{211, 211, 211, 255};
const Color dark_gray{64, 64, 64, 255};
const Color orange{255, 165, 0, 255};
const Color pink{255, 192, 203, 255};
const Color purple{128, 0, 128, 255};
const Color brown{165, 42, 42, 255};
const Color light_blue{173, 216, 230, 255};
const Color light_green{144, 238, 144, 255};
const Color dark_red{139, 0, 0, 255};
const Color dark_green{0, 100, 0, 255};
const Color dark_blue{0, 0, 139, 255};
}  // namespace explore::color
#endif  // EXPLORE_COMMON_H
