#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "../common.h"

namespace explore::managers::game {
bool initialize();

void set_cap_frame_rate(bool state);

void setup();

void load_level(u32 level);

void run();

void process_input();

void update();

void render();

void destroy();
}  // namespace explore::managers::game

#endif  // GAME_MANAGER_H
