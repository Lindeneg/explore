#include <iostream>
#include <SDL2/SDL.h>
#include <glm/vec2.hpp>

int main(int argv, char **args) {
    const std::string lang = "C++";
    std::cout << "Hello and welcome to " << lang << "!\n";

    auto vec{glm::vec2(0, 0)};

    auto d{SDL_acos(0.1l)};

    std::cout << "SDL RESULT " << d << std::endl;

    for (int i = 1; i <= 5; i++) {
        std::cout << "i = " << i << std::endl;
    }

    return 0;
}
