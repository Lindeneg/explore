#include <spdlog/spdlog.h>

#include "common.h"
#include "managers/game_manager.h"

i32 main() {
    spdlog::set_level(spdlog::level::trace);
    if (!explore::managers::game::initialize()) {
        return EXIT_FAILURE;
    }
    explore::managers::game::run();
    explore::managers::game::destroy();
    return EXIT_SUCCESS;
}
