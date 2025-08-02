#include "game_manager.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <spdlog/spdlog.h>

#include "../common.h"
#include "../core/context.h"
#include "../core/file.h"
#include "./resource_manager.h"
#include "./screen_manager.h"

static bool is_running{false};
static bool do_cap_frame_rate{false};
static SDL_Event event{};
static explore::core::GameContext game_context{};

static glm::vec2 pos;
static glm::vec2 vel;

bool explore::managers::game::initialize() {
    if (!screen::initialize()) {
        return false;
    }
    is_running = true;
    return true;
}

void explore::managers::game::setup() {
    pos = glm::vec2(10, 20);
    vel = glm::vec2(25.0, 25.0);

    resource::add_texture(
        "tank", FPATH("assets", "images", "tank-tiger-right.png"), 32, 32);
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

void explore::managers::game::update() {
    game_context.update_delta_time();
    pos.x += vel.x * game_context.delta_time;
    pos.y += vel.y * game_context.delta_time;
}

void explore::managers::game::render() {
    screen::set_draw_color(color::black);
    screen::clear();

    screen::draw_texture("tank", SDL_Rect{static_cast<int>(pos.x),
                                          static_cast<int>(pos.y), 64, 64});

    screen::present();
}

void explore::managers::game::destroy() {
    resource::destroy();
    screen::destroy();
}
