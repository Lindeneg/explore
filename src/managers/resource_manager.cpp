#include "resource_manager.h"

#include <SDL_render.h>
#include <spdlog/spdlog.h>

#include <optional>
#include <sstream>
#include <string>

#include "../core/file.h"
#include "../core/rect.h"
#include "../core/texture2d.h"
#include "../ecs/components.h"
#include "../ecs/ecs.h"
#include "./screen_manager.h"

namespace explore::manager {

ResourceManager::ResourceManager() : _textures(), _renderer(nullptr) {}

ResourceManager::~ResourceManager() {
    spdlog::trace("clearing all resources");
    _textures.clear();
}

void ResourceManager::set_renderer(SDL_Renderer *renderer) {
    _renderer = renderer;
}

bool ResourceManager::add_texture(const std::string &name,
                                  const std::filesystem::path &path) {
    ASSERT_RET_MSG(_renderer, false, "renderer is null");

    if (_textures.find(name) != _textures.end()) {
        spdlog::warn("texture '{}' has already been added", name);
        return true;
    }

    auto tex = std::make_unique<core::Texture2D>(name, path);
    if (!tex->initialize(_renderer)) {
        spdlog::error("failed to initialize texture '{}'<-'{}'", name,
                      path.string());
        return false;
    }

    _textures.emplace(name, std::move(tex));
    return true;
}

std::optional<std::reference_wrapper<const core::Texture2D>>
ResourceManager::get_texture(const std::string &name) const {
    auto it = _textures.find(name);
    if (it == _textures.end()) return std::nullopt;
    return std::cref(*it->second);
}

bool ResourceManager::remove_texture(const std::string &name) {
    const auto iter{_textures.find(name)};
    if (iter == _textures.end()) {
        spdlog::error("failed to remove texture: '{}'", name);
        return false;
    }
    _textures.erase(iter);
    return true;
}

void ResourceManager::load_tilemap(const std::filesystem::path &path,
                                   const std::string &tex, u32 tile_width,
                                   u32 tile_height, u32 tile_scale,
                                   u32 map_width, u32 map_height,
                                   explore::ecs::Registry &registry) {
    std::string file_contents;
    if (!explore::file::read_all(path, file_contents)) {
        spdlog::error("failed to read tilemap file: {}", path.string());
        return;
    }

    auto tex_opt = get_texture(tex);
    ASSERT_RET_V(tex_opt.has_value());
    const explore::core::Texture2D &texture = tex_opt->get();

    const u32 tileset_cols = texture.get_width() / tile_width;
    ASSERT_RET_V_MSG(tileset_cols > 0,
                     "tileset columns is zero — check tile sizes");

    std::stringstream ss(file_contents);
    std::string line;

    u32 y = 0;
    while (y < map_height && std::getline(ss, line)) {
        std::stringstream line_stream(line);
        std::string value;

        u32 x = 0;
        while (x < map_width && std::getline(line_stream, value, ',')) {
            int tile_index = std::stoi(value);

            if (tile_index >= 0) {
                const u32 tile_col =
                    static_cast<u32>(tile_index) % tileset_cols;
                const u32 tile_row =
                    static_cast<u32>(tile_index) / tileset_cols;

                SDL_Rect src{core::rect(tile_col * tile_width,
                                        tile_row * tile_height, tile_width,
                                        tile_height)};

                explore::ecs::Entity tile = registry.create_entity();

                glm::vec2 pos{static_cast<float>(x * tile_width * tile_scale),
                              static_cast<float>(y * tile_height * tile_scale)};

                glm::vec2 scale{static_cast<float>(tile_scale),
                                static_cast<float>(tile_scale)};

                tile.add_component<component::Transform>(pos, scale, 0.f);
                tile.add_component<component::Sprite>(tex, 0u, src);
            }

            ++x;
        }

        ++y;
    }

    spdlog::info("Tilemap loaded: {}x{} tiles from '{}'", map_width, map_height,
                 path.string());
}

}  // namespace explore::manager

