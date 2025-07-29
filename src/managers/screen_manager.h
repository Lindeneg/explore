#ifndef EXPLORE_MANAGERS_SCREEN_MANAGER_H
#define EXPLORE_MANAGERS_SCREEN_MANAGER_H

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include <glm/glm.hpp>

#include "../common.h"

namespace explore::managers::screen {
bool initialize();

void set_draw_color(Color color);

void draw_texture(SDL_Texture *texture, const SDL_Rect *dst);

void draw_texture(SDL_Texture *texture, i32 x_dst, i32 y_dst, i32 w_dst,
                  i32 h_dst);

void draw_texture(SDL_Texture *texture, i32 x_src, i32 y_src, i32 w_src,
                  i32 h_src, i32 x_dst, i32 y_dst, i32 w_dst, i32 h_dst);

void draw_texture(SDL_Texture *texture, const SDL_Rect *src,
                  const SDL_Rect *dst);

void clear();

void present();

glm::ivec2 get_dimensions();

SDL_Window *get_window();

SDL_Renderer *get_renderer();

void destroy();
}  // namespace explore::managers::screen

#endif  // EXPLORE_MANAGERS_SCREEN_MANAGER_H
