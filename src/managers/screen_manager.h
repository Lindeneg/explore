#ifndef EXPLORE_MANAGERS_SCREEN_MANAGER_H
#define EXPLORE_MANAGERS_SCREEN_MANAGER_H

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include <glm/glm.hpp>

#include "../common.h"

namespace explore::core {
class Texture2D;
}

namespace explore::manager {

class ScreenManager {
   private:
    SDL_Window *_window;
    SDL_Renderer *_renderer;
    SDL_DisplayMode _display_mode;
    glm::ivec2 _dimensions;

   private:
    bool _initialize_sdl();

   public:
    ScreenManager();
    ~ScreenManager();

    glm::ivec2 get_dimensions() const;

    SDL_Window *get_window() const;

    SDL_Renderer *get_renderer() const;

    bool initialize();

    void set_draw_color(Color color);

    void draw_rect(const SDL_Rect &dst, const Color color = color::white);

    void draw_rect_outline(const SDL_Rect &dst,
                           const Color color = color::white);

    void draw_texture(const core::Texture2D &tex, SDL_Rect dst,
                      f32 angle) const;

    void draw_texture(const core::Texture2D &tex, SDL_Rect src, SDL_Rect dst,
                      f32 angle) const;

    void clear();

    void present() const;
};

}  // namespace explore::manager

#endif  // EXPLORE_MANAGERS_SCREEN_MANAGER_H
