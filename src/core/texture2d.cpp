#include "texture2d.h"

#include <SDL_image.h>
#include <SDL_render.h>
#include <SDL_surface.h>

namespace explore::core {
Texture2D::Texture2D()
    : _name(""), _path(""), _data(nullptr), _width(0), _height(0) {}

Texture2D::Texture2D(std::string name, std::filesystem::path path)
    : _name(std::move(name)),
      _path(std::move(path)),
      _data(nullptr),
      _width(0),
      _height(0) {}

bool Texture2D::initialize(SDL_Renderer *renderer) {
    SDL_Surface *surface{IMG_Load(_path.string().c_str())};
    ASSERT_RET(surface, false);

    SDL_Texture *sdl_texture{SDL_CreateTextureFromSurface(renderer, surface)};
    ASSERT_RET(sdl_texture, false);

    _width = surface->w;
    _height = surface->h;

    SDL_FreeSurface(surface);

    spdlog::trace("initialized texture '{}' with path '{}'", _name,
                  _path.string());

    _data = sdl_texture;
    return true;
}

Texture2D::~Texture2D() {
    spdlog::trace("destroying texture: '{}'", _name);
    SDL_DestroyTexture(_data);
    _data = nullptr;
}

}  // namespace explore::core
