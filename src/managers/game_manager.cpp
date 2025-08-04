#include "game_manager.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <spdlog/spdlog.h>

#include "../common.h"
#include "../core/context.h"
#include "./resource_manager.h"
#include "./screen_manager.h"

static bool is_running{false};
static SDL_Event event{};
static explore::core::GameContext game_context{};

bool explore::managers::game::initialize() {
    if (!screen::initialize()) {
        return false;
    }
    is_running = true;
    return true;
}

void explore::managers::game::setup() {
    //    ecs::Entity tank{registry.create_entity()};
    //    tank.add_component<component::Transform>();
    //    tank.add_component<component::BoxCollider>();
    //    tank.add_component<component::Sprite>(
    //        FPATH("assets", "images", "tank-tiger-right.png"), 32, 32);
    //    resource::add_texture(
    //        "tank", FPATH("assets", "images", "tank-tiger-right.png"), 32,
    //        32);
}

void explore::managers::game::load_level(const u32 level) {}

void explore::managers::game::run() {
    setup();
    while (is_running) {
        process_input();
        update();
        render();
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

void explore::managers::game::update() { game_context.update_delta_time(); }

void explore::managers::game::render() {
    screen::set_draw_color(color::black);
    screen::clear();

    // TODO RENDER

    screen::present();
}

void explore::managers::game::destroy() {
    resource::destroy();
    screen::destroy();
}
