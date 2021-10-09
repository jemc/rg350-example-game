#include "../interact.h"

#include "../camera.h"
#include "../image.h"
#include "../phys.h"
#include "../sprite.h"

#include "../sprite/proj_sun.h"

ECS_PREFAB_DECLARE(InteractPowerBall);

// When a save point is interacted with, (TODO).
WORLD_DEF_SYS(interact_power_ball,
  (IsA, InteractPowerBall),
  DidInteract,
) {
  // Empty
}

// Rotate the sprite 90 degrees every few frames.
// Because of how the sprite image is designed, this will give the appearance
// of spinning slowly (about 18 degrees per every 90 degrees actual rotation).
WORLD_DEF_SYS(interact_power_ball_choose_sprite,
  SpriteChoice(InteractPowerBall)
) {
  SpriteChoice *sprite = ecs_term(it, SpriteChoice, 1);

  sprite->angle = world_frame_number(it->world) / 8 % 4 * 90;
  sprite->rect = (world_frame_number(it->world) / 4 % 2 == 0)
    ? &sprite_proj_sun_default
    : &sprite_proj_sun_rot45;
}

// Set up all these systems in the correct order of operations.
void world_setup_sys_interact_power_ball(World* world) {
  ECS_PREFAB_DEFINE(world, InteractPowerBall);

  WORLD_SETUP_SYS(world, interact_power_ball, EcsPostLoad);
  WORLD_SETUP_SYS(world, interact_power_ball_choose_sprite, EcsPreStore);
}

// Set up all entities for this module.
void world_setup_ent_interact_power_ball(World* world) {
  ecs_add(world, InteractPowerBall, CanInteract);
  ecs_set(world, InteractPowerBall, ImageSource, {sprite_proj_sun.data});
  ecs_set_ptr(world, InteractPowerBall, SpriteSheet, &sprite_proj_sun);
  ecs_set(world, InteractPowerBall, SpriteChoice, {&sprite_proj_sun_default});
}

