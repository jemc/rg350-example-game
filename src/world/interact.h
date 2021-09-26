#ifndef WORLD_INTERACT_H
#define WORLD_INTERACT_H

#include "../world.h"

// TODO: Document
typedef struct DidInteract {
  int32_t frame;
} DidInteract;

// TODO: Document
typedef struct InteractDoor {
  int target_xi;
  int target_yi;
} InteractDoor;

// Forward-declare all components and entities for this module.
ECS_ENTITY_EXTERN_DECLARE(CanInteract);
ECS_COMPONENT_EXTERN_DECLARE(DidInteract);
ECS_COMPONENT_EXTERN_DECLARE(InteractDoor);

// Concretely declare all components and entities for this module.
#define WORLD_IMPLEMENT_INTERACT() \
  ECS_ENTITY_DECLARE(CanInteract); \
  ECS_COMPONENT_DECLARE(DidInteract); \
  ECS_COMPONENT_DECLARE(InteractDoor); \

// Setup all components and entities for this module in the given world.
#define WORLD_IMPORT_INTERACT(world) \
  ECS_ENTITY_DEFINE(world, CanInteract); \
  ECS_COMPONENT_DEFINE(world, DidInteract); \
  ECS_COMPONENT_DEFINE(world, InteractDoor); \

// Setup all systems for this module in the correct order of operations.
void world_setup_sys_interact(World* world);

// Set up all entities for this module.
void world_setup_ent_interact(World* world);

#endif
