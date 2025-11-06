#include "game_manager.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL_timer.h>
#include <spdlog/spdlog.h>

#include "../common.h"
#include "../core/file.h"
#include "../core/game_context.h"
#include "../core/rect.h"
#include "../ecs/components.h"
#include "../ecs/ecs.h"
#include "../events/bus.h"
#include "../events/key_pressed.h"
#include "../systems/animation.h"
#include "../systems/collision.h"
#include "../systems/damage.h"
#include "../systems/debug_render.h"
#include "../systems/keyboard.h"
#include "../systems/movement.h"
#include "../systems/render.h"
#include "./resource_manager.h"
#include "./screen_manager.h"

static bool is_running{false};
static SDL_Event sdl_event{};
static explore::core::GameContext game_context{};
static explore::ecs::Registry registry{};
static explore::event::Bus event_bus{};
static explore::manager::ScreenManager screen_manager{};
static explore::manager::ResourceManager resource_manager{};

bool explore::manager::game::initialize() {
    if (!screen_manager.initialize()) {
        return false;
    }
    // TODO refactor so this isn't needed it's only used to import
    // an image via sdl instead create an img loader class and use that instead
    resource_manager.set_renderer(screen_manager.get_renderer());
    is_running = true;
    return true;
}

void explore::manager::game::setup() {
    game_context.capped_frame_rate = true;
    game_context.sample_fps = true;
    game_context.draw_collision_rects = true;

    registry.add_system<system::Movement>();
    registry.add_system<system::Render>();
    registry.add_system<system::Animation>();
    registry.add_system<system::Collision>();
    registry.add_system<system::DebugRender>();
    registry.add_system<system::Damage>();
    registry.add_system<system::Keyboard>();

    resource_manager.add_texture(
        "tank-tex", FPATH("assets", "images", "tank-panther-right.png"));

    resource_manager.add_texture(
        "truck-tex", FPATH("assets", "images", "truck-ford-right.png"));

    resource_manager.add_texture("chopper-tex",
                                 FPATH("assets", "images", "chopper.png"));

    resource_manager.add_texture("radar-tex",
                                 FPATH("assets", "images", "radar.png"));

    resource_manager.add_texture("tile-map",
                                 FPATH("assets", "tilemaps", "jungle.png"));

    load_level(1u);
}

void explore::manager::game::load_level(const u32 level) {
    resource_manager.load_tilemap(FPATH("assets", "tilemaps", "jungle.map"),
                                  "tile-map", 32u, 32u, 2u, 25u, 20u, registry);

    ecs::Entity chopper{registry.create_entity("chopper")};
    chopper.add_component<component::Transform>(glm::vec2(10.f, 10.f),
                                                glm::vec2(1.f, 1.f), 0.f);
    chopper.add_component<component::RigidBody>(glm::vec2(0.f, 0.f));
    chopper.add_component<component::Sprite>("chopper-tex", 1u,
                                             core::rect(0, 0, 32, 32));
    chopper.add_component<component::Animation>(2u, 15u, true);

    ecs::Entity radar{registry.create_entity("radar")};
    radar.add_component<component::Transform>(
        glm::vec2(screen_manager.get_dimensions().x - 72.f, 8.f),
        glm::vec2(1.f, 1.f), 0.f);
    radar.add_component<component::RigidBody>(glm::vec2(0.f, 0.f));
    radar.add_component<component::Sprite>("radar-tex", 2u,
                                           core::rect(0, 0, 64, 64));
    radar.add_component<component::Animation>(8u, 5u, true);

    ecs::Entity tank{registry.create_entity("tank")};
    tank.add_component<component::Transform>(glm::vec2(250.f, 10.f),
                                             glm::vec2(2.f, 2.f), 0.f);
    tank.add_component<component::RigidBody>(glm::vec2(-30.f, 0.f));
    tank.add_component<component::Sprite>("tank-tex", 2u,
                                          core::rect(0, 0, 32, 32));
    tank.add_component<component::BoxCollider>(32u, 32u);

    ecs::Entity truck{registry.create_entity("truck")};
    truck.add_component<component::Transform>(glm::vec2(10.f, 10.f),
                                              glm::vec2(1.f, 1.f), 0.f);
    truck.add_component<component::RigidBody>(glm::vec2(20.f, 0.f));
    truck.add_component<component::Sprite>("truck-tex", 2u,
                                           core::rect(0, 0, 32, 32));
    truck.add_component<component::BoxCollider>(32u, 32u);
}

void explore::manager::game::run() {
    setup();
    while (is_running) {
        process_input();
        update();
        render();
        //        spdlog::debug("FPS:{}", game_context.FPS());
    }
}

void explore::manager::game::process_input() {
    while (SDL_PollEvent(&sdl_event)) {
        switch (sdl_event.type) {
            case SDL_QUIT:
                is_running = false;
                break;
            case SDL_KEYDOWN:
                if (sdl_event.key.keysym.sym == SDLK_d) {
                    game_context.draw_collision_rects =
                        !game_context.draw_collision_rects;
                }
                if (sdl_event.key.keysym.sym == SDLK_ESCAPE) {
                    is_running = false;
                }
                event_bus.emit<event::KeyPressed>(sdl_event.key.keysym.sym);
                break;
            default:
                break;
        }
    }
}

void explore::manager::game::update() {
    game_context.update_delta_time();

    event_bus.reset();

    registry.get_system<system::Damage>().subscribe_to_events(event_bus);
    registry.get_system<system::Keyboard>().subscribe_to_events(event_bus);

    registry.get_system<system::Movement>().update(game_context.delta_time);
    registry.get_system<system::Animation>().update();
    registry.get_system<system::Collision>().update(event_bus);

    registry.update();
}

void explore::manager::game::render() {
    screen_manager.set_draw_color(color::black);
    screen_manager.clear();

    registry.get_system<system::Render>().update(screen_manager,
                                                 resource_manager);

    if (game_context.draw_collision_rects) {
        registry.get_system<system::DebugRender>().update(screen_manager);
    }

    screen_manager.present();
}

