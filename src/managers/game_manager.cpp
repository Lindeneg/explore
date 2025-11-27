#include "game_manager.h"

#include <SDL2/SDL_image.h>
#include <SDL_timer.h>
#include <spdlog/spdlog.h>

#include "../core/file.h"
#include "../core/game_context.h"
#include "../core/rect.h"
#include "../core/tilemap.h"
#include "../ecs/components.h"
#include "../events/key_pressed.h"
#include "../systems/animation.h"
#include "../systems/camera_movement.h"
#include "../systems/collision.h"
#include "../systems/damage.h"
#include "../systems/debug_render.h"
#include "../systems/keyboard.h"
#include "../systems/movement.h"
#include "../systems/projectile_emit.h"
#include "../systems/projectile_lifecycle.h"
#include "../systems/render.h"

namespace explore::manager {

bool GameManager::initialize() {
    if (!_screen_manager.initialize()) {
        return false;
    }

    _resource_manager.set_renderer(_screen_manager.get_renderer());

    auto dimensions{_screen_manager.get_dimensions()};

    _camera.x = 0;
    _camera.y = 0;
    _camera.w = dimensions.x;
    _camera.h = dimensions.y;

    // TODO instead of keeping track of window dimensions
    // multiple places, have a single source of truth
    _game_context.window_width = dimensions.x;
    _game_context.window_height = dimensions.y;

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
    _registry.add_system<system::CameraMovement>();
    _registry.add_system<system::ProjectileEmit>();
    _registry.add_system<system::ProjectileLifecycle>();

    _resource_manager.add_texture(
        "tank-tex", FPATH("assets", "images", "tank-panther-right.png"));

    _resource_manager.add_texture(
        "truck-tex", FPATH("assets", "images", "truck-ford-right.png"));

    _resource_manager.add_texture(
        "chopper-tex", FPATH("assets", "images", "chopper-spritesheet.png"));

    _resource_manager.add_texture("radar-tex",
                                  FPATH("assets", "images", "radar.png"));

    _resource_manager.add_texture("bullet-tex",
                                  FPATH("assets", "images", "bullet.png"));

    _resource_manager.add_texture("jungle",
                                  FPATH("assets", "tilemaps", "jungle.png"));

    _resource_manager.add_tilemap(_registry, "tilemap", 32u, 32u, 3u);

    _load_level(1u);
}

void GameManager::_load_level(const u32 level) {
    _resource_manager.load_tilemap(
        "tilemap", FPATH("assets", "tilemaps", "jungle.map"), "jungle");

    auto map_size{_resource_manager.loaded_tilemap_dimensions()};

    _game_context.map_width = map_size.x;
    _game_context.map_height = map_size.y;

    ecs::Entity chopper{_registry.create_entity("chopper")};
    chopper.add_component<component::Transform>(glm::vec2(10.f, 10.f),
                                                glm::vec2(1.f, 1.f), 0.f);
    chopper.add_component<component::RigidBody>(glm::vec2(0.f, 0.f));
    chopper.add_component<component::Sprite>("chopper-tex", 1u,
                                             core::rect(0, 0, 32, 32));
    chopper.add_component<component::Animation>(2u, 15u, true);

    chopper.add_component<component::KeyboardControl>(
        glm::vec2(0, -50), glm::vec2(50, 0), glm::vec2(0, 50),
        glm::vec2(-50, 0));
    chopper.add_component<component::CameraFollow>();
    chopper.add_component<component::Health>(100u);
    chopper.add_component<component::ProjectileEmitter>(glm::vec2(150.0, 150.0),
                                                        0u, 10000u, 0u, true);

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
    tank.add_component<component::RigidBody>(glm::vec2(0.f, 0.f));
    tank.add_component<component::Sprite>("tank-tex", 2u,
                                          core::rect(0, 0, 32, 32));
    tank.add_component<component::BoxCollider>(32u, 32u);
    tank.add_component<component::ProjectileEmitter>(glm::vec2(100.0, 0.0),
                                                     5000u, 3000u, 0u, false);
    tank.add_component<component::Health>(100u);

    ecs::Entity truck{_registry.create_entity("truck")};
    truck.add_component<component::Transform>(glm::vec2(10.f, 10.f),
                                              glm::vec2(1.f, 1.f), 0.f);
    truck.add_component<component::RigidBody>(glm::vec2(0.f, 0.f));
    truck.add_component<component::Sprite>("truck-tex", 2u,
                                           core::rect(0, 0, 32, 32));
    truck.add_component<component::BoxCollider>(32u, 32u);
    truck.add_component<component::ProjectileEmitter>(glm::vec2(0, 100.0),
                                                      2000u, 5000u, 0u, false);
    truck.add_component<component::Health>(100u);
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
    _registry.get_system<system::ProjectileEmit>().subscribe_to_events(
        _event_bus);

    _registry.get_system<system::Movement>().update(_game_context.delta_time);
    _registry.get_system<system::Animation>().update();
    _registry.get_system<system::Collision>().update(_event_bus);
    _registry.get_system<system::ProjectileEmit>().update(_registry);
    _registry.get_system<system::ProjectileLifecycle>().update();
    _registry.get_system<system::CameraMovement>().update(_camera,
                                                          _game_context);

    _registry.update();
}

void GameManager::_render() {
    _screen_manager.set_draw_color(color::black);
    _screen_manager.clear();

    _registry.get_system<system::Render>().update(_screen_manager,
                                                  _resource_manager, _camera);

    if (_game_context.draw_collision_rects) {
        _registry.get_system<system::DebugRender>().update(_screen_manager);
    }

    _screen_manager.present();
}
}  // namespace explore::manager

