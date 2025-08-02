#ifndef EXPLORE_CORE_TEXTURE2D_H_
#define EXPLORE_CORE_TEXTURE2D_H_

#include <SDL_render.h>
#include <spdlog/spdlog.h>

#include <filesystem>
#include <utility>

#include "../common.h"

namespace explore::core {
struct Texture2D {
    const std::string name;
    const std::filesystem::path path;

    SDL_Texture *data;

    const u32 width;
    const u32 height;

    Texture2D() : name(""), path(""), data(nullptr), width(0), height(0) {}

    Texture2D(std::string name, std::filesystem::path path,
              SDL_Texture *texture, const u32 width, const u32 height)
        : name(std::move(name)),
          path(std::move(path)),
          data(texture),
          width(width),
          height(height) {}
};
}  // namespace explore::core

#endif  // EXPLORE_CORE_TEXTURE2D_H_
