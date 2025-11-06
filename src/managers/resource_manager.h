#ifndef EXPLORE_MANAGERS_RESOURCE_MANAGER_H_
#define EXPLORE_MANAGERS_RESOURCE_MANAGER_H_

#include <filesystem>
#include <optional>
#include <string>
#include <unordered_map>

#include "../common.h"

struct SDL_Renderer;

namespace explore::core {
class Texture2D;
}

namespace explore::ecs {
class Registry;
}

namespace explore::manager {

class ResourceManager {
   private:
    std::unordered_map<std::string, std::unique_ptr<core::Texture2D>> _textures;
    SDL_Renderer *_renderer;

   public:
    ResourceManager();
    ~ResourceManager();

    void set_renderer(SDL_Renderer *renderer);

    bool add_texture(const std::string &name,
                     const std::filesystem::path &path);

    std::optional<std::reference_wrapper<const core::Texture2D>> get_texture(
        const std::string &name) const;

    bool remove_texture(const std::string &name);

    void load_tilemap(const std::filesystem::path &path, const std::string &tex,
                      u32 tile_width, u32 tile_height, u32 tile_scale,
                      u32 map_width, u32 map_height,
                      explore::ecs::Registry &registry);
};

}  // namespace explore::manager

#endif  // EXPLORE_MANAGERS_RESOURCE_MANAGER_H_
