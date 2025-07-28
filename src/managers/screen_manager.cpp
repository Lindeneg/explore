#include "screen_manager.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <spdlog/spdlog.h>

static uint32_t sdl_subsystem_flags{
    SDL_INIT_VIDEO | SDL_INIT_TIMER |
    SDL_INIT_EVENTS
};

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
    assert(renderer);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void explore::managers::screen::draw_texture(SDL_Texture *texture, const SDL_Rect *dst) {
    draw_texture(texture, nullptr, dst);
}

void explore::managers::screen::draw_texture(SDL_Texture *texture, const int x_dst, const int y_dst, const int w_dst,
                                             const int h_dst) {
    const SDL_Rect dst{x_dst, y_dst, w_dst, h_dst};
    draw_texture(texture, nullptr, &dst);
}

void explore::managers::screen::draw_texture(SDL_Texture *texture, const int x_src, const int y_src, const int w_src,
                                             const int h_src, const int x_dst,
                                             const int y_dst, const int w_dst,
                                             const int h_dst) {
    const SDL_Rect src{x_src, y_src, w_src, h_src};
    const SDL_Rect dst{x_dst, y_dst, w_dst, h_dst};
    draw_texture(texture, &src, &dst);
}

void explore::managers::screen::draw_texture(SDL_Texture *texture, const SDL_Rect *src, const SDL_Rect *dst) {
    assert(renderer);
    assert(texture);
    SDL_RenderCopy(renderer, texture, src, dst);
}

void explore::managers::screen::clear() {
    assert(renderer);
    set_draw_color(color::black);
    SDL_RenderClear(renderer);
}

void explore::managers::screen::present() {
    assert(renderer);
    SDL_RenderPresent(renderer);
}

glm::ivec2 explore::managers::screen::get_dimensions() { return dimensions; }

SDL_Window *explore::managers::screen::get_window() {
    assert(window);
    return window;
}

SDL_Renderer *explore::managers::screen::get_renderer() {
    assert(renderer);
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
