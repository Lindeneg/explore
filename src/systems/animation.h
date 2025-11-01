#ifndef EXPLORE_SYSTEMS_ANIMATION_H_
#define EXPLORE_SYSTEMS_ANIMATION_H_

#include "../ecs/ecs.h"

namespace explore::system {
class Animation : public ecs::System {
   public:
    Animation();

    void update(f32) override;
};
}  // namespace explore::system

#endif  // EXPLORE_SYSTEMS_ANIMATION_H_
