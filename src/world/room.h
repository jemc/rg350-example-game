#ifndef WORLD_ROOM_H
#define WORLD_ROOM_H

#include "../world.h"

// Define some characteristics of the room tiles.
#define ROOM_TILE_SIZE 16

// A RoomLayer defines the set of tiles to render at a particular layer
// of a particular room (with the room being the entity to which it belongs).
typedef struct {
  const char* name;
  const uint8_t* tiles;
  const int width;
  const int height;
} RoomLayer;

// When an entity with RoomLayer also has RoomLayerIsSolid, then all nonzero
// tiles in that layer will count as static full-tile collisions for the player.
typedef struct {} RoomLayerIsSolid;

// Convenience macro for declaring RoomLayer values in generated room headers.
#define DEF_ROOM_LAYER(room_id, id, w, h, ...) \
  static const uint8_t room_##room_id##_layer_##id##_tiles[] = __VA_ARGS__; \
  static const RoomLayer room_##room_id##_layer_##id = { \
    .name = #id, \
    .tiles = room_##room_id##_layer_##id##_tiles, \
    .width = w, \
    .height = h \
  }; \

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
