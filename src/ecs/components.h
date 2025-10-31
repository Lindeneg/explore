#ifndef EXPLORE_ECS_COMPONENTS_H_
#define EXPLORE_ECS_COMPONENTS_H_

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <string>

#include "../common.h"

namespace explore::component {

struct Transform {
    glm::vec2 position;
    glm::vec2 scale;
    f64 rotation;

    Transform(glm::vec2 position = {0, 0}, glm::vec2 scale = {1, 1},
              f64 rotation = 0.0)
        : position(position), scale(scale), rotation(rotation) {}
};

struct RigidBody {
    glm::vec2 velocity;

    RigidBody(glm::vec2 velocity = {0, 0}) : velocity(velocity) {}
};

struct Sprite {
    std::string texture_name;

    u32 z_index;
    u32 width;
    u32 height;

    SDL_Rect src_rect;

    // TODO: do not take a string
    Sprite(std::string texture_name = "", u32 z_index = 0, u32 width = 0,
           u32 height = 0, SDL_Rect src_rect = SDL_Rect())
        : texture_name(texture_name),
          z_index(z_index),
          width(width),
          height(height),
          src_rect(src_rect) {}
};

}  // namespace explore::component

#endif  // EXPLORE_ECS_COMPONENTS_H_
