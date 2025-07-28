#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include <glm/glm.hpp>

#include "../common.h"

namespace explore::managers::screen {
    bool initialize();

    void set_draw_color(Color color);

    void clear();

    void present();

    glm::ivec2 get_dimensions();

    SDL_Window *get_window();

    SDL_Renderer *get_renderer();

    void destroy();
} // namespace explore::managers::screen

#endif //SCREEN_MANAGER_H
