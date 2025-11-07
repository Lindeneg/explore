#include "game_manager.h"

#include <SDL2/SDL_image.h>
#include <SDL_timer.h>
#include <spdlog/spdlog.h>

#include "../core/file.h"
#include "../core/game_context.h"
#include "../core/rect.h"
#include "../ecs/components.h"
#include "../events/key_pressed.h"
#include "../systems/animation.h"
#include "../systems/collision.h"
#include "../systems/damage.h"
#include "../systems/debug_render.h"
#include "../systems/keyboard.h"
#include "../systems/movement.h"
#include "../systems/render.h"

namespace explore::manager {

bool GameManager::initialize() {
    if (!_screen_manager.initialize()) {
        return false;
    }
    _resource_manager.set_renderer(_screen_manager.get_renderer());
    _running = true;
    return true;
}

void GameManager::run() {
    _setup();
    while (_running) {
        _process_input();
        _update();
        _render();
        //        spdlog::debug("FPS:{}", game_context.FPS());
    }
}

void GameManager::_setup() {
    _game_context.capped_frame_rate = true;
    _game_context.sample_fps = true;
    _game_context.draw_collision_rects = true;

    _registry.add_system<system::Movement>();
    _registry.add_system<system::Render>();
    _registry.add_system<system::Animation>();
    _registry.add_system<system::Collision>();
    _registry.add_system<system::DebugRender>();
    _registry.add_system<system::Damage>();
    _registry.add_system<system::Keyboard>();

    _resource_manager.add_texture(
        "tank-tex", FPATH("assets", "images", "tank-panther-right.png"));

    _resource_manager.add_texture(
        "truck-tex", FPATH("assets", "images", "truck-ford-right.png"));

    _resource_manager.add_texture("chopper-tex",
                                  FPATH("assets", "images", "chopper.png"));

    _resource_manager.add_texture("radar-tex",
                                  FPATH("assets", "images", "radar.png"));

    _resource_manager.add_texture("tile-map",
                                  FPATH("assets", "tilemaps", "jungle.png"));

    _load_level(1u);
}

void GameManager::_load_level(const u32 level) {
    _resource_manager.load_tilemap(FPATH("assets", "tilemaps", "jungle.map"),
                                   "tile-map", 32u, 32u, 2u, 25u, 20u,
                                   _registry);

    ecs::Entity chopper{_registry.create_entity("chopper")};
    chopper.add_component<component::Transform>(glm::vec2(10.f, 10.f),
                                                glm::vec2(1.f, 1.f), 0.f);
    chopper.add_component<component::RigidBody>(glm::vec2(0.f, 0.f));
    chopper.add_component<component::Sprite>("chopper-tex", 1u,
                                             core::rect(0, 0, 32, 32));
    chopper.add_component<component::Animation>(2u, 15u, true);

    ecs::Entity radar{_registry.create_entity("radar")};
    radar.add_component<component::Transform>(
        glm::vec2(_screen_manager.get_dimensions().x - 72.f, 8.f),
        glm::vec2(1.f, 1.f), 0.f);
    radar.add_component<component::RigidBody>(glm::vec2(0.f, 0.f));
    radar.add_component<component::Sprite>("radar-tex", 2u,
                                           core::rect(0, 0, 64, 64));
    radar.add_component<component::Animation>(8u, 5u, true);

    ecs::Entity tank{_registry.create_entity("tank")};
    tank.add_component<component::Transform>(glm::vec2(250.f, 10.f),
                                             glm::vec2(2.f, 2.f), 0.f);
    tank.add_component<component::RigidBody>(glm::vec2(-30.f, 0.f));
    tank.add_component<component::Sprite>("tank-tex", 2u,
                                          core::rect(0, 0, 32, 32));
    tank.add_component<component::BoxCollider>(32u, 32u);

    ecs::Entity truck{_registry.create_entity("truck")};
    truck.add_component<component::Transform>(glm::vec2(10.f, 10.f),
                                              glm::vec2(1.f, 1.f), 0.f);
    truck.add_component<component::RigidBody>(glm::vec2(20.f, 0.f));
    truck.add_component<component::Sprite>("truck-tex", 2u,
                                           core::rect(0, 0, 32, 32));
    truck.add_component<component::BoxCollider>(32u, 32u);
}

void GameManager::_process_input() {
    while (SDL_PollEvent(&_sdl_event)) {
        switch (_sdl_event.type) {
            case SDL_QUIT:
                _running = false;
                break;
            case SDL_KEYDOWN:
                if (_sdl_event.key.keysym.sym == SDLK_d) {
                    _game_context.draw_collision_rects =
                        !_game_context.draw_collision_rects;
                }
                if (_sdl_event.key.keysym.sym == SDLK_ESCAPE) {
                    _running = false;
                }
                _event_bus.emit<event::KeyPressed>(_sdl_event.key.keysym.sym);
                break;
            default:
                break;
        }
    }
}

void GameManager::_update() {
    _game_context.update_delta_time();

    _event_bus.reset();

    _registry.get_system<system::Damage>().subscribe_to_events(_event_bus);
    _registry.get_system<system::Keyboard>().subscribe_to_events(_event_bus);

    _registry.get_system<system::Movement>().update(_game_context.delta_time);
    _registry.get_system<system::Animation>().update();
    _registry.get_system<system::Collision>().update(_event_bus);

    _registry.update();
}

void GameManager::_render() {
    _screen_manager.set_draw_color(color::black);
    _screen_manager.clear();

    _registry.get_system<system::Render>().update(_screen_manager,
                                                  _resource_manager);

    if (_game_context.draw_collision_rects) {
        _registry.get_system<system::DebugRender>().update(_screen_manager);
    }

    _screen_manager.present();
}
}  // namespace explore::manager

