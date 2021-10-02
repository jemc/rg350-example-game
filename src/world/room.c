#include "room.h"
WORLD_IMPLEMENT_ROOM();

#include <SDL2/SDL_image.h>

#include "image.h"
#include "interact.h"
#include "phys.h"
#include "player.h"
#include "render.h"

#include "room/room1.h"
#include "room/room2.h"

// For any new room layers (being in need of rendering from a tile set),
// render it once to an SDL texture for use in rendering later.
WORLD_DEF_SYS(room_layer_pre_render,
  $Video, ImageSource(self|super), RoomVisualLayer, RoomVisualLayerNeedsRender
) {
  Video *video = ecs_term(it, Video, 1);
  ImageSource* image_source = ecs_term(it, ImageSource, 2);
  RoomVisualLayer *layer = ecs_term(it, RoomVisualLayer, 3);

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

    for (int j = 0; layer[i].tiles[j] != NULL; j++) {
      int tile_index = 0;
      for (int y = 0; y < pixel_height; y += ROOM_TILE_SIZE) {
        for (int x = 0; x < pixel_width; x += ROOM_TILE_SIZE) {
          int tile_type = layer[i].tiles[j][tile_index++];
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
          rc = SDL_RenderCopy(video->renderer, image_source->texture,
            &src_rect, &dst_rect);
          ecs_assert(rc == 0, ECS_INTERNAL_ERROR, NULL);
        }
      }
    }

    // Remove the NeedsRoomTileSet component - the need has been fulfilled.
    ecs_remove(it->world, it->entities[i], RoomVisualLayerNeedsRender);
  }

  // Ensure that the render target is set back to rendering to the window.
  rc = SDL_SetRenderTarget(video->renderer, NULL);
  ecs_assert(rc == 0, ECS_INTERNAL_ERROR, NULL);
}

// Ensure that the SDL resources for a given RoomVisualLayer get freed when
// the component gets destroyed.
static void room_visual_layer_free(
  ecs_world_t *world,
  ecs_entity_t component,
  const ecs_entity_t *entity_ptr,
  void *ptr,
  size_t size,
  int32_t count,
  void *ctx
) {
  RoomVisualLayer* layer = (RoomVisualLayer*)ptr;

  if(layer->texture) SDL_DestroyTexture(layer->texture);
}

// Set up all these systems in the correct order of operations.
void world_setup_sys_room(World* world) {
  WORLD_SETUP_SYS(world, room_layer_pre_render, EcsOnLoad);

  // Set up lifecycle hook for freeing SDL resources for a room visual layer.
  ecs_set_component_actions(world, RoomVisualLayer, {
    .dtor = room_visual_layer_free,
  });
}

void world_setup_ent_room_1(World* world) {
  ECS_PREFAB(world, Room1TileSet, (ChildOf, Room));
  ecs_set(world, Room1TileSet, ImageSource, {room_room1_tileset_data});

  ECS_ENTITY(world, Room1VisualLayerParallax, (ChildOf, Room));
  ecs_add_pair(world, Room1VisualLayerParallax, EcsIsA, Room1TileSet);
  ecs_add(world, Room1VisualLayerParallax, RoomVisualLayerNeedsRender);
  ecs_set(world, Room1VisualLayerParallax, RoomVisualLayer, {
    .tiles = {
      room_room1_layer_parallax.tiles,
      NULL
    },
    .width = room_room1_layer_parallax.width,
    .height = room_room1_layer_parallax.height,
    .parallax_factor = 0.1
  });

  ECS_ENTITY(world, Room1VisualLayerMain, (ChildOf, Room));
  ecs_add_pair(world, Room1VisualLayerMain, EcsIsA, Room1TileSet);
  ecs_add(world, Room1VisualLayerMain, RoomVisualLayerNeedsRender);
  ecs_set(world, Room1VisualLayerMain, RoomVisualLayer, {
    .tiles = {
      room_room1_layer_backdrop.tiles,
      room_room1_layer_decor.tiles,
      room_room1_layer_solids.tiles,
      NULL
    },
    .width = room_room1_layer_solids.width,
    .height = room_room1_layer_solids.height,
  });

  ECS_ENTITY(world, Room1Solids, (ChildOf, Room));
  ecs_set_ptr(world, Room1Solids, RoomLayer, &room_room1_layer_solids);
  ecs_add(world, Room1Solids, RoomLayerIsSolid);

  ECS_ENTITY(world, Room1DoorTmp, (ChildOf, Room), (IsA, InteractDoor));
  ecs_set(world, Room1DoorTmp, PhysTilePosition, {36, 31});
  ecs_set(world, Room1DoorTmp, PhysTargetTilePosition, {100, 24});

  ECS_ENTITY(world, Room1DoorToTmp, (ChildOf, Room), (IsA, InteractDoor));
  ecs_set(world, Room1DoorToTmp, PhysTilePosition, {100, 24});
  ecs_set(world, Room1DoorToTmp, PhysTargetTilePosition, {4, 11});
  ecs_set(world, Room1DoorToTmp, TargetRoom, {2});

  ECS_ENTITY(world, Room1ChestTmp, (ChildOf, Room), (IsA, InteractChest));
  ecs_set(world, Room1ChestTmp, PhysTilePosition, {50, 43});
}

void world_teardown_ent_room_1(World* world) {
  ecs_delete(world, ecs_lookup_child(world, Room, "Room1ChestTmp"));
  ecs_delete(world, ecs_lookup_child(world, Room, "Room1DoorToTmp"));
  ecs_delete(world, ecs_lookup_child(world, Room, "Room1DoorTmp"));
  ecs_delete(world, ecs_lookup_child(world, Room, "Room1Solids"));
  ecs_delete(world, ecs_lookup_child(world, Room, "Room1VisualLayerMain"));
  ecs_delete(world, ecs_lookup_child(world, Room, "Room1VisualLayerParallax"));
  ecs_delete(world, ecs_lookup_child(world, Room, "Room1TileSet"));
}

void world_setup_ent_room_2(World* world) {
  ECS_PREFAB(world, Room2TileSet, (ChildOf, Room));
  ecs_set(world, Room2TileSet, ImageSource, {room_room2_tileset_data});

  ECS_ENTITY(world, Room2VisualLayerParallax, (ChildOf, Room));
  ecs_add_pair(world, Room2VisualLayerParallax, EcsIsA, Room2TileSet);
  ecs_add(world, Room2VisualLayerParallax, RoomVisualLayerNeedsRender);
  ecs_set(world, Room2VisualLayerParallax, RoomVisualLayer, {
    .tiles = {
      room_room2_layer_parallax.tiles,
      NULL
    },
    .width = room_room2_layer_parallax.width,
    .height = room_room2_layer_parallax.height,
    .parallax_factor = 0.1
  });

  ECS_ENTITY(world, Room2VisualLayerMain, (ChildOf, Room));
  ecs_add_pair(world, Room2VisualLayerMain, EcsIsA, Room2TileSet);
  ecs_add(world, Room2VisualLayerMain, RoomVisualLayerNeedsRender);
  ecs_set(world, Room2VisualLayerMain, RoomVisualLayer, {
    .tiles = {
      room_room2_layer_backdrop.tiles,
      room_room2_layer_decor.tiles,
      room_room2_layer_solids.tiles,
      NULL
    },
    .width = room_room2_layer_solids.width,
    .height = room_room2_layer_solids.height,
  });

  ECS_ENTITY(world, Room2Solids, (ChildOf, Room));
  ecs_set_ptr(world, Room2Solids, RoomLayer, &room_room2_layer_solids);
  ecs_add(world, Room2Solids, RoomLayerIsSolid);

  ECS_ENTITY(world, Room2DoorTmp, (ChildOf, Room), (IsA, InteractDoor));
  ecs_set(world, Room2DoorTmp, PhysTilePosition, {4, 11});
  ecs_set(world, Room2DoorTmp, PhysTargetTilePosition, {100, 24});
  ecs_set(world, Room2DoorTmp, TargetRoom, {1});

  ECS_ENTITY(world, Room2DoorToTmp, (ChildOf, Room), (IsA, InteractDoor));
  ecs_set(world, Room2DoorToTmp, PhysTilePosition, {20, 5});
  ecs_set(world, Room2DoorToTmp, PhysTargetTilePosition, {4, 11});

  ECS_ENTITY(world, Room2SavePointTmp, (ChildOf, Room), (IsA, InteractSavePoint));
  ecs_set(world, Room2SavePointTmp, PhysTilePosition, {12, 8});
}

void world_teardown_ent_room_2(World* world) {
  ecs_delete(world, ecs_lookup_child(world, Room, "Room2SavePointTmp"));
  ecs_delete(world, ecs_lookup_child(world, Room, "Room2DoorToTmp"));
  ecs_delete(world, ecs_lookup_child(world, Room, "Room2DoorTmp"));
  ecs_delete(world, ecs_lookup_child(world, Room, "Room2Solids"));
  ecs_delete(world, ecs_lookup_child(world, Room, "Room2VisualLayerMain"));
  ecs_delete(world, ecs_lookup_child(world, Room, "Room2VisualLayerParallax"));
  ecs_delete(world, ecs_lookup_child(world, Room, "Room2TileSet"));
}


// Set up all entities for this module.
void world_setup_ent_room(World* world) {
  // TODO: Load rooms dynamically as needed instead of hard-coding use of Room1.
  world_setup_ent_room_1(world);
}
