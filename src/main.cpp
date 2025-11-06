#include <spdlog/spdlog.h>

#include "common.h"
#include "managers/game_manager.h"

using namespace explore;

i32 main() {
    // TODO read config file
    spdlog::set_level(spdlog::level::trace);
    if (!manager::game::initialize()) {
        return EXIT_FAILURE;
    }
    manager::game::run();
    return EXIT_SUCCESS;
}
