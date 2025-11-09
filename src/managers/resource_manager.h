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
class Tilemap;
}  // namespace explore::core

namespace explore::ecs {
class Registry;
}

namespace explore::manager {

class ResourceManager {
   private:
    std::unordered_map<std::string, std::unique_ptr<core::Texture2D>> _textures;
    std::unordered_map<std::string, std::unique_ptr<core::Tilemap>> _tilemaps;
    SDL_Renderer *_renderer;

    std::string _loaded_tilemap;

   public:
    ResourceManager();
    ~ResourceManager();

    void set_renderer(SDL_Renderer *renderer);

    bool add_texture(const std::string &name,
                     const std::filesystem::path &path);
    std::optional<std::reference_wrapper<const core::Texture2D>> get_texture(
        const std::string &name) const;
    bool remove_texture(const std::string &name);

    bool add_tilemap(explore::ecs::Registry &registry, const std::string &name,
                     u32 tile_width, u32 tile_height, u32 tile_scale);
    std::optional<std::reference_wrapper<const core::Tilemap>> get_tilemap(
        const std::string &name) const;

    bool remove_tilemap(const std::string &name);

    bool load_tilemap(const std::string &name,
                      const std::filesystem::path &path,
                      const std::string &texture_name);

    bool unload_tilemap(const std::string &name);

    bool has_loaded_tilemap() const;

    glm::vec2 loaded_tilemap_dimensions() const;
};

}  // namespace explore::manager

#endif  // EXPLORE_MANAGERS_RESOURCE_MANAGER_H_
