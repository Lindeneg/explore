#ifndef EXPLORE_SYSTEMS_COLLISION_H_
#define EXPLORE_SYSTEMS_COLLISION_H_

#include "../ecs/ecs.h"

class SDL_Rect;

namespace explore::system {
class Collision : public ecs::System {
   public:
    Collision();

    void update(f32) override;

   private:
    static bool aabb_intersect(const SDL_Rect &a, const SDL_Rect &b);
};
}  // namespace explore::system

#endif  // EXPLORE_SYSTEMS_COLLISION_H_
