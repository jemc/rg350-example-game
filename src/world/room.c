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
// the component gets destroyed.
// TODO: Consolidate with similar code for SpriteSheet.
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

// For any new room layers (being in need of rendering from a tile set),
// render it once to an SDL texture for use in rendering later.
WORLD_DEF_SYS(room_layer_pre_render,
  $Video, RoomLayer, (NeedsRoomTileSet, *),
) {
  Video *video = ecs_term(it, Video, 1);
  RoomLayer *layer = ecs_term(it, RoomLayer, 2);
  ecs_entity_t needs_tile_set_pair = ecs_term_id(it, 3);

  // Grab the tile set we need to load from.
  const RoomTileSet* tile_set = ecs_get(it->world,
    ecs_pair_object(it->world, needs_tile_set_pair),
    RoomTileSet
  );

  // If the tile set texture isn't loaded yet for some reason, there's
  // nothing we can do here. Assume we printed an error in room_tile_set_load.
  if (tile_set->texture == NULL) return;

  int rc;
  for (int i = 0; i < it->count; i++) {
    // Create the render target texture and target it.
    const int pixel_width = layer[i].width * ROOM_TILE_SIZE;
    const int pixel_height = layer[i].height * ROOM_TILE_SIZE;
    SDL_Texture* texture = SDL_CreateTexture(video->renderer,
      SDL_PIXELFORMAT_ARGB8888,
      SDL_TEXTUREACCESS_TARGET,
      pixel_width, pixel_height
    );
    ecs_assert(texture != NULL, ECS_INTERNAL_ERROR, NULL);
    layer[i].texture = texture;

    rc = SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    ecs_assert(rc == 0, ECS_INTERNAL_ERROR, NULL);

    rc = SDL_SetRenderTarget(video->renderer, texture);
    ecs_assert(rc == 0, ECS_INTERNAL_ERROR, NULL);

    int tile_index = 0;
    for (int y = 0; y < pixel_height; y += ROOM_TILE_SIZE) {
      for (int x = 0; x < pixel_width; x += ROOM_TILE_SIZE) {
        int tile_type = layer[i].tiles[tile_index++];
        if (tile_type == (uint8_t)-1) continue;

        // Set the location to copy from in the tile set.
        const SDL_Rect src_rect = {
          .x = (tile_type % ROOM_TILE_SET_COLUMNS) * ROOM_TILE_SIZE,
          .y = (tile_type / ROOM_TILE_SET_COLUMNS) * ROOM_TILE_SIZE,
          .w = ROOM_TILE_SIZE,
          .h = ROOM_TILE_SIZE
        };

        // Set the location to copy to in the render canvas.
        const SDL_Rect dst_rect = { x, y, ROOM_TILE_SIZE, ROOM_TILE_SIZE };

        // Render the tile by copying from the source to the destination.
        rc = SDL_RenderCopy(video->renderer, tile_set->texture, &src_rect, &dst_rect);
        ecs_assert(rc == 0, ECS_INTERNAL_ERROR, NULL);
      }
    }

    // Remove the NeedsRoomTileSet component - the need has been fulfilled.
    ecs_remove_id(it->world, it->entities[i], needs_tile_set_pair);
  }

  // Ensure that the render target is set back to rendering to the window.
  rc = SDL_SetRenderTarget(video->renderer, NULL);
  ecs_assert(rc == 0, ECS_INTERNAL_ERROR, NULL);
}

// Ensure that the SDL resources for a given RoomLayer get freed when
// the component gets destroyed.
static void room_layer_free(
  ecs_world_t *world,
  ecs_entity_t component,
  const ecs_entity_t *entity_ptr,
  void *ptr,
  size_t size,
  int32_t count,
  void *ctx
) {
  RoomLayer* layer = (RoomLayer*)ptr;

  if(layer->texture) SDL_DestroyTexture(layer->texture);
}

// Set up all these systems in the correct order of operations.
void world_setup_sys_room(World* world) {
  // Set up lifecycle hooks for tile set loading/freeing in SDL.
  ecs_set_component_actions(world, RoomTileSet, {
    .copy = room_tile_set_load,
    .dtor = room_tile_set_free,
  });

  // Set up lifecycle hook for freeing SDL resources for a room layer.
  ecs_set_component_actions(world, RoomLayer, {
    .dtor = room_layer_free,
  });

  WORLD_SETUP_SYS(world, room_layer_pre_render, EcsOnLoad);
}

// Set up all entities for this module.
void world_setup_ent_room(World* world) {
  // TODO: Load rooms dynamically as needed instead of hard-coding use of Room1.
  ECS_ENTITY(world, RoomTileSetRoom1);
  ecs_set(world, RoomTileSetRoom1, RoomTileSet, {room_room1_tileset_data});

  ECS_ENTITY(world, Room1Parallax);
  ecs_set_pair(world, Room1Parallax, NeedsRoomTileSet, RoomTileSetRoom1, {});
  ecs_set_ptr(world, Room1Parallax, RoomLayer, &room_room1_layer_parallax);
  ecs_set_pair(world, Room1Parallax, InRoom, Room1, {});

  ECS_ENTITY(world, Room1Backdrop);
  ecs_set_pair(world, Room1Backdrop, NeedsRoomTileSet, RoomTileSetRoom1, {});
  ecs_set_ptr(world, Room1Backdrop, RoomLayer, &room_room1_layer_backdrop);
  ecs_set_pair(world, Room1Backdrop, InRoom, Room1, {});

  ECS_ENTITY(world, Room1Decor);
  ecs_set_pair(world, Room1Decor, NeedsRoomTileSet, RoomTileSetRoom1, {});
  ecs_set_ptr(world, Room1Decor, RoomLayer, &room_room1_layer_decor);
  ecs_set_pair(world, Room1Decor, InRoom, Room1, {});

  ECS_ENTITY(world, Room1Solids);
  ecs_set_pair(world, Room1Solids, NeedsRoomTileSet, RoomTileSetRoom1, {});
  ecs_set_ptr(world, Room1Solids, RoomLayer, &room_room1_layer_solids);
  ecs_set_pair(world, Room1Solids, InRoom, Room1, {});
  ecs_add(world, Room1Solids, RoomLayerIsSolid);

  ECS_ENTITY(world, Room1DoorTmp);
  ecs_set(world, Room1DoorTmp, PhysTilePosition, {36, 31});
  ecs_set_pair_object(world, Room1DoorTmp, CanInteract, InteractDoor, {100, 24});

  ECS_ENTITY(world, Room1DoorToTmp);
  ecs_set(world, Room1DoorToTmp, PhysTilePosition, {100, 24});
  ecs_set_pair_object(world, Room1DoorToTmp, CanInteract, InteractDoor, {36, 31});
}
