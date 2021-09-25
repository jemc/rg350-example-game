#include "camera.h"
WORLD_IMPLEMENT_CAMERA();

#include <flecs.h>

// Set up all these systems in the correct order of operations.
void world_setup_sys_camera(World* world) {
  // Empty
}

// Set up all entities for this module.
void world_setup_ent_camera(World* world) {
  ecs_singleton_set(world, Camera, {0, 0});
}
