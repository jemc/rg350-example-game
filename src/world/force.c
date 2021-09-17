#include "force.h"
WORLD_IMPLEMENT_FORCE()

#include <flecs_components_physics.h>

// Apply gravity by modifying the velocity based on the gravity for that object.
WORLD_DEF_SYS(apply_gravity, EcsVelocity2, Gravity) {
  EcsVelocity2 *v = ecs_term(it, EcsVelocity2, 1);
  Gravity *g = ecs_term(it, Gravity, 2);

  for (int i = 0; i < it->count; i ++) {
    v[i].y += g[i].magnitude * it->delta_time;
  }
}

// Set up all these systems in the correct order of operations.
void world_setup_sys_force(World* world) {
  WORLD_SETUP_SYS(world, apply_gravity, EcsOnUpdate);
}
