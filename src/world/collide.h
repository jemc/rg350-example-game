#ifndef WORLD_COLLIDE_H
#define WORLD_COLLIDE_H

#include "../world.h"

// Forward-declare all components and entities for this module.
  // Empty

// Concretely declare all components and entities for this module.
#define WORLD_IMPLEMENT_COLLIDE() \
  // Empty

// Setup all components and entities for this module in the given world.
#define WORLD_IMPORT_COLLIDE(world) \
  // Empty

// Setup all systems for this module in the correct order of operations.
void world_setup_sys_collide(World* world);

// Set up all entities for this module.
void world_setup_ent_collide(World* world);

#endif
