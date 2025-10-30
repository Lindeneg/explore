#include "resource_manager.h"

#include <SDL_render.h>
#include <spdlog/spdlog.h>

#include <optional>
#include <string>
#include <unordered_map>

#include "../core/texture2d.h"
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
    return true;
}

void destroy() {
    spdlog::trace("clearing all resources");
    textures.clear();
}
}  // namespace explore::managers::resource
