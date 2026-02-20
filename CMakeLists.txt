cmake_minimum_required(VERSION 3.15)
project(ExploreApp LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

message("-- Toolchain file: " ${CMAKE_TOOLCHAIN_FILE})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(Lua REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(sol2 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_executable(ExploreApp
        src/main.cpp

        src/core/file.cpp
        src/core/game_context.cpp
        src/core/texture2d.cpp
        src/core/tilemap.cpp
        src/core/rect.cpp

        src/managers/screen_manager.cpp
        src/managers/game_manager.cpp
        src/managers/resource_manager.cpp

        src/ecs/ecs.cpp

        src/systems/movement.cpp
        src/systems/render.cpp
        src/systems/animation.cpp
        src/systems/collision.cpp
        src/systems/debug_render.cpp
        src/systems/damage.cpp
        src/systems/keyboard.cpp
        src/systems/camera_movement.cpp
        src/systems/projectile_emit.cpp
        src/systems/projectile_lifecycle.cpp
)

add_custom_command(TARGET ExploreApp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:ExploreApp>/assets
        COMMENT "Copying assets to output folder"
)

add_custom_command(TARGET ExploreApp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json"
        "${CMAKE_SOURCE_DIR}/")

target_include_directories(ExploreApp PRIVATE ${LUA_INCLUDE_DIR})
target_include_directories(ExploreApp PRIVATE ${SOL2_INCLUDE_DIRS})

target_link_libraries(ExploreApp PRIVATE
        SDL2::SDL2
        SDL2::SDL2main

        $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,
        SDL2_image::SDL2_image,
        SDL2_image::SDL2_image-static>

        $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,
        SDL2_ttf::SDL2_ttf,
        SDL2_ttf::SDL2_ttf-static>

        $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,
        SDL2_mixer::SDL2_mixer,
        SDL2_mixer::SDL2_mixer-static>

        ${LUA_LIBRARIES}
        glm::glm
        imgui::imgui
        spdlog::spdlog
)
