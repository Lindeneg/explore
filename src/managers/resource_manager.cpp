#include "resource_manager.h"

#include <SDL_render.h>
#include <SDL_surface.h>
#include <spdlog/spdlog.h>

#include <string>
#include <unordered_map>

#include "../core/texture2d.h"
#include "./screen_manager.h"

static std::unordered_map<std::string, const explore::core::Texture2D *>
    textures{};

namespace explore::managers::resource {
bool add_texture(const std::string &name, const std::filesystem::path &path,
                 const u32 width, const u32 height) {
    auto iter{textures.find(name)};
    if (iter != textures.end()) {
        spdlog::warn("texture '{}' has already been added", name);
        return true;
    }
    auto *tex{new core::Texture2D(name, path, width, height)};

    if (!tex->initialize(screen::get_renderer())) {
        spdlog::error("failed to initialize texture '{}", name);
        delete tex;
        return false;
    }

    textures.emplace(name, tex);
    return true;
}

const core::Texture2D *get_texture(const std::string &name) {
    auto iter{textures.find(name)};
    ASSERT_RET_MSG(iter != textures.end(), nullptr, "texture '%s' not found",
                   name.c_str());
    return iter->second;
}

bool remove_texture(const std::string &name) {
    auto iter{textures.find(name)};
    if (iter == textures.end()) {
        spdlog::error("failed to remove texture: '{}'", name);
        return false;
    }
    textures.erase(iter);
    delete iter->second;
    return true;
}

void destroy() {
    spdlog::trace("clearing all resources");
    for (const auto &iter : textures) {
        delete iter.second;
    }
    textures.clear();
}
}  // namespace explore::managers::resource
