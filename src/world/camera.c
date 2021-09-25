#include "camera.h"
WORLD_IMPLEMENT_CAMERA();

#include "phys.h"
#include "player.h"

// Use the position of the player to update the camera position.
WORLD_DEF_SYS(camera_move, $Camera, PhysPosition, PhysBounds, PlayerDirection) {
  Camera* cam = ecs_term(it, Camera, 1);
  PhysPosition* p = ecs_term(it, PhysPosition, 2);
  PhysBounds* b = ecs_term(it, PhysBounds, 3);
  PlayerDirection* dir = ecs_term(it, PlayerDirection, 4);

  for (int i = 0; i < it->count; i++) {
    float offset_x = 0;
    float offset_y = 0;
    if (dir[i].rightward) {
      offset_x = CAMERA_OFFSET_HORIZONTAL;
    } else if (dir[i].leftward) {
      offset_x = -CAMERA_OFFSET_HORIZONTAL;
    }
    if (dir[i].downward) {
      offset_y = CAMERA_OFFSET_VERTICAL;
    } else if (dir[i].upward) {
      offset_y = -CAMERA_OFFSET_VERTICAL;
    }

    cam[i].target_x = p[i].x + b[i].w / 2 - CAMERA_PIXEL_WIDTH / 2 + offset_x;
    cam[i].target_y = p[i].y + b[i].h / 2 - CAMERA_PIXEL_HEIGHT / 2 + offset_y;

    // TODO: Is there a better way to mark this condition?
    // Do we need a CameraReset component?
    if (world_frame_number(it->world) == 0) {
      cam[i].x = cam[i].target_x;
      cam[i].y = cam[i].target_y;
    } else {
      cam[i].x = (cam[i].target_x
        + (CAMERA_SLOWDOWN_HORIZONTAL - 1) * cam[i].x
      ) / CAMERA_SLOWDOWN_HORIZONTAL;
      cam[i].y = (cam[i].target_y
        + (CAMERA_SLOWDOWN_VERTICAL - 1) * cam[i].y
      ) / CAMERA_SLOWDOWN_VERTICAL;
    }
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
