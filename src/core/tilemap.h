#ifndef EXPLORE_CORE_TILEMAP_H_
#define EXPLORE_CORE_TILEMAP_H_

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include "../common.h"

namespace explore::ecs {
class Registry;
class Entity;
}  // namespace explore::ecs

namespace explore::core {
class Texture2D;

// TODO: make this mess a lot better, so many things not considered
// multiple textures, multuple maps etc.
class Tilemap {
   public:
    Tilemap(explore::ecs::Registry &registry, std::string name, u32 tile_width,
            u32 tile_height, u32 tile_scale);
    ~Tilemap();

    std::string_view name() const { return _name; }
    u32 tile_width() const { return _tile_width; }
    u32 tile_height() const { return _tile_height; }
    u32 tile_scale() const { return _tile_scale; }
    u32 map_width() const { return _map_width; }
    u32 map_height() const { return _map_height; }
    u32 scaled_map_width() const {
        return _map_width * _tile_width * _tile_scale;
    }
    u32 scaled_map_height() const {
        return _map_height * _tile_height * _tile_scale;
    }

    bool load(const std::filesystem::path &path, const Texture2D &texture);

    bool unload();

   private:
    std::vector<ecs::Entity> _entities;

    explore::ecs::Registry &_registry;

    std::string _name;

    u32 _tile_width;
    u32 _tile_height;
    u32 _tile_scale;

    u32 _map_width;
    u32 _map_height;

    bool _is_loaded;
};

}  // namespace explore::core
   //
#endif  // EXPLORE_CORE_TILEMAP_H_
