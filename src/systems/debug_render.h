#ifndef EXPLORE_SYSTEMS_DEBUG_RENDER_H_
#define EXPLORE_SYSTEMS_DEBUG_RENDER_H_

#include "../ecs/ecs.h"

namespace explore::system {
class DebugRender : public ecs::System {
   public:
    DebugRender();

    void update();
};
}  // namespace explore::system

#endif  // EXPLORE_SYSTEMS_DEBUG_RENDER_H_
