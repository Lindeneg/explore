cmake_minimum_required(VERSION 3.15)
project(ExploreApp LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

if (NOT DEFINED VCPKG_TARGET_TRIPLET)
    if (MSVC)
        set(VCPKG_TARGET_TRIPLET "x64-windows"
                CACHE STRING "vcpkg target triplet" FORCE)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(VCPKG_TARGET_TRIPLET "x64-mingw-dynamic"
                CACHE STRING "vcpkg target triplet" FORCE)
    else ()
        message(WARNING
                "Unrecognized compiler ${CMAKE_CXX_COMPILER_ID}; "
                "defaulting to x64-windows triplet.")
        set(VCPKG_TARGET_TRIPLET "x64-windows"
                CACHE STRING "vcpkg target triplet" FORCE)
    endif ()
    message(STATUS "vcpkg triplet set to: ${VCPKG_TARGET_TRIPLET}")
endif ()

if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)

    # reuse existing VCPKG_ROOT or default to build/vcpkg
    if (NOT DEFINED VCPKG_ROOT)
        if (DEFINED ENV{VCPKG_ROOT})
            set(VCPKG_ROOT "$ENV{VCPKG_ROOT}" CACHE PATH "Location of an existing vcpkg root")
        else ()
            set(VCPKG_ROOT "${CMAKE_BINARY_DIR}/vcpkg"
                    CACHE PATH "Location to clone vcpkg into if none found")
        endif ()
    endif ()

    if (EXISTS "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
        message(STATUS "Using existing vcpkg at: ${VCPKG_ROOT}")
    else ()
        include(FetchContent)
        FetchContent_Declare(
                vcpkg
                GIT_REPOSITORY https://github.com/microsoft/vcpkg.git
                GIT_TAG master
        )
        FetchContent_GetProperties(vcpkg)
        if (NOT vcpkg_POPULATED)
            FetchContent_Populate(vcpkg)
            message(STATUS "Bootstrapping vcpkg into: ${vcpkg_SOURCE_DIR}")

            if (WIN32)
                set(_BOOTSTRAP_CMD "${vcpkg_SOURCE_DIR}/bootstrap-vcpkg.bat")
            else ()
                set(_BOOTSTRAP_CMD "${vcpkg_SOURCE_DIR}/bootstrap-vcpkg.sh")
            endif ()

            execute_process(
                    COMMAND ${_BOOTSTRAP_CMD}
                    WORKING_DIRECTORY ${vcpkg_SOURCE_DIR}
                    RESULT_VARIABLE _vcpkg_bootstrap_err
            )
            if (_vcpkg_bootstrap_err)
                message(FATAL_ERROR "vcpkg bootstrap failed (err=${_vcpkg_bootstrap_err})")
            endif ()

            set(VCPKG_ROOT "${vcpkg_SOURCE_DIR}" CACHE PATH "Location of freshly‑bootstrapped vcpkg")
        endif ()
    endif ()

    set(CMAKE_TOOLCHAIN_FILE
            "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
            CACHE STRING "Vcpkg toolchain file")
    set(VCPKG_FEATURE_FLAGS "manifests" CACHE STRING "Enable vcpkg manifest mode")
endif ()

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
        src/core/context.cpp
        src/core/texture2d.cpp
        src/managers/screen_manager.cpp
        src/managers/game_manager.cpp
        src/managers/resource_manager.cpp
        src/ecs/ecs.cpp
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
