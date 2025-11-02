#include "screen_manager.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <spdlog/spdlog.h>

#include "../core/texture2d.h"
#include "resource_manager.h"

#define FILL_RECT_P(tex, rect)        \
    if (rect.w == 0 && rect.h == 0) { \
        rect.w = tex->get_width();    \
        rect.h = tex->get_height();   \
    }

#define FILL_RECT(tex, rect)          \
    if (rect.w == 0 && rect.h == 0) { \
        rect.w = tex.get_width();     \
        rect.h = tex.get_height();    \
    }

static u32 sdl_subsystem_flags{SDL_INIT_VIDEO | SDL_INIT_TIMER |
                               SDL_INIT_EVENTS};

static SDL_Window *window{nullptr};
static SDL_Renderer *renderer{nullptr};
static SDL_DisplayMode display_mode{};
static auto dimensions{glm::ivec2(0, 0)};

static bool initialize_sdl() {
    if (SDL_WasInit(sdl_subsystem_flags)) {
        spdlog::warn("SDL has already been initialized");
        return true;
    }
    if (SDL_Init(sdl_subsystem_flags) != 0) {
        spdlog::error("failed to initialize SDL {0}", SDL_GetError());
        return false;
    }
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0) {
        spdlog::error("failed to initialize SDL Image {0}", SDL_GetError());
        return false;
    }
    return true;
}

bool explore::managers::screen::initialize() {
    if (!initialize_sdl()) {
        return false;
    }
    SDL_GetCurrentDisplayMode(0, &display_mode);
    dimensions.x = 1600;
    dimensions.y = 1024;
    //    dimensions.x = display_mode.w;
    //    dimensions.y = display_mode.h;

    if (!window) {
        window = SDL_CreateWindow("Explore", SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED, dimensions.x,
                                  dimensions.y, SDL_WINDOW_BORDERLESS);
        if (!window) {
            spdlog::error("failed to create SDL window {0}", SDL_GetError());
            return false;
        }
        spdlog::trace("initialized SDL window ({0:d},{1:d})", dimensions.x,
                      dimensions.y);
    }
    if (!renderer) {
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            spdlog::error("failed to create SDL renderer {0}", SDL_GetError());
            SDL_DestroyWindow(window);
            window = nullptr;
            return false;
        }
        spdlog::trace("initialized SDL renderer");
    }
    return true;
}

void explore::managers::screen::set_draw_color(const Color color) {
    ASSERT_RET_V(renderer);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void explore::managers::screen::draw_rect(const SDL_Rect &dst,
                                          const Color color) {
    ASSERT_RET_V(renderer);
    set_draw_color(color);
    SDL_RenderFillRect(renderer, &dst);
}

void explore::managers::screen::draw_rect_outline(const SDL_Rect &dst,
                                                  const Color color) {
    ASSERT_RET_V(renderer);
    set_draw_color(color);
    SDL_RenderDrawRect(renderer, &dst);
}

void explore::managers::screen::draw_texture(const core::Texture2D &tex,
                                             SDL_Rect dst, f32 angle) {
    ASSERT_RET_V(renderer);
    FILL_RECT(tex, dst)
    SDL_RenderCopyEx(renderer, tex.get_data(), nullptr, &dst, angle, nullptr,
                     SDL_FLIP_NONE);
}

void explore::managers::screen::draw_texture(const core::Texture2D &tex,
                                             SDL_Rect src, SDL_Rect dst,
                                             f32 angle) {
    ASSERT_RET_V(renderer);
    FILL_RECT(tex, src)
    FILL_RECT(tex, dst)
    SDL_RenderCopyEx(renderer, tex.get_data(), &src, &dst, angle, nullptr,
                     SDL_FLIP_NONE);
}

void explore::managers::screen::clear() {
    ASSERT_RET_V(renderer);
    set_draw_color(color::black);
    SDL_RenderClear(renderer);
}

void explore::managers::screen::present() {
    ASSERT_RET_V(renderer);
    SDL_RenderPresent(renderer);
}

glm::ivec2 explore::managers::screen::get_dimensions() { return dimensions; }

SDL_Window *explore::managers::screen::get_window() {
    ASSERT_RET(window, nullptr);
    return window;
}

SDL_Renderer *explore::managers::screen::get_renderer() {
    ASSERT_RET(renderer, nullptr);
    return renderer;
}

void explore::managers::screen::destroy() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
        spdlog::trace("destroyed SDL renderer");
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
        spdlog::trace("destroyed SDL window");
    }
    IMG_Quit();
    SDL_QuitSubSystem(sdl_subsystem_flags);
    SDL_Quit();
}
