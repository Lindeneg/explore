#ifndef EXPLORE_CORE_TEXTURE2D_H_
#define EXPLORE_CORE_TEXTURE2D_H_

#include <SDL_render.h>
#include <spdlog/spdlog.h>

#include <filesystem>

#include "../common.h"

struct SDL_Renderer;

namespace explore::core {
class Texture2D {
   public:
    Texture2D();
    Texture2D(std::string name, std::filesystem::path path);
    ~Texture2D();

    [[nodiscard]] const std::string &get_name() const { return _name; }
    [[nodiscard]] const std::filesystem::path &get_path() const {
        return _path;
    }
    [[nodiscard]] SDL_Texture *get_data() const { return _data; }
    [[nodiscard]] u32 get_width() const { return _width; }
    [[nodiscard]] u32 get_height() const { return _height; }

    bool initialize(SDL_Renderer *renderer);

   private:
    const std::string _name;
    const std::filesystem::path _path;

    u32 _width;
    u32 _height;

    SDL_Texture *_data;
};
}  // namespace explore::core

#endif  // EXPLORE_CORE_TEXTURE2D_H_
