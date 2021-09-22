#ifndef WORLD_ROOM_H
#define WORLD_ROOM_H

#include "../world.h"

// Define some characteristics of the room tiles.
#define ROOM_TILE_SIZE 16

// A RoomLayer defines the set of tiles to render at a particular layer
// of a particular room (with the room being the entity to which it belongs).
typedef struct {
  const int width;
  const int height;
  const uint8_t* tiles;
} RoomLayer;

// When an entity with RoomLayer also has RoomLayerIsSolid, then all nonzero
// tiles in that layer will count as static full-tile collisions for the player.
typedef struct {} RoomLayerIsSolid;

// Forward-declare all components and entities for this module.
ECS_COMPONENT_EXTERN_DECLARE(RoomLayer);
ECS_COMPONENT_EXTERN_DECLARE(RoomLayerIsSolid);

// Concretely declare all components and entities for this module.
#define WORLD_IMPLEMENT_ROOM() \
  ECS_COMPONENT_DECLARE(RoomLayer); \
  ECS_COMPONENT_DECLARE(RoomLayerIsSolid); \

// Setup all components and entities for this module in the given world.
#define WORLD_IMPORT_ROOM(world) \
  ECS_COMPONENT_DEFINE(world, RoomLayer); \
  ECS_COMPONENT_DEFINE(world, RoomLayerIsSolid); \

// Setup all systems for this module in the correct order of operations.
void world_setup_sys_room(World* world);

// Set up all entities for this module.
void world_setup_ent_room(World* world);

#endif
