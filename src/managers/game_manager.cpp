#include "game_manager.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include "../common.h"
#include "screen_manager.h"

static bool is_running{false};
static bool do_cap_frame_rate{false};
static SDL_Event event{};
static float previous_frame_time{};
static explore::GameContext game_context{};

static void cap_frame_rate() {
    const int time_to_wait = static_cast<int>(
        explore::constants::FRAME_TARGET -
        (static_cast<float>(SDL_GetTicks()) - previous_frame_time));
    // only delay if too fast
    if (time_to_wait > 0 && time_to_wait < explore::constants::FRAME_TARGET) {
        SDL_Delay(time_to_wait);
    }
}

static void calculate_delta_time() {
    if (do_cap_frame_rate) {
        cap_frame_rate();
    }
    // calculate delta time
    const float delta_time =
        (static_cast<float>(SDL_GetTicks()) - previous_frame_time) / 1000.f;
    // clamp value (if running in debugger dt will be messed up)
    game_context.delta_time =
        explore::utils::greater(delta_time, explore::constants::MAXIMUM_DT)
            ? explore::constants::MAXIMUM_DT
            : delta_time;
    // update previous frame time
    previous_frame_time = static_cast<float>(SDL_GetTicks());
}

bool explore::managers::game::initialize() {
    if (!screen::initialize()) {
        return false;
    }
    is_running = true;
    return true;
}

void explore::managers::game::setup() {}

void explore::managers::game::load_level(const int level) {}

void explore::managers::game::run() {
    setup();
    // ReSharper disable once CppDFALoopConditionNotUpdated
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

void explore::managers::game::update() { calculate_delta_time(); }

void explore::managers::game::render() {
    screen::set_draw_color(color::black);
    screen::clear();

    SDL_Surface *surface{IMG_Load("assets/images/tank-tiger-right.png")};
    assert(surface);
    SDL_Texture *texture{
        SDL_CreateTextureFromSurface(screen::get_renderer(), surface)};
    SDL_FreeSurface(surface);

    screen::draw_texture(texture, 10, 10, 32, 32);

    SDL_DestroyTexture(texture);

    screen::present();
}

void explore::managers::game::destroy() {
    // asset::destroy();
    screen::destroy();
}
