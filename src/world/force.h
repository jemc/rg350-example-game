#ifndef WORLD_FORCE_H
#define WORLD_FORCE_H

#include "../world.h"

// Gravity causes a constant downward acceleration on the Y axis.
// The value here should be a positive number representing the magnitude.
typedef struct { float magnitude; } Gravity;

// Forward-declare all components and entities for this module.
ECS_COMPONENT_EXTERN_DECLARE(Gravity);

// Concretely declare all components and entities for this module.
#define WORLD_IMPLEMENT_FORCE() \
  ECS_COMPONENT_DECLARE(Gravity); \

// Setup all components and entities for this module in the given world.
#define WORLD_IMPORT_FORCE(world) \
  ECS_COMPONENT_DEFINE(world, Gravity); \

// Setup all systems for this module in the correct order of operations.
void world_setup_sys_force(World* world);

#endif
