#include "collide.h"
WORLD_IMPLEMENT_COLLIDE()

#include <flecs_components_transform.h>
#include <flecs_components_physics.h>
#include <flecs_components_geometry.h>

// TODO: Flesh this out some more with real tiles.
WORLD_DEF_SYS(collide_actor_to_tile, EcsPosition2, EcsVelocity2) {
  EcsPosition2 *p = ecs_term(it, EcsPosition2, 1);
  EcsVelocity2 *v = ecs_term(it, EcsVelocity2, 2);

  for (int i = 0; i < it->count; i ++) {
    if (p[i].y > VIDEO_HEIGHT) {
      p[i].y = VIDEO_HEIGHT;
      v[i].y = 0; // clear vertical velocity back to zero
    }
  }
}

// Set up all these systems in the correct order of operations.
void world_setup_sys_collide(World* world) {
  WORLD_SETUP_SYS(world, collide_actor_to_tile, EcsOnUpdate);
}
