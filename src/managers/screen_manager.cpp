#include "screen_manager.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <spdlog/spdlog.h>

#include "../core/texture2d.h"
#include "resource_manager.h"

static u32 sdl_subsystem_flags{SDL_INIT_VIDEO | SDL_INIT_TIMER |
                               SDL_INIT_EVENTS};

namespace explore::manager {

ScreenManager::ScreenManager()
    : _window(nullptr),
      _renderer(nullptr),
      _display_mode({}),
      _dimensions(glm::ivec2(0)) {}

ScreenManager::~ScreenManager() { destroy(); }

bool ScreenManager::_initialize_sdl() {
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

glm::ivec2 ScreenManager::get_dimensions() const { return _dimensions; }

SDL_Window *ScreenManager::get_window() const {
    ASSERT_RET(_window, nullptr);
    return _window;
}

SDL_Renderer *ScreenManager::get_renderer() const {
    ASSERT_RET(_renderer, nullptr);
    return _renderer;
}

bool ScreenManager::initialize() {
    if (!_initialize_sdl()) {
        return false;
    }
    SDL_GetCurrentDisplayMode(0, &_display_mode);
    _dimensions.x = 1600;
    _dimensions.y = 1024;
    //    dimensions.x = display_mode.w;
    //    dimensions.y = display_mode.h;

    if (!_window) {
        _window = SDL_CreateWindow("Explore", SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED, _dimensions.x,
                                   _dimensions.y, SDL_WINDOW_BORDERLESS);
        if (!_window) {
            spdlog::error("failed to create SDL window {0}", SDL_GetError());
            return false;
        }
        spdlog::trace("initialized SDL window ({0:d},{1:d})", _dimensions.x,
                      _dimensions.y);
    }
    if (!_renderer) {
        _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
        if (!_renderer) {
            spdlog::error("failed to create SDL renderer {0}", SDL_GetError());
            SDL_DestroyWindow(_window);
            _window = nullptr;
            return false;
        }
        spdlog::trace("initialized SDL renderer");
    }
    return true;
}

void ScreenManager::set_draw_color(const Color color) {
    ASSERT_RET_V(_renderer);
    SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
}

void ScreenManager::draw_rect(const SDL_Rect &dst, const Color color) {
    ASSERT_RET_V(_renderer);
    set_draw_color(color);
    SDL_RenderFillRect(_renderer, &dst);
}

void ScreenManager::draw_rect_outline(const SDL_Rect &dst, const Color color) {
    ASSERT_RET_V(_renderer);
    set_draw_color(color);
    SDL_RenderDrawRect(_renderer, &dst);
}

void ScreenManager::draw_texture(const core::Texture2D &tex, SDL_Rect dst,
                                 f32 angle) const {
    ASSERT_RET_V(_renderer);
    SDL_RenderCopyEx(_renderer, tex.get_data(), nullptr, &dst, angle, nullptr,
                     SDL_FLIP_NONE);
}

void ScreenManager::draw_texture(const core::Texture2D &tex, SDL_Rect src,
                                 SDL_Rect dst, f32 angle) const {
    ASSERT_RET_V(_renderer);
    SDL_RenderCopyEx(_renderer, tex.get_data(), &src, &dst, angle, nullptr,
                     SDL_FLIP_NONE);
}

void ScreenManager::clear() {
    ASSERT_RET_V(_renderer);
    set_draw_color(color::black);
    SDL_RenderClear(_renderer);
}

void ScreenManager::present() const {
    ASSERT_RET_V(_renderer);
    SDL_RenderPresent(_renderer);
}

void ScreenManager::destroy() {
    if (_renderer) {
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
        spdlog::trace("destroyed SDL _renderer");
    }
    if (_window) {
        SDL_DestroyWindow(_window);
        _window = nullptr;
        spdlog::trace("destroyed SDL window");
    }
    IMG_Quit();
    SDL_QuitSubSystem(sdl_subsystem_flags);
    SDL_Quit();
}

}  // namespace explore::manager

