#include "keyboard.h"

#include <SDL_keycode.h>
#include <SDL_timer.h>

#include <string>

#include "../ecs/components.h"
#include "../events/bus.h"
#include "../events/key_pressed.h"

namespace explore::system {

Keyboard::Keyboard() {
    _name = "KeyboardSystem";

    require_component<component::KeyboardControl>();
    require_component<component::Sprite>();
    require_component<component::RigidBody>();
}

void Keyboard::subscribe_to_events(event::Bus &event_bus) {
    event_bus.on<event::KeyPressed>(this, &Keyboard::on_key_pressed);
}

void Keyboard::on_key_pressed(event::KeyPressed &event) {
    std::string code{std::to_string(event.symbol)};
    std::string symbol(1, event.symbol);

    for (auto entity : get_entities()) {
        const auto &keyboard_control{
            entity.get_component<component::KeyboardControl>()};
        auto &sprite{entity.get_component<component::Sprite>()};
        auto &rigid_body{entity.get_component<component::RigidBody>()};

        switch (event.symbol) {
            case SDLK_UP:
                rigid_body.velocity = keyboard_control.up_velocity;
                sprite.src_rect.y = sprite.src_rect.h * 0;
                break;
            case SDLK_RIGHT:
                rigid_body.velocity = keyboard_control.right_velocity;
                sprite.src_rect.y = sprite.src_rect.h * 1;
                break;
            case SDLK_DOWN:
                rigid_body.velocity = keyboard_control.down_velocity;
                sprite.src_rect.y = sprite.src_rect.h * 2;
                break;
            case SDLK_LEFT:
                rigid_body.velocity = keyboard_control.left_velocity;
                sprite.src_rect.y = sprite.src_rect.h * 3;
                break;
            default:
                break;
        }
    }
}

void Keyboard::update() {}

};  // namespace explore::system
