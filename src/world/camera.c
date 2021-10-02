#include "camera.h"
WORLD_IMPLEMENT_CAMERA();

#include "phys.h"
#include "player.h"
#include "room.h"

// Use the position of the player to update the camera target and position.
WORLD_DEF_SYS(camera_move,
  CameraTarget(Camera), PhysPosition(Camera),
  PhysPosition(Player), PhysBounds(Player), PlayerDirection(Player),
  RoomVisualLayer
) {
  CameraTarget* target = ecs_term(it, CameraTarget, 1);
  PhysPosition* cam = ecs_term(it, PhysPosition, 2);
  PhysPosition* p = ecs_term(it, PhysPosition, 3);
  PhysBounds* b = ecs_term(it, PhysBounds, 4);
  PlayerDirection* dir = ecs_term(it, PlayerDirection, 5);
  RoomVisualLayer* layer = ecs_term(it, RoomVisualLayer, 6);

  int max_width = 0;
  int max_height = 0;
  for (int i = 0; i < it->count; i++) {
    const int layer_width = layer[i].width * ROOM_TILE_SIZE;
    const int layer_height = layer[i].height * ROOM_TILE_SIZE;
    if (layer_width > max_width) max_width = layer_width;
    if (layer_height > max_height) max_height = layer_height;
  }

  float offset_x = 0;
  float offset_y = 0;
  if (dir->rightward) {
    offset_x = CAMERA_OFFSET_HORIZONTAL;
  } else if (dir->leftward) {
    offset_x = -CAMERA_OFFSET_HORIZONTAL;
  }
  if (dir->downward) {
    offset_y = CAMERA_OFFSET_VERTICAL;
  } else if (dir->upward) {
    offset_y = -CAMERA_OFFSET_VERTICAL;
  }

  target->x = p->x + b->w / 2 - CAMERA_PIXEL_WIDTH / 2 + offset_x;
  target->y = p->y + b->h / 2 - CAMERA_PIXEL_HEIGHT / 2 + offset_y;

  // Keep camera within bounds of the visual layers of the room.
  if (target->x < 0) target->x = 0;
  if (target->y < 0) target->y = 0;
  if (target->x > max_width - CAMERA_PIXEL_WIDTH)
    target->x = max_width - CAMERA_PIXEL_WIDTH;
  if (target->y > max_height - CAMERA_PIXEL_HEIGHT)
    target->y = max_height - CAMERA_PIXEL_HEIGHT;

  if (target->next_immediate) {
    cam->x = target->x;
    cam->y = target->y;
    target->next_immediate = false;
  } else {
    cam->x = (target->x
      + (CAMERA_SLOWDOWN_HORIZONTAL - 1) * cam->x
    ) / CAMERA_SLOWDOWN_HORIZONTAL;
    cam->y = (target->y
      + (CAMERA_SLOWDOWN_VERTICAL - 1) * cam->y
    ) / CAMERA_SLOWDOWN_VERTICAL;
  }
}

// Set up all these systems in the correct order of operations.
void world_setup_sys_camera(World* world) {
  WORLD_SETUP_SYS(world, camera_move, EcsPreStore);
}

// Set up all entities for this module.
void world_setup_ent_camera(World* world) {
  ecs_set(world, Camera, PhysPosition, {});
  ecs_set(world, Camera, CameraTarget, { .next_immediate = true });
}
