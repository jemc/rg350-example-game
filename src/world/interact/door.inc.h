#include "../interact.h"

#include "../camera.h"
#include "../phys.h"
#include "../player.h"
#include "../room.h"

// When a door is interacted with, move the Player and Camera to the target.
WORLD_DEF_SYS(interact_door,
  PhysPosition(Player),
  PlayerDirection(Player),
  PhysPosition(Camera),
  (CanInteract, InteractDoor),
  DidInteract,
) {
  PhysPosition* pos = ecs_term(it, PhysPosition, 1);
  PlayerDirection* dir = ecs_term(it, PlayerDirection, 2);
  PhysPosition* cam = ecs_term(it, PhysPosition, 3);
  InteractDoor* door = ecs_term(it, InteractDoor, 4);

  for (int i = 0; i < it->count; i++) {
    // Move the Player and Camera to the target position.
    const float target_x = door[i].target_xi * ROOM_TILE_SIZE;
    const float target_y = door[i].target_yi * ROOM_TILE_SIZE;
    pos->x = target_x;
    pos->y = target_y;
    cam->x = target_x + ROOM_TILE_SIZE / 2 - CAMERA_PIXEL_WIDTH / 2;
    cam->y = target_y + ROOM_TILE_SIZE / 2 - CAMERA_PIXEL_HEIGHT / 2;

    // Ensure the player is facing "inward", in a neutral orientation.
    dir->leftward = false;
    dir->rightward = false;
    dir->awayward = false;
    dir->upward = false;
    dir->downward = false;

    // Clear the event now that we've finished processing it.
    ecs_remove(it->world, it->entities[i], DidInteract);
  }
}

// Set up all these systems in the correct order of operations.
void world_setup_sys_interact_door(World* world) {
  WORLD_SETUP_SYS(world, interact_door, EcsPostLoad);
}

// Set up all entities for this module.
void world_setup_ent_interact_door(World* world) {
  // Empty
}

