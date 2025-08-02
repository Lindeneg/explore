#include "game_manager.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <spdlog/spdlog.h>

#include "../common.h"
#include "../core/file.h"
#include "../core/texture2d.h"
#include "./screen_manager.h"

static bool is_running{false};
static bool do_cap_frame_rate{false};
static SDL_Event event{};
static u32 previous_frame_time{};
static explore::GameContext game_context{};

static void cap_frame_rate() {
    const i32 time_to_wait = static_cast<i32>(
        explore::constants::FRAME_TARGET -
        (static_cast<u32>(SDL_GetTicks()) - previous_frame_time));
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
    const f32 delta_time =
        (static_cast<f32>(SDL_GetTicks()) - previous_frame_time) / 1000.f;
    // clamp value (if running in debugger dt will be messed up)
    // game_context.delta_time =
    //    GREATER(delta_time, explore::constants::MAXIMUM_DT)
    //        ? explore::constants::MAXIMUM_DT
    //        : delta_time;
    // update previous frame time
    previous_frame_time = static_cast<f32>(SDL_GetTicks());
}

bool explore::managers::game::initialize() {
    if (!screen::initialize()) {
        return false;
    }
    is_running = true;
    return true;
}

void explore::managers::game::setup() {}

void explore::managers::game::load_level(const u32 level) {}

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
    ASSERT_RET_V(surface);

    SDL_Texture *texture{
        SDL_CreateTextureFromSurface(screen::get_renderer(), surface)};
    SDL_FreeSurface(surface);

    auto tex = core::Texture2D(
        "tank", FPATH("assets", "images", "tank-tiger-right.png"), texture, 32,
        32);

    screen::draw_texture(tex, SDL_Rect{400, 300});

    SDL_DestroyTexture(texture);

    screen::present();
}

void explore::managers::game::destroy() {
    // asset::destroy();
    screen::destroy();
}
