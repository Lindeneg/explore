#ifndef EXPLORE_SYSTEMS_KEYBOARD_H_
#define EXPLORE_SYSTEMS_KEYBOARD_H_

#include "../ecs/ecs.h"

namespace explore::event {
class Bus;
class KeyPressed;
}  // namespace explore::event

namespace explore::system {
class Keyboard : public ecs::System {
   public:
    Keyboard();

    virtual void subscribe_to_events(event::Bus &event_bus) override;

    void on_key_pressed(event::KeyPressed &event);

    void update();
};
}  // namespace explore::system

#endif  // EXPLORE_SYSTEMS_KEYBOARD_H_
