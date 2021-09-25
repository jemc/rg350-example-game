#include "camera.h"
WORLD_IMPLEMENT_CAMERA();

#include <flecs.h>
#include <flecs_components_transform.h>
#include <flecs_components_geometry.h>

// Use the position of the player to update the camera position.
WORLD_DEF_SYS(camera_move, $Camera, EcsPosition2, EcsSquare, IsPlayer) {
  Camera* cam = ecs_term(it, Camera, 1);
  EcsPosition2* p = ecs_term(it, EcsPosition2, 2);
  EcsSquare* s = ecs_term(it, EcsSquare, 3);

  for (int i = 0; i < it->count; i++) {
    const float size = s[i].size;
    cam[i].x = p[i].x + size / 2 - CAMERA_PIXEL_WIDTH / 2;
    cam[i].y = p[i].y + size / 2 - CAMERA_PIXEL_HEIGHT / 2;
  }
}

// Set up all these systems in the correct order of operations.
void world_setup_sys_camera(World* world) {
  WORLD_SETUP_SYS(world, camera_move, EcsPreStore);
}

// Set up all entities for this module.
void world_setup_ent_camera(World* world) {
  ecs_singleton_set(world, Camera, {0, 0});
}
