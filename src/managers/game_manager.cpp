#include "game_manager.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL_timer.h>
#include <spdlog/spdlog.h>

#include "../common.h"
#include "../core/file.h"
#include "../core/game_context.h"
#include "../ecs/components.h"
#include "../ecs/ecs.h"
#include "../systems/movement.h"
#include "../systems/render.h"
#include "./resource_manager.h"
#include "./screen_manager.h"

static bool is_running{false};
static SDL_Event event{};
static explore::core::GameContext game_context{};
static explore::ecs::Registry registry{};

bool explore::managers::game::initialize() {
    if (!screen::initialize()) {
        return false;
    }
    is_running = true;
    return true;
}

void explore::managers::game::setup() {
    game_context.capped_frame_rate = false;
    game_context.sample_fps = true;

    registry.add_system<system::Movement>();
    registry.add_system<system::Render>();

    resource::add_texture("tank-tex",
                          FPATH("assets", "images", "tank-panther-right.png"),
                          32, 32);

    resource::add_texture(
        "truck-tex", FPATH("assets", "images", "truck-ford-right.png"), 32, 32);

    resource::add_texture("tile-map", FPATH("assets", "tilemaps", "jungle.png"),
                          320, 96);

    resource::load_tilemap(FPATH("assets", "tilemaps", "jungle.map"),
                           "tile-map", 32u, 32u, 2u, 25u, 20u, registry);

    ecs::Entity tank{registry.create_entity("tank")};
    tank.add_component<component::Transform>(glm::vec2(10.f, 10.f),
                                             glm::vec2(1.f, 1.f), 0.f);

    tank.add_component<component::RigidBody>(glm::vec2(30.f, 0.f));

    tank.add_component<component::Sprite>("tank-tex", 1u);

    ecs::Entity truck{registry.create_entity("truck")};
    truck.add_component<component::Transform>(glm::vec2(10.f, 10.f),
                                              glm::vec2(1.f, 1.f), 0.f);

    truck.add_component<component::RigidBody>(glm::vec2(20.f, 0.f));

    truck.add_component<component::Sprite>("truck-tex", 2u);
}

void explore::managers::game::load_level(const u32 level) {}

void explore::managers::game::run() {
    setup();
    while (is_running) {
        process_input();
        update();
        render();
        //        spdlog::debug("FPS:{}", game_context.FPS());
    }
}

void explore::managers::game::process_input() {
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                is_running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_d) {
                    game_context.draw_collision_rects =
                        !game_context.draw_collision_rects;
                }
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    is_running = false;
                }
                break;
            default:
                break;
        }
    }
}

void explore::managers::game::update() {
    game_context.update_delta_time();

    registry.get_system<system::Movement>().update(game_context.delta_time);

    registry.update();
}

void explore::managers::game::render() {
    screen::set_draw_color(color::black);
    screen::clear();
    registry.get_system<system::Render>().update(game_context);
    screen::present();
}

void explore::managers::game::destroy() {
    resource::destroy();
    screen::destroy();
}
