#include <spdlog/spdlog.h>

#include "./common.h"
#include "./managers/game_manager.h"

using namespace explore;

i32 main() {
    // TODO read config file or use argument vector
    spdlog::set_level(spdlog::level::trace);
    manager::GameManager game_manager{};
    if (!game_manager.initialize()) {
        return EXIT_FAILURE;
    }
    game_manager.run();
    return EXIT_SUCCESS;
}
