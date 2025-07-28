#include <spdlog/spdlog.h>

#include "managers/game_manager.h"

int main(int, char**) {
    spdlog::set_level(spdlog::level::trace);
    if (!explore::managers::game::initialize()) {
        return EXIT_FAILURE;
    }
    explore::managers::game::run();
    explore::managers::game::destroy();
    return EXIT_SUCCESS;
}
