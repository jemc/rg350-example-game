#include "../interact.h"

#include "../camera.h"
#include "../image.h"
#include "../phys.h"
#include "../sprite.h"

#include "../sprite/chest.h"

ECS_PREFAB_DECLARE(InteractChest);

// When a save point is interacted with, (TODO).
WORLD_DEF_SYS(interact_chest,
  SpriteChoice(self|super),
  (IsA, InteractChest),
  DidInteract,
) {
  SpriteChoice *sprite = ecs_term(it, SpriteChoice, 1);

  for (int i = 0; i < it->count; i++) {
    sprite[i].rect = &sprite_chest_opened;
  }
}

// Set up all these systems in the correct order of operations.
void world_setup_sys_interact_chest(World* world) {
  ECS_PREFAB_DEFINE(world, InteractChest);

  WORLD_SETUP_SYS(world, interact_chest, EcsPostLoad);
}

// Set up all entities for this module.
void world_setup_ent_interact_chest(World* world) {
  ecs_add(world, InteractChest, CanInteract);
  ecs_set(world, InteractChest, ImageSource, {sprite_chest.data});
  ecs_set_ptr(world, InteractChest, SpriteSheet, &sprite_chest);
  ecs_set(world, InteractChest, SpriteChoice, {&sprite_chest_default});
}

