#include "tilemap.h"

#include <SDL_render.h>
#include <spdlog/spdlog.h>

#include <sstream>
#include <string>

#include "../core/file.h"
#include "../core/rect.h"
#include "../core/texture2d.h"
#include "../ecs/components.h"
#include "../ecs/ecs.h"

namespace explore::core {
Tilemap::Tilemap(explore::ecs::Registry &registry, std::string name,
                 u32 tile_width, u32 tile_height, u32 tile_scale)
    : _entities(),
      _registry(registry),
      _name(name),
      _tile_width(tile_width),
      _tile_height(tile_height),
      _tile_scale(tile_scale),
      _map_width(0),
      _map_height(0),
      _is_loaded(false) {}

Tilemap::~Tilemap() {
    if (_entities.size()) {
        unload();
    }
}

bool Tilemap::load(const std::filesystem::path &path,
                   const Texture2D &texture) {
    ASSERT_RET_MSG(!_is_loaded && _entities.size() == 0, false,
                   "tilemap already loaded");

    std::string file_contents;
    if (!explore::file::read_all(path, file_contents)) {
        spdlog::error("failed to read tilemap file: {}", path.string());
        return _is_loaded;
    }

    const u32 tileset_cols = texture.get_width() / _tile_width;
    ASSERT_RET_MSG(tileset_cols > 0, _is_loaded,
                   "tileset columns is zero — check tile sizes");

    std::stringstream ss(file_contents);
    std::string line;

    u32 y = 0;
    u32 x = 0;
    while (std::getline(ss, line)) {
        std::stringstream line_stream(line);
        std::string value;

        x = 0;
        while (std::getline(line_stream, value, ',')) {
            int tile_index = std::stoi(value);

            if (tile_index >= 0) {
                const u32 tile_col =
                    static_cast<u32>(tile_index) % tileset_cols;
                const u32 tile_row =
                    static_cast<u32>(tile_index) / tileset_cols;

                SDL_Rect src{core::rect(tile_col * _tile_width,
                                        tile_row * _tile_height, _tile_width,
                                        _tile_height)};

                explore::ecs::Entity tile = _registry.create_entity();
                tile.add_group(constants::TILE_GROUP);

                glm::vec2 pos{
                    static_cast<float>(x * _tile_width * _tile_scale),
                    static_cast<float>(y * _tile_height * _tile_scale)};

                glm::vec2 scale{static_cast<float>(_tile_scale),
                                static_cast<float>(_tile_scale)};

                tile.add_component<component::Transform>(pos, scale, 0.f);
                tile.add_component<component::Sprite>(texture.get_name(), 0u,
                                                      src);

                _entities.push_back(tile);
            }

            ++x;
        }

        ++y;
    }
    _map_width = x;
    _map_height = y;
    _is_loaded = true;
    spdlog::debug("tilemap '{}' loaded with '{}x{}' tiles from '{}'", _name,
                  _map_width, _map_height, path.string());
    return _is_loaded;
}

bool Tilemap::unload() {
    ASSERT_RET_MSG(_is_loaded, false, "no tilemap loaded");

    for (auto &entity : _entities) {
        _registry.kill_entity(entity);
    }

    _entities.clear();
    _map_width = 0;
    _map_height = 0;
    _is_loaded = false;

    spdlog::debug("tilemap '{}' unloaded", _name);

    return true;
}

}  // namespace explore::core
