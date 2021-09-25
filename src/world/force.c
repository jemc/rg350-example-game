#include "force.h"
WORLD_IMPLEMENT_FORCE();

#include <flecs_components_transform.h>
#include <flecs_components_physics.h>

// Apply gravity as a force that increases downward velocity.
WORLD_DEF_SYS(force_apply_gravity, EcsVelocity2, Gravity) {
  EcsVelocity2 *v = ecs_term(it, EcsVelocity2, 1);
  Gravity *g = ecs_term(it, Gravity, 2);

  for (int i = 0; i < it->count; i ++) {
    v[i].y += g[i].magnitude;
    if (v[i].y > g[i].terminal_speed)
      v[i].y = g[i].terminal_speed;
  }
}

// Apply friction as a force that dampens horizontal velocity.
WORLD_DEF_SYS(force_apply_friction_horizontal, EcsVelocity2, FrictionHorizontal) {
  EcsVelocity2 *v = ecs_term(it, EcsVelocity2, 1);
  FrictionHorizontal *g = ecs_term(it, FrictionHorizontal, 2);

  float modifier = 0;
  for (int i = 0; i < it->count; i ++) {
    modifier = g[i].magnitude;
    if (v[i].x > modifier)
      v[i].x -= modifier;
    else if (v[i].x < -modifier)
      v[i].x += modifier;
    else
      v[i].x = 0;
  }
}

// Apply velocity to position each frame.
WORLD_DEF_SYS(force_apply_velocity, EcsPosition2, EcsVelocity2) {
  EcsPosition2 *p = ecs_term(it, EcsPosition2, 1);
  EcsVelocity2 *v = ecs_term(it, EcsVelocity2, 2);

  for (int i = 0; i < it->count; i++) {
    p[i].x += v[i].x;
    p[i].y += v[i].y;
  }
}

// Set up all these systems in the correct order of operations.
void world_setup_sys_force(World* world) {
  WORLD_SETUP_SYS(world, force_apply_gravity, EcsOnUpdate);
  WORLD_SETUP_SYS(world, force_apply_friction_horizontal, EcsOnUpdate);
  WORLD_SETUP_SYS(world, force_apply_velocity, EcsOnUpdate);
}

// Set up all entities for this module.
void world_setup_ent_force(World* world) {
  // Empty
}
