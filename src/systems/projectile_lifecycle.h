#ifndef EXPLORE_SYSTEMS_PROJECTILE_LIFECYCLE_H_
#define EXPLORE_SYSTEMS_PROJECTILE_LIFECYCLE_H_

#include "../ecs/ecs.h"

namespace explore::system {
class ProjectileLifecycle : public ecs::System {
   public:
    ProjectileLifecycle();

    void update();
};
}  // namespace explore::system

#endif  // EXPLORE_SYSTEMS_PROJECTILE_LIFECYCLE_H_
