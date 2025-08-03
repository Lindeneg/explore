#ifndef EXPLORE_CORE_TEXTURE2D_H_
#define EXPLORE_CORE_TEXTURE2D_H_

#include <SDL_render.h>
#include <spdlog/spdlog.h>

#include <filesystem>

#include "../common.h"

struct SDL_Renderer;

namespace explore::core {
class Texture2D {
   private:
    const std::string _name;
    const std::filesystem::path _path;

    SDL_Texture *_data;

    const u32 _width;
    const u32 _height;

   public:
    Texture2D();
    Texture2D(std::string name, std::filesystem::path path, const u32 width,
              const u32 height);
    ~Texture2D();

    const std::string &get_name() const { return _name; }
    const std::filesystem::path &get_path() const { return _path; }
    SDL_Texture *get_data() const { return _data; }
    u32 get_width() const { return _width; }
    u32 get_height() const { return _height; }

    bool initialize(SDL_Renderer *renderer);
};
}  // namespace explore::core

#endif  // EXPLORE_CORE_TEXTURE2D_H_
