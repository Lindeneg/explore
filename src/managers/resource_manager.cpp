#include "resource_manager.h"

#include <SDL_render.h>
#include <spdlog/spdlog.h>

#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>

#include "../core/file.h"
#include "../core/texture2d.h"
#include "../ecs/components.h"
#include "../ecs/ecs.h"
#include "./screen_manager.h"

static std::unordered_map<std::string, explore::core::Texture2D *> textures{};

namespace explore::managers::resource {
bool add_texture(const std::string &name, const std::filesystem::path &path,
                 const u32 width, const u32 height) {
    if (const auto iter{textures.find(name)}; iter != textures.end()) {
        spdlog::warn("texture '{}' has already been added", name);
        return true;
    }

    auto tex{new core::Texture2D(name, path, width, height)};

    if (!tex->initialize(screen::get_renderer())) {
        spdlog::error("failed to initialize texture '{}'<-'{}'", name,
                      path.string());
        return false;
    }

    textures.emplace(name, std::move(tex));
    return true;
}

std::optional<const core::Texture2D *> get_texture(const std::string &name) {
    const auto iter{textures.find(name)};
    ASSERT_RET_MSG(iter != textures.end(), std::nullopt,
                   "texture '%s' not found", name.c_str());
    return iter->second;
}

bool remove_texture(const std::string &name) {
    const auto iter{textures.find(name)};
    if (iter == textures.end()) {
        spdlog::error("failed to remove texture: '{}'", name);
        return false;
    }
    textures.erase(iter);
    delete iter->second;
    return true;
}

void load_tilemap(const std::filesystem::path &path, const std::string &tex,
                  u32 tile_width, u32 tile_height, u32 tile_scale,
                  u32 map_width, u32 map_height,
                  explore::ecs::Registry &registry) {
    std::string file_contents;
    if (!explore::file::read_all(path, file_contents)) {
        spdlog::error("Failed to read tilemap file: {}", path.string());
        return;
    }

    auto tex_opt = explore::managers::resource::get_texture(tex);
    ASSERT_RET_V(tex_opt.has_value());
    const explore::core::Texture2D *texture = tex_opt.value();

    const u32 tileset_cols = texture->get_width() / tile_width;
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

                SDL_Rect src{sdl::rect(tile_col * tile_width,
                                       tile_row * tile_height, tile_width,
                                       tile_height)};

                explore::ecs::Entity tile = registry.create_entity();

                glm::vec2 pos{static_cast<float>(x * tile_width * tile_scale),
                              static_cast<float>(y * tile_height * tile_scale)};

                glm::vec2 scale{static_cast<float>(tile_scale),
                                static_cast<float>(tile_scale)};

                tile.add_component<component::Transform>(pos, scale, 0.f);
                tile.add_component<component::Sprite>(tex, tile_width,
                                                      tile_height, src);
            }

            ++x;
        }

        ++y;
    }

    spdlog::info("Tilemap loaded: {}x{} tiles from '{}'", map_width, map_height,
                 path.string());
}

void destroy() {
    spdlog::trace("clearing all resources");
    for (auto tex : textures) {
        delete tex.second;
    };
    textures.clear();
}
}  // namespace explore::managers::resource
