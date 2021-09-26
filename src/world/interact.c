#include "interact.h"
WORLD_IMPLEMENT_INTERACT();

#include "camera.h"
#include "input.h"
#include "phys.h"
#include "player.h"
#include "room.h"

// When the Player presses the Up button, the first CanInteract tile entity that
// the Player is positioned over will be tagged with the DidInteract event tag.
WORLD_DEF_SYS(interact_trigger,
  PhysTilePosition,
  PhysPosition(Player),
  PlayerDirection(Player),
  InputButton(Player, InputButtonUp),
  (CanInteract, *),
) {
  PhysTilePosition* p = ecs_term(it, PhysTilePosition, 1);
  PhysPosition* player = ecs_term(it, PhysPosition, 2);
  PlayerDirection* player_dir = ecs_term(it, PlayerDirection, 3);
  InputButton* button = ecs_term(it, InputButton, 4);

  // Only test interaction logic if the button press is fresh in this frame.
  if (button->initial_frame != world_frame_number(it->world)) return;

  // Determine the tile position of the player.
  // TODO: Don't assume player size is the same as ROOM_TILE_SIZE.
  const int player_xi = floor(player->x / ROOM_TILE_SIZE + 0.5);
  const int player_yi = floor(player->y / ROOM_TILE_SIZE + 0.5);

  for (int i = 0; i < it->count; i++) {
    // Only consider an entity positioned at the same tile index as the player.
    if (p[i].xi != player_xi || p[i].yi != player_yi) continue;

    // Mark the interaction with the current frame number.
    const int32_t frame_number = world_frame_number(it->world);
    ecs_set(it->world, it->entities[i], DidInteract, {frame_number});

    // Remove the input button event from the player, so that it
    // won't be used to do anything in future frames.
    ecs_remove_id(it->world, Player, ecs_pair(ecs_id(InputButton), InputButtonUp));

    // Mark the player as facing away from the camera, interacting with it.
    player_dir->awayward = true;

    // Break so that we won't count interaction events with any other thing.
    // The player can only interact with one entity at a time.
    break;
  }
}

// When a door is interacted with, move the Player and Camera to the target.
WORLD_DEF_SYS(interact_door,
  $Camera,
  PhysPosition(Player),
  PlayerDirection(Player),
  (CanInteract, InteractDoor),
  DidInteract,
) {
  Camera* cam = ecs_term(it, Camera, 1);
  PhysPosition* p = ecs_term(it, PhysPosition, 2);
  PlayerDirection* dir = ecs_term(it, PlayerDirection, 3);
  InteractDoor* door = ecs_term(it, InteractDoor, 4);

  for (int i = 0; i < it->count; i++) {
    // Move the Player and Camera to the target position.
    const float target_x = door[i].target_xi * ROOM_TILE_SIZE;
    const float target_y = door[i].target_yi * ROOM_TILE_SIZE;
    p[i].x = target_x;
    p[i].y = target_y;
    cam[i].x = target_x + ROOM_TILE_SIZE / 2 - CAMERA_PIXEL_WIDTH / 2;
    cam[i].y = target_y + ROOM_TILE_SIZE / 2 - CAMERA_PIXEL_HEIGHT / 2;

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
void world_setup_sys_interact(World* world) {
  WORLD_SETUP_SYS(world, interact_trigger, EcsPostLoad);
  WORLD_SETUP_SYS(world, interact_door, EcsPostLoad);
}

// Set up all entities for this module.
void world_setup_ent_interact(World* world) {
  // Empty
}
