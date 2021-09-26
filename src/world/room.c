#include "room.h"
WORLD_IMPLEMENT_ROOM();

#include <SDL2/SDL_image.h>

#include "interact.h"
#include "phys.h"
#include "render.h"

#include "room/room1.h"

// Ensure that the given RoomTileSet component is loaded when saving to ECS.
// Typically the entity definition code that sets the RoomTileSet component
// will just set the RoomTileSetSpec, and then it's the job of this function
// to load the data from the spec into an SDL surface and texture for use.
// TODO: Consolidate with similar code for SpriteSheet.
static void room_tile_set_load(
  ecs_world_t *world,
  ecs_entity_t component,
  const ecs_entity_t *dst_entity,
  const ecs_entity_t *src_entity,
  void *dst_ptr,
  const void *src_ptr,
  size_t size,
  int32_t count,
  void *ctx
) {
  RoomTileSet* tile_set = (RoomTileSet*)dst_ptr;

  // Copy the spec pointer from the source component.
  tile_set->data = ((RoomTileSet*)src_ptr)->data;

  // Load the raw data from the sprite tile_set spec into an SDL surface.
  tile_set->surface = IMG_ReadXPMFromArray(tile_set->data);
  if (!tile_set->surface) {
    printf("RoomTileSet failed to load.\n");
    return;
  }

  // Create an SDL texture that maps onto the sprite tile_set surface.
  tile_set->texture = SDL_CreateTextureFromSurface(
    ecs_singleton_get(world, Video)->renderer,
    tile_set->surface
  );
  if (!tile_set->texture) {
    printf("RoomTileSet failed to create texture.\n");
    return;
  }
}

// Ensure that the SDL resources for a given RoomTileSet get freed when
// TODO: Consolidate with similar code for SpriteSheet.
// the component gets destroyed.
static void room_tile_set_free(
  ecs_world_t *world,
  ecs_entity_t component,
  const ecs_entity_t *entity_ptr,
  void *ptr,
  size_t size,
  int32_t count,
  void *ctx
) {
  RoomTileSet* tile_set = (RoomTileSet*)ptr;

  if(tile_set->texture) SDL_DestroyTexture(tile_set->texture);
  if(tile_set->surface) SDL_FreeSurface(tile_set->surface);
}

// Set up all these systems in the correct order of operations.
void world_setup_sys_room(World* world) {
  // Set up lifecycle hooks for tile set loading/freeing in SDL.
  ecs_set_component_actions(world, RoomTileSet, {
    .copy = room_tile_set_load,
    .dtor = room_tile_set_free,
  });
}

// Set up all entities for this module.
void world_setup_ent_room(World* world) {
  // TODO: Load rooms dynamically as needed instead of hard-coding use of Room1.
  ECS_ENTITY(world, Room1Parallax);
  ecs_set(world, Room1Parallax, RoomTileSet, {room_room1_tileset_data});
  ecs_set_ptr(world, Room1Parallax, RoomLayer, &room_room1_layer_parallax);
  ecs_set_pair(world, Room1Parallax, InRoom, Room1, {});
  ECS_ENTITY(world, Room1Backdrop);
  ecs_set(world, Room1Backdrop, RoomTileSet, {room_room1_tileset_data});
  ecs_set_ptr(world, Room1Backdrop, RoomLayer, &room_room1_layer_backdrop);
  ecs_set_pair(world, Room1Backdrop, InRoom, Room1, {});
  ECS_ENTITY(world, Room1Decor);
  ecs_set(world, Room1Decor, RoomTileSet, {room_room1_tileset_data});
  ecs_set_ptr(world, Room1Decor, RoomLayer, &room_room1_layer_decor);
  ecs_set_pair(world, Room1Decor, InRoom, Room1, {});
  ECS_ENTITY(world, Room1Solids);
  ecs_set(world, Room1Solids, RoomTileSet, {room_room1_tileset_data});
  ecs_set_ptr(world, Room1Solids, RoomLayer, &room_room1_layer_solids);
  ecs_add(world, Room1Solids, RoomLayerIsSolid);
  ecs_set_pair(world, Room1Solids, InRoom, Room1, {});

  ECS_ENTITY(world, Room1DoorTmp);
  ecs_set(world, Room1DoorTmp, PhysTilePosition, {36, 31});
  ecs_set_pair_object(world, Room1DoorTmp, CanInteract, InteractDoor, {100, 24});

  ECS_ENTITY(world, Room1DoorToTmp);
  ecs_set(world, Room1DoorToTmp, PhysTilePosition, {100, 24});
  ecs_set_pair_object(world, Room1DoorToTmp, CanInteract, InteractDoor, {36, 31});
}
