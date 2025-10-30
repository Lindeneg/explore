#ifndef EXPLORE_ECS_COMPONENTS_H_
#define EXPLORE_ECS_COMPONENTS_H_

#include <glm/fwd.hpp>
#include <glm/glm.hpp>

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
    u32 width;
    u32 height;

    Sprite(u32 width = 0, u32 height = 0) : width(width), height(height) {}
};

}  // namespace explore::component

#endif  // EXPLORE_ECS_COMPONENTS_H_
