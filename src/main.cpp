#include <spdlog/spdlog.h>

#include "common.h"
#include "managers/game_manager.h"

using namespace explore;

i32 main() {
    spdlog::set_level(spdlog::level::trace);
    if (!managers::game::initialize()) {
        return EXIT_FAILURE;
    }
    managers::game::run();
    managers::game::destroy();
    return EXIT_SUCCESS;
}
