#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#include <string>

#include "./common.h"
#include "./managers/game_manager.h"

i32 main(int argc, char **argv) {
    auto log_level{spdlog::level::debug};
    for (int i = 0; i < argc; i++) {
        std::string key{argv[i]};

        if ((key == "--log-level" || key == "-L") && i + 1 < argc) {
            log_level = spdlog::level::from_str(argv[i + 1]);
            i++;
        }
    }

    spdlog::set_level(log_level);
    spdlog::info("log level: {}", spdlog::level::to_string_view(log_level));

    explore::manager::GameManager game_manager{};
    if (!game_manager.initialize()) {
        return EXIT_FAILURE;
    }
    game_manager.run();
    return EXIT_SUCCESS;
}
