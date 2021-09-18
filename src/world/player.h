#ifndef WORLD_PLAYER_H
#define WORLD_PLAYER_H

#include "../world.h"

// Forward-declare all components and entities for this module.
ECS_ENTITY_EXTERN_DECLARE(Player);

// Concretely declare all components and entities for this module.
#define WORLD_IMPLEMENT_PLAYER() \
  ECS_ENTITY_DECLARE(Player); \

// Setup all components and entities for this module in the given world.
#define WORLD_IMPORT_PLAYER(world) \
  ECS_ENTITY_DEFINE(world, Player); \
  ecs_set(world, Player, EcsSquare, {16}); \
  ecs_set(world, Player, EcsRgb, {0xFF, 0xBB, 0x88}); \
  ecs_set(world, Player, EcsPosition2, {VIDEO_WIDTH / 2 + 8, VIDEO_HEIGHT / 2}); \
  ecs_set(world, Player, EcsVelocity2, {0, 0}); \
  ecs_set(world, Player, Gravity, {350}); \
  ecs_set(world, Player, FrictionHorizontal, {200}); \

// Setup all systems for this module in the correct order of operations.
void world_setup_sys_player(World* world);

#endif
