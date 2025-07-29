#include "screen_manager.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <spdlog/spdlog.h>

internal u32 sdl_subsystem_flags{SDL_INIT_VIDEO | SDL_INIT_TIMER |
                                 SDL_INIT_EVENTS};

internal SDL_Window *window{nullptr};
internal SDL_Renderer *renderer{nullptr};
internal SDL_DisplayMode display_mode{};
internal auto dimensions{glm::ivec2(0, 0)};

internal bool initialize_sdl() {
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
    dimensions.x = display_mode.w;
    dimensions.y = display_mode.h;
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

void explore::managers::screen::draw_texture(SDL_Texture *texture,
                                             const SDL_Rect *dst) {
    draw_texture(texture, nullptr, dst);
}

void explore::managers::screen::draw_texture(SDL_Texture *texture,
                                             const i32 x_dst, const i32 y_dst,
                                             const i32 w_dst, const i32 h_dst) {
    const SDL_Rect dst{x_dst, y_dst, w_dst, h_dst};
    draw_texture(texture, nullptr, &dst);
}

void explore::managers::screen::draw_texture(SDL_Texture *texture,
                                             const i32 x_src, const i32 y_src,
                                             const i32 w_src, const i32 h_src,
                                             const i32 x_dst, const i32 y_dst,
                                             const i32 w_dst, const i32 h_dst) {
    const SDL_Rect src{x_src, y_src, w_src, h_src};
    const SDL_Rect dst{x_dst, y_dst, w_dst, h_dst};
    draw_texture(texture, &src, &dst);
}

void explore::managers::screen::draw_texture(SDL_Texture *texture,
                                             const SDL_Rect *src,
                                             const SDL_Rect *dst) {
    ASSERT_RET_V(renderer);
    ASSERT_RET_V(texture);
    SDL_RenderCopy(renderer, texture, src, dst);
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
