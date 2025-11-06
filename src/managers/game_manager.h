#ifndef EXPLORE_MANAGERS_GAME_MANAGER_H
#define EXPLORE_MANAGERS_GAME_MANAGER_H

#include "../common.h"

namespace explore::manager::game {
bool initialize();

void set_cap_frame_rate(bool state);

void setup();

void load_level(u32 level);

void run();

void process_input();

void update();

void render();

void destroy();
}  // namespace explore::manager::game

#endif  // EXPLORE_MANAGERS_GAME_MANAGER_H
