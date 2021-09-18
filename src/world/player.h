#ifndef WORLD_PLAYER_H
#define WORLD_PLAYER_H

#include "../world.h"

// Define some characteristics of the player character.
#define PLAYER_HEIGHT 16
#define PLAYER_JUMP_PEAK_HEIGHT (PLAYER_HEIGHT * 4.3)
#define PLAYER_JUMP_PEAK_TIME 0.5
#define PLAYER_JUMP_IMPULSE \
  (2 * PLAYER_JUMP_PEAK_HEIGHT / PLAYER_JUMP_PEAK_TIME)
#define PLAYER_GRAVITY \
  (2 * PLAYER_JUMP_PEAK_HEIGHT / PLAYER_JUMP_PEAK_TIME / PLAYER_JUMP_PEAK_TIME)

// Forward-declare all components and entities for this module.
ECS_ENTITY_EXTERN_DECLARE(Player);

// Concretely declare all components and entities for this module.
#define WORLD_IMPLEMENT_PLAYER() \
  ECS_ENTITY_DECLARE(Player); \

// Setup all components and entities for this module in the given world.
#define WORLD_IMPORT_PLAYER(world) \
  ECS_ENTITY_DEFINE(world, Player); \
  ecs_set(world, Player, EcsRgb, {0xFF, 0xBB, 0x88}); \
  ecs_set(world, Player, EcsSquare, {PLAYER_HEIGHT}); \
  ecs_set(world, Player, Gravity, {PLAYER_GRAVITY}); \
  ecs_set(world, Player, FrictionHorizontal, {200}); \
  ecs_set(world, Player, EcsVelocity2, {0, 0}); \
  ecs_set(world, Player, EcsPosition2, \
    {(VIDEO_WIDTH + PLAYER_HEIGHT) / 2, VIDEO_HEIGHT / 2}); \

// Setup all systems for this module in the correct order of operations.
void world_setup_sys_player(World* world);

#endif
