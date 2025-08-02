#ifndef EXPLORE_CORE_TEXTURE2D_H_
#define EXPLORE_CORE_TEXTURE2D_H_

#include <SDL_render.h>
#include <spdlog/spdlog.h>

#include <filesystem>

#include "../common.h"

namespace explore::core {
struct Texture2D {
    const std::string name;
    const std::filesystem::path path;

    SDL_Texture *texture;

    const u32 width;
    const u32 height;

    Texture2D() : name(""), path(""), texture(nullptr), width(0), height(0) {}
    Texture2D(const std::string name, const std::filesystem::path path,
              SDL_Texture *texture, const u32 width, const u32 height)
        : name(name),
          path(path),
          texture(texture),
          width(width),
          height(height) {}

    Texture2D(const Texture2D &obj)
        : name(obj.name),
          path(obj.path),
          texture(obj.texture),
          width(obj.width),
          height(obj.height) {
        spdlog::trace("TEXTURE COPY CONSTRUCTOR CALLED");
    }
};
}  // namespace explore::core

#endif  // EXPLORE_CORE_TEXTURE2D_H_
