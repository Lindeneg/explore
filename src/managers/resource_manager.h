#ifndef EXPLORE_MANAGERS_RESOURCE_MANAGER_H_
#define EXPLORE_MANAGERS_RESOURCE_MANAGER_H_

#include <filesystem>
#include <string>

#include "../common.h"

namespace explore::core {
struct Texture2D;
}

namespace explore::managers::resource {
bool add_texture(const std::string &name, const std::filesystem::path &path,
                 const u32 width, const u32 height);

const core::Texture2D *get_texture(const std::string &name);

bool remove_texture(const std::string &name);

void destroy();
}  // namespace explore::managers::resource

#endif  // EXPLORE_MANAGERS_RESOURCE_MANAGER_H_
