#ifndef EXPLORE_MANAGERS_GAME_MANAGER_H
#define EXPLORE_MANAGERS_GAME_MANAGER_H

#include <SDL2/SDL_events.h>

#include "../common.h"
#include "../core/game_context.h"
#include "../ecs/ecs.h"
#include "../events/bus.h"
#include "./resource_manager.h"
#include "./screen_manager.h"

namespace explore::manager {
class GameManager {
   private:
    bool _running;
    SDL_Event _sdl_event;
    SDL_Rect _camera;

    core::GameContext _game_context;
    ecs::Registry _registry;
    event::Bus _event_bus;
    manager::ScreenManager _screen_manager;
    manager::ResourceManager _resource_manager;

   public:
    GameManager() = default;
    ~GameManager() = default;

    bool initialize();
    void run();

   private:
    void _setup();
    void _load_level(u32 level);
    void _process_input();
    void _update();
    void _render();
};

}  // namespace explore::manager

#endif  // EXPLORE_MANAGERS_GAME_MANAGER_H
