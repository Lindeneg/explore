#ifndef EXPLORE_COMMON_H
#define EXPLORE_COMMON_H

#include <SDL2/SDL_pixels.h>

#include <glm/glm.hpp>
#include <string_view>

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

using Color = SDL_Color;
}  // namespace explore

namespace explore::constants {
// TODO maybe move target FPS and frame target to configuration

/* target FPS for fixed updates */
constexpr i32 FPS{120};

/* target time in ms for each frame */
constexpr f64 FRAME_TARGET{1000.f / FPS};

/* maximum delta time (useful if running in debugger) */
constexpr f64 MAXIMUM_DT{0.05f};

constexpr std::string_view PLAYER_TAG{"player"};
constexpr std::string_view ENEMY_TAG{"enemy"};

constexpr std::string_view ENEMY_GROUP{"enemies"};
constexpr std::string_view TILE_GROUP{"tiles"};
constexpr std::string_view PROJECTILE_GROUP{"projectiles"};

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

#endif  // EXPLORE_COMMON_H
