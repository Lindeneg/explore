#ifndef EXPLORE_EVENTS_KEY_PRESSED_H_
#define EXPLORE_EVENTS_KEY_PRESSED_H_

#include <SDL_keycode.h>

#include "./event.h"

namespace explore::event {

struct KeyPressed : public Event {
    SDL_Keycode symbol;

    KeyPressed(SDL_Keycode symbol) : symbol(symbol) {}
};

}  // namespace explore::event

#endif  // EXPLORE_EVENTS_KEY_PRESSED_H_
