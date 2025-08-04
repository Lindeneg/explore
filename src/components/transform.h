#ifndef EXPLORE_COMPONENTS_TRANSFORM_H_
#define EXPLORE_COMPONENTS_TRANSFORM_H_

#include <glm/glm.hpp>

#include "../common.h"

namespace explore::component {
struct Transform {
    glm::vec2 position;
    glm::vec2 scale;
    f64 rotation;
};
}  // namespace explore::component

#endif  // EXPLORE_COMPONENTS_TRANSFORM_H_
