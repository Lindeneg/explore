#ifndef EXPLORE_SYSTEMS_PROJECTILE_EMIT_H_
#define EXPLORE_SYSTEMS_PROJECTILE_EMIT_H_

#include "../ecs/ecs.h"

namespace explore::system {
class ProjectileEmit : public ecs::System {
   public:
    ProjectileEmit();

    void update(ecs::Registry &registry);
};
}  // namespace explore::system

#endif  // EXPLORE_SYSTEMS_PROJECTILE_EMIT_H_
