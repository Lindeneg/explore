#include "game_manager.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <spdlog/spdlog.h>

#include "../common.h"
#include "../core/context.h"
#include "../ecs/components.h"
#include "../ecs/ecs.h"
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
    ecs::Entity tank{registry.create_entity("tank")};
    //    tank.add_component<component::Transform>(glm::vec2(10.0f, 30.0f),
    //                                             glm::vec2(1.0f, 1.0f), 0.0);
    //    tank.add_component<component::RigidBody>(glm::vec2(50.0f, 0.0));
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
