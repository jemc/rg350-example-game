#ifndef WORLD_ROOM_H
#define WORLD_ROOM_H

#include "../world.h"

// Define some characteristics of the room tiles.
#define ROOM_TILE_SIZE 16

// Forward-declare all components and entities for this module.
  // Empty

// Concretely declare all components and entities for this module.
#define WORLD_IMPLEMENT_ROOM() \
  // Empty

// Setup all components and entities for this module in the given world.
#define WORLD_IMPORT_ROOM(world) \
  ECS_ENTITY(world, Block1); \
  ecs_set(world, Block1, EcsRgb, {0xBB, 0x88, 0xFF}); \
  ecs_set(world, Block1, EcsSquare, {ROOM_TILE_SIZE}); \
  ecs_set(world, Block1, EcsPosition2, \
    {(VIDEO_WIDTH - ROOM_TILE_SIZE) / 2, VIDEO_HEIGHT - ROOM_TILE_SIZE}); \
  ECS_ENTITY(world, Block3); \
  ecs_set(world, Block3, EcsRgb, {0xBB, 0x88, 0xFF}); \
  ecs_set(world, Block3, EcsSquare, {ROOM_TILE_SIZE}); \
  ecs_set(world, Block3, EcsPosition2, \
    {(VIDEO_WIDTH - ROOM_TILE_SIZE) / 2, VIDEO_HEIGHT - ROOM_TILE_SIZE * 3}); \

// Setup all systems for this module in the correct order of operations.
void world_setup_sys_room(World* world);

// Set up all entities for this module.
void world_setup_ent_room(World* world);

#endif
