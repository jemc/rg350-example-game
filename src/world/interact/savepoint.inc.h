#include "../interact.h"

#include "../camera.h"
#include "../image.h"
#include "../phys.h"
#include "../sprite.h"

#include "../sprite/savepoint.h"

ECS_PREFAB_DECLARE(InteractSavePoint);

// When a save point is interacted with, (TODO).
WORLD_DEF_SYS(interact_savepoint,
  (IsA, InteractSavePoint),
  DidInteract,
) {
  // Empty
}

// Rotate the sprite 90 degrees every few frames.
// Because of how the sprite image is designed, this will give the appearance
// of spinning slowly (about 18 degrees per every 90 degrees actual rotation).
WORLD_DEF_SYS(interact_savepoint_choose_sprite,
  SpriteChoice(InteractSavePoint)
) {
  SpriteChoice *sprite = ecs_term(it, SpriteChoice, 1);

  sprite->angle = world_frame_number(it->world) / 8 % 4 * 90;
}

// Set up all these systems in the correct order of operations.
void world_setup_sys_interact_savepoint(World* world) {
  ECS_PREFAB_DEFINE(world, InteractSavePoint);

  WORLD_SETUP_SYS(world, interact_savepoint, EcsPostLoad);
  WORLD_SETUP_SYS(world, interact_savepoint_choose_sprite, EcsPreStore);
}

// Set up all entities for this module.
void world_setup_ent_interact_savepoint(World* world) {
  ecs_add(world, InteractSavePoint, CanInteract);
  ecs_set(world, InteractSavePoint, ImageSource, {sprite_savepoint.data});
  ecs_set_ptr(world, InteractSavePoint, SpriteSheet, &sprite_savepoint);
  ecs_set(world, InteractSavePoint, SpriteChoice, {&sprite_savepoint_default});
}

