#include "room.h"
WORLD_IMPLEMENT_ROOM();

#include "room/room1.h"

// Set up all these systems in the correct order of operations.
void world_setup_sys_room(World* world) {
  // Empty
}

// Set up all entities for this module.
void world_setup_ent_room(World* world) {
  // TODO: Load rooms dynamically as needed instead of hard-coding use of Room1.
  ECS_ENTITY(world, Room1);
  ecs_set_ptr(world, Room1, RoomLayer, &room_room1_layer_solids);
}
