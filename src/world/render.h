#ifndef WORLD_RENDER_H
#define WORLD_RENDER_H

#include "../world.h"

// Forward-declare all components and entities for this module.
extern ECS_COMPONENT_DECLARE(Video);

// Concretely declare all components and entities for this module.
#define WORLD_IMPLEMENT_RENDER() \
  ECS_COMPONENT_DECLARE(Video); \

// Setup all components and entities for this module in the given world.
#define WORLD_IMPORT_RENDER(world) \
  ECS_COMPONENT_DEFINE(world, Video); \

// Setup all systems for this module in the correct order of operations.
void world_setup_sys_render(World* world);

#endif
