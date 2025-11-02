#ifndef EXPLORE_ECS_COMPONENTS_H_
#define EXPLORE_ECS_COMPONENTS_H_

#include <SDL_rect.h>
#include <SDL_timer.h>

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

struct Animation {
    u32 num_frames;
    u32 current_frame;
    u32 speed_rate;
    u32 start_time;
    bool is_loop;

    Animation(u32 num_frames = 1, u32 speed_rate = 1, bool is_loop = true)
        : num_frames(num_frames),
          current_frame(1),
          speed_rate(speed_rate),
          start_time(SDL_GetTicks()),
          is_loop(is_loop) {}
};

struct BoxCollider {
    u32 width;
    u32 height;
    glm::vec2 offset;

    BoxCollider(u32 width = 0, u32 height = 0, glm::vec2 offset = glm::vec2(0))
        : width(width), height(height), offset(offset) {}
};

}  // namespace explore::component

#endif  // EXPLORE_ECS_COMPONENTS_H_
