#include "room.h"
WORLD_IMPLEMENT_ROOM();

#include <SDL2/SDL_image.h>

#include "image.h"
#include "interact.h"
#include "phys.h"
#include "player.h"
#include "render.h"

#include "room/room1.inc.h"
#include "room/room2.inc.h"

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

    // Clear the full texture canvas to a neutral empty color.
    const SDL_Rect background_rect = { 0, 0, pixel_width, pixel_height };
    SDL_SetRenderDrawColor(video->renderer, 0, 0, 0, 0);
    SDL_RenderFillRect(video->renderer, &background_rect);

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

// Set up all entities for this module.
void world_setup_ent_room(World* world, RoomId room_id) {
  // Delete any entities currently nested within the Room.
  // TODO: Replace this snippet with `ecs_delete_children` when it is fixed.
  ecs_filter_t f;
  int rc = ecs_filter_init(world, &f, &(ecs_filter_desc_t) {
    .terms = {{ .id = ecs_pair(EcsChildOf, Room) }}
  });
  ecs_assert(rc == 0, ECS_INTERNAL_ERROR, NULL);
  ecs_iter_t it = ecs_filter_iter(world, &f);
  while (ecs_filter_next(&it)) {
    for (int i = 0; i < it.count; i ++) {
      ecs_delete(world, it.entities[i]);
    }
  }
  ecs_filter_fini(&f);

  // Load the specified room based on the room id.
  switch (room_id) {
    case RoomIdRoom1: world_setup_ent_room_1(world); break;
    case RoomIdRoom2: world_setup_ent_room_2(world); break;
    default: ecs_assert("Unknown RoomId" == 0, ECS_INVALID_PARAMETER, NULL);
  }
}
