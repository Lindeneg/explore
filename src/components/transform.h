#ifndef EXPLORE_COMPONENTS_TRANSFORM_H_
#define EXPLORE_COMPONENTS_TRANSFORM_H_

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
}  // namespace explore::component

#endif  // EXPLORE_COMPONENTS_TRANSFORM_H_
