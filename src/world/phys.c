#include "phys.h"
WORLD_IMPLEMENT_PHYS();

// Apply gravity as a force that increases downward velocity.
WORLD_DEF_SYS(phys_apply_gravity, PhysVelocity, Gravity) {
  PhysVelocity *v = ecs_term(it, PhysVelocity, 1);
  Gravity *g = ecs_term(it, Gravity, 2);

  for (int i = 0; i < it->count; i ++) {
    v[i].y += g[i].magnitude;
    if (v[i].y > g[i].terminal_speed)
      v[i].y = g[i].terminal_speed;
  }
}

// Apply friction as a force that dampens horizontal velocity.
WORLD_DEF_SYS(phys_apply_friction_horizontal, PhysVelocity, FrictionHorizontal) {
  PhysVelocity *v = ecs_term(it, PhysVelocity, 1);
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
WORLD_DEF_SYS(phys_apply_velocity, PhysPosition, PhysVelocity) {
  PhysPosition *p = ecs_term(it, PhysPosition, 1);
  PhysVelocity *v = ecs_term(it, PhysVelocity, 2);

  for (int i = 0; i < it->count; i++) {
    p[i].x += v[i].x;
    p[i].y += v[i].y;
  }
}

// Set up all these systems in the correct order of operations.
void world_setup_sys_phys(World* world) {
  WORLD_SETUP_SYS(world, phys_apply_gravity, EcsOnUpdate);
  WORLD_SETUP_SYS(world, phys_apply_friction_horizontal, EcsOnUpdate);
  WORLD_SETUP_SYS(world, phys_apply_velocity, EcsOnUpdate);
}

// Set up all entities for this module.
void world_setup_ent_phys(World* world) {
  // Empty
}
