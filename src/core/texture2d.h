#ifndef EXPLORE_CORE_TEXTURE2D_H_
#define EXPLORE_CORE_TEXTURE2D_H_

#include <SDL_render.h>

#include <filesystem>

#include "../common.h"

namespace explore::core {
struct Texture2D {
    static const i32 DEFAULT_KIND = -1;

    const int kind;
    const std::filesystem::path path;

    SDL_Texture *texture;

    const u32 width;
    const u32 height;

    Texture2D()
        : kind(DEFAULT_KIND), path(""), texture(nullptr), width(0), height(0) {}
    Texture2D(const int kind, const std::filesystem::path path, const u32 width,
              const u32 height)
        : kind(kind),
          path(path),
          texture(nullptr),
          width(width),
          height(height) {}
};
}  // namespace explore::core

#endif  // EXPLORE_CORE_TEXTURE2D_H_
