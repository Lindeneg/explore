#ifndef EXPLORE_SYSTEMS_RENDER_H_
#define EXPLORE_SYSTEMS_RENDER_H_

#include "../ecs/ecs.h"

namespace explore::manager {
class ScreenManager;
class ResourceManager;
}  // namespace explore::manager

namespace explore::system {
class Render : public ecs::System {
   public:
    Render();

    void add_entity(ecs::Entity entity) override;

    void update(const manager::ScreenManager &screen_manager,
                const manager::ResourceManager &resource_manager);
};
}  // namespace explore::system

#endif  // EXPLORE_SYSTEMS_RENDER_H_
