#include "collide.h"
WORLD_IMPLEMENT_COLLIDE()

#include <flecs_components_transform.h>
#include <flecs_components_physics.h>
#include <flecs_components_geometry.h>

// TODO: Flesh this out some more with real tiles.
WORLD_DEF_SYS(collide_actor_to_tile, EcsPosition2, EcsVelocity2, EcsSquare) {
  EcsPosition2* p = ecs_term(it, EcsPosition2, 1);
  EcsVelocity2* v = ecs_term(it, EcsVelocity2, 2);
  EcsSquare* square = ecs_term(it, EcsSquare, 3);

  for (int i = 0; i < it->count; i ++) {
    float size = square[i].size;
    if (p[i].y > VIDEO_HEIGHT) {
      p[i].y = VIDEO_HEIGHT;
      v[i].y = 0;
    } else if (p[i].y < size) {
      p[i].y = size;
      v[i].y = 0;
    }
    if (p[i].x > VIDEO_WIDTH) {
      p[i].x = VIDEO_WIDTH;
      v[i].x = 0;
    }
    if (p[i].x < size) {
      p[i].x = size;
      v[i].x = 0;
    }
  }
}

// Set up all these systems in the correct order of operations.
void world_setup_sys_collide(World* world) {
  WORLD_SETUP_SYS(world, collide_actor_to_tile, EcsOnUpdate);
}
