#ifndef EXPLORE_MANAGERS_RESOURCE_MANAGER_H_
#define EXPLORE_MANAGERS_RESOURCE_MANAGER_H_

#include <filesystem>
#include <optional>
#include <string>

#include "../common.h"

namespace explore::core {
class Texture2D;
}

namespace explore::ecs {
class Registry;
}

namespace explore::managers::resource {
bool add_texture(const std::string &name, const std::filesystem::path &path);

std::optional<const core::Texture2D *> get_texture(const std::string &name);

bool remove_texture(const std::string &name);

void load_tilemap(const std::filesystem::path &path, const std::string &tex,
                  u32 tile_width, u32 tile_height, u32 tile_scale,
                  u32 map_width, u32 map_height,
                  explore::ecs::Registry &registry);

void destroy();
}  // namespace explore::managers::resource

#endif  // EXPLORE_MANAGERS_RESOURCE_MANAGER_H_
