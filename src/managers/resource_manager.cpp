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
                  u32 tile_width, u32 tile_height,
                  explore::ecs::Registry &registry) {
    std::string file_contents;
    if (!explore::file::read_all(path, file_contents)) {
        spdlog::error("Failed to read tilemap file: {}", path.string());
        return;
    }

    std::stringstream ss(file_contents);
    std::string line;
    int y = 0;

    // Determine how many columns are in the tileset texture
    auto tex_opt = explore::managers::resource::get_texture(tex);
    ASSERT_RET_V(tex_opt.has_value());
    auto texture = tex_opt.value();

    int tileset_columns = texture->get_width() / tile_width;

    while (std::getline(ss, line)) {
        std::stringstream line_stream(line);
        std::string value;
        int x = 0;

        while (std::getline(line_stream, value, ',')) {
            int tile_index = std::stoi(value);

            if (tile_index >= 0) {  // skip empty tiles if desired
                auto src{sdl::rect((tile_index % tileset_columns) * tile_width,
                                   (tile_index / tileset_columns) * tile_height,
                                   tile_width, tile_height)};

                // create tile entity
                ecs::Entity tile = registry.create_entity();
                tile.add_component<component::Transform>(
                    glm::vec2(x * tile_width, y * tile_height),
                    glm::vec2(1.f, 1.f), 0.f);

                tile.add_component<component::Sprite>(tex, tile_width,
                                                      tile_height, src);
            }
            ++x;
        }
        ++y;
    }

    spdlog::info("Loaded tilemap '{}' with {} rows", path.string(), y);
}

void destroy() {
    spdlog::trace("clearing all resources");
    for (auto tex : textures) {
        delete tex.second;
    };
    textures.clear();
}
}  // namespace explore::managers::resource
