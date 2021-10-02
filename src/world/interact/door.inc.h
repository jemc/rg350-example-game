#include "../interact.h"

#include "../camera.h"
#include "../phys.h"
#include "../player.h"
#include "../room.h"

ECS_PREFAB_DECLARE(InteractDoor);

// When a door is interacted with, move the Player and Camera to the target.
WORLD_DEF_SYS(interact_door,
  PhysPosition(Player),
  PlayerDirection(Player),
  CameraTarget(Camera),
  PhysTargetTilePosition,
  (IsA, InteractDoor),
  DidInteract,
) {
  PhysPosition* pos = ecs_term(it, PhysPosition, 1);
  PlayerDirection* dir = ecs_term(it, PlayerDirection, 2);
  CameraTarget* cam = ecs_term(it, CameraTarget, 3);
  PhysTargetTilePosition* target = ecs_term(it, PhysTargetTilePosition, 4);

  for (int i = 0; i < it->count; i++) {
    // Move the Player to the target position.
    pos->x = target[i].xi * ROOM_TILE_SIZE;
    pos->y = target[i].yi * ROOM_TILE_SIZE;

    // Ensure the player is facing "inward", in a neutral orientation.
    dir->leftward = false;
    dir->rightward = false;
    dir->awayward = false;
    dir->upward = false;
    dir->downward = false;

    // Ensure that the camera will jump immediately in the next frame,
    // instead of smoothly scrolling to the player's new position.
    cam->next_immediate = true;

    // Clear the event now that we've finished processing it.
    ecs_remove(it->world, it->entities[i], DidInteract);
  }
}

// Set up all these systems in the correct order of operations.
void world_setup_sys_interact_door(World* world) {
  ECS_PREFAB_DEFINE(world, InteractDoor, CanInteract);

  WORLD_SETUP_SYS(world, interact_door, EcsPostLoad);
}

// Set up all entities for this module.
void world_setup_ent_interact_door(World* world) {
  // Empty
}

