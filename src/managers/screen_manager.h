#ifndef EXPLORE_MANAGERS_SCREEN_MANAGER_H
#define EXPLORE_MANAGERS_SCREEN_MANAGER_H

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include <glm/glm.hpp>

#include "../common.h"

namespace explore::core {
class Texture2D;
}

namespace explore::managers::screen {
bool initialize();

void set_draw_color(Color color);

void draw_rect(const SDL_Rect &dst, const Color color = color::white);

void draw_texture(const core::Texture2D &tex, SDL_Rect dst, f32 angle);

void draw_texture(const core::Texture2D &tex, SDL_Rect src, SDL_Rect dst,
                  f32 angle);

void clear();

void present();

glm::ivec2 get_dimensions();

SDL_Window *get_window();

SDL_Renderer *get_renderer();

void destroy();
}  // namespace explore::managers::screen

#endif  // EXPLORE_MANAGERS_SCREEN_MANAGER_H
