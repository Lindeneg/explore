#ifndef EXPLORE_SYSTEMS_RENDER_H_
#define EXPLORE_SYSTEMS_RENDER_H_

#include "../ecs/ecs.h"

namespace explore::system {
class Render : public ecs::System {
   public:
    Render();

    void add_entity(ecs::Entity entity) override;

    void update(const core::GameContext &game_context);
};
}  // namespace explore::system

#endif  // EXPLORE_SYSTEMS_RENDER_H_
