#include "keyboard.h"

#include <SDL_timer.h>
#include <spdlog/spdlog.h>

#include <string>

#include "../events/bus.h"
#include "../events/key_pressed.h"

namespace explore::system {

Keyboard::Keyboard() { _name = "KeyboardSystem"; }

void Keyboard::subscribe_to_events(event::Bus &event_bus) {
    event_bus.on<event::KeyPressed>(this, &Keyboard::on_key_pressed);
}

void Keyboard::on_key_pressed(event::KeyPressed &event) {
    std::string code{std::to_string(event.symbol)};
    std::string symbol(1, event.symbol);
    spdlog::debug("KEY PRESSED CODE:{} SYMBOL:{}", code, symbol);
}

void Keyboard::update() {}

};  // namespace explore::system
