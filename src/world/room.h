#ifndef WORLD_ROOM_H
#define WORLD_ROOM_H

#include "../world.h"

// Define some characteristics of the room tiles.
#define ROOM_TILE_SIZE 16
#define ROOM_TILE_SET_COLUMNS 8

// A RoomTileSet defines the image that the tiles for a room come from.
// It uses XPM-formatted source data, and SDL data loaded from that.
// TODO: Consolidate this with SpriteSheet component?
typedef struct {
  char** data;
  SDL_Surface* surface;
  SDL_Texture* texture;
} RoomTileSet;

// A relation component indicating which room tile set a given entity uses.
typedef struct {
  const int dummy; // TODO: remove this dummy value.
} NeedsRoomTileSet;

// A RoomLayer defines the set of tiles to render at a particular layer
// of a particular room (with the room being the entity to which it belongs).
typedef struct {
  const char* name;
  const uint8_t* tiles;
  int width;
  int height;
  SDL_Texture* texture;
} RoomLayer;

// A relation component indicating which room entity a given entity is inside.
typedef struct {
  const int dummy; // TODO: remove this dummy value.
} InRoom;

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
ECS_COMPONENT_EXTERN_DECLARE(RoomTileSet);
ECS_COMPONENT_EXTERN_DECLARE(NeedsRoomTileSet);
ECS_COMPONENT_EXTERN_DECLARE(RoomLayer);
ECS_TAG_EXTERN_DECLARE(RoomLayerIsSolid);
ECS_COMPONENT_EXTERN_DECLARE(InRoom);
ECS_ENTITY_EXTERN_DECLARE(Room1); // TODO: dynamic entity instead of static

// Concretely declare all components and entities for this module.
#define WORLD_IMPLEMENT_ROOM() \
  ECS_COMPONENT_DECLARE(RoomTileSet); \
  ECS_COMPONENT_DECLARE(NeedsRoomTileSet); \
  ECS_COMPONENT_DECLARE(RoomLayer); \
  ECS_TAG_DECLARE(RoomLayerIsSolid); \
  ECS_COMPONENT_DECLARE(InRoom); \
  ECS_ENTITY_DECLARE(Room1); \

// Setup all components and entities for this module in the given world.
#define WORLD_IMPORT_ROOM(world) \
  ECS_COMPONENT_DEFINE(world, RoomTileSet); \
  ECS_COMPONENT_DEFINE(world, NeedsRoomTileSet); \
  ECS_COMPONENT_DEFINE(world, RoomLayer); \
  ECS_TAG_DEFINE(world, RoomLayerIsSolid); \
  ECS_COMPONENT_DEFINE(world, InRoom); \
  ECS_ENTITY_DEFINE(world, Room1); \

// Setup all systems for this module in the correct order of operations.
void world_setup_sys_room(World* world);

// Set up all entities for this module.
void world_setup_ent_room(World* world);

#endif
