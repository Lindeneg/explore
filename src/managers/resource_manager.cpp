#include "resource_manager.h"

#include <SDL_render.h>
#include <spdlog/spdlog.h>

#include <optional>
#include <string>

#include "../core/texture2d.h"
#include "../core/tilemap.h"
#include "./screen_manager.h"

namespace explore::manager {

ResourceManager::ResourceManager()
    : _textures(), _renderer(nullptr), _loaded_tilemap("") {}

ResourceManager::~ResourceManager() {
    spdlog::trace("clearing all resources");
    _textures.clear();
    if (has_loaded_tilemap()) {
        unload_tilemap(_loaded_tilemap);
    }
    _loaded_tilemap.clear();
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

bool ResourceManager::add_tilemap(explore::ecs::Registry &registry,
                                  const std::string &name, u32 tile_width,
                                  u32 tile_height, u32 tile_scale) {
    if (_tilemaps.find(name) != _tilemaps.end()) {
        spdlog::warn("tilemap '{}' has already been added", name);
        return true;
    }

    auto tilemap = std::make_unique<core::Tilemap>(registry, name, tile_width,
                                                   tile_height, tile_scale);
    _tilemaps.emplace(name, std::move(tilemap));

    return true;
}

std::optional<std::reference_wrapper<const core::Tilemap>>
ResourceManager::get_tilemap(const std::string &name) const {
    auto it = _tilemaps.find(name);
    if (it == _tilemaps.end()) return std::nullopt;
    return std::cref(*it->second);
}

bool ResourceManager::remove_tilemap(const std::string &name) {
    const auto iter{_tilemaps.find(name)};
    if (iter == _tilemaps.end()) {
        spdlog::error("failed to remove tilemap: '{}'", name);
        return false;
    }
    _tilemaps.erase(iter);
    return true;
}

bool ResourceManager::load_tilemap(const std::string &name,
                                   const std::filesystem::path &path,
                                   const std::string &texture_name) {
    ASSERT_RET_MSG(_loaded_tilemap == "", false, "tilemap already loaded");
    auto it = _tilemaps.find(name);
    if (it == _tilemaps.end()) return false;

    auto opt_texture{get_texture(texture_name)};
    ASSERT_RET_MSG(opt_texture.has_value(), false, "tilemap texture not found");
    const core::Texture2D &texture{opt_texture->get()};

    if (it->second->load(path, texture)) {
        _loaded_tilemap = it->second->name();
        return true;
    }

    return false;
}

bool ResourceManager::unload_tilemap(const std::string &name) {
    auto it = _tilemaps.find(name);
    if (it == _tilemaps.end()) return false;
    if (it->second->unload()) {
        _loaded_tilemap = "";
        return true;
    }
    return false;
}

bool ResourceManager::has_loaded_tilemap() const {
    return _loaded_tilemap != "";
}

glm::vec2 ResourceManager::loaded_tilemap_dimensions() const {
    if (!has_loaded_tilemap()) return glm::vec2(0);
    auto t{_tilemaps.find(_loaded_tilemap)};
    return glm::vec2(t->second->scaled_map_width(),
                     t->second->scaled_map_height());
}

}  // namespace explore::manager

