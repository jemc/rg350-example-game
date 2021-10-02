#include "interact.h"
WORLD_IMPLEMENT_INTERACT();

#include "camera.h"
#include "input.h"
#include "phys.h"
#include "player.h"
#include "room.h"

#include "interact/door.inc.h"
#include "interact/savepoint.inc.h"

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

// Set up all these systems in the correct order of operations.
void world_setup_sys_interact(World* world) {
  WORLD_SETUP_SYS(world, interact_trigger, EcsPostLoad);
  world_setup_sys_interact_door(world);
  world_setup_sys_interact_savepoint(world);
}

// Set up all entities for this module.
void world_setup_ent_interact(World* world) {
  world_setup_ent_interact_door(world);
  world_setup_ent_interact_savepoint(world);
}
