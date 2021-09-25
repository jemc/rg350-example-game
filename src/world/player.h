#ifndef WORLD_PLAYER_H
#define WORLD_PLAYER_H

#include "../world.h"

// Define some characteristics of the player character.
#define PLAYER_HEIGHT 16
#define PLAYER_JUMP_IMPULSE 2.1
#define PLAYER_JUMP_DEFY_GRAVITY_FRAMES 15
#define PLAYER_GRAVITY 0.12
#define PLAYER_GRAVITY_TERMINAL_SPEED 7 // keeping this below 8 helps keep collision checks simple
#define PLAYER_ACCEL_HORIZONTAL 0.06
#define PLAYER_MAX_SPEED_HORIZONTAL 2.3
#define PLAYER_FRICTION_HORIZONTAL 0.12

// Forward-declare all components and entities for this module.
ECS_ENTITY_EXTERN_DECLARE(Player);
ECS_TAG_EXTERN_DECLARE(IsPlayer);

// Concretely declare all components and entities for this module.
#define WORLD_IMPLEMENT_PLAYER() \
  ECS_ENTITY_DECLARE(Player); \
  ECS_TAG_DECLARE(IsPlayer); \

// Setup all components and entities for this module in the given world.
#define WORLD_IMPORT_PLAYER(world) \
  ECS_ENTITY_DEFINE(world, Player); \
  ECS_TAG_DEFINE(world, IsPlayer); \

// Setup all systems for this module in the correct order of operations.
void world_setup_sys_player(World* world);

// Set up all entities for this module.
void world_setup_ent_player(World* world);

#endif
