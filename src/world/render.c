#include "render.h"
WORLD_IMPLEMENT_RENDER();

#include "camera.h"
#include "phys.h"
#include "room.h"
#include "sprite.h"

// Render the background.
WORLD_DEF_SYS(render_background, $Video) {
  Video *video = ecs_term(it, Video, 1);

	const SDL_Rect background_rect = {
    .x = 0, .y = 0,
    .w = VIDEO_SCALE * VIDEO_WIDTH,
    .h = VIDEO_SCALE * VIDEO_HEIGHT
  };

	SDL_SetRenderDrawColor(video->renderer, 0x66, 0x66, 0x99, 0xFF);
	SDL_RenderFillRect(video->renderer, &background_rect);

	SDL_SetRenderDrawColor(video->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderDrawRect(video->renderer, &background_rect);
}

// Render the tiles of each room layer.
// TODO: Sort layers by Z order, so they'll stay in the right render order
// even when flecs tables shuffle them around in their memory.
WORLD_DEF_SYS(render_room_layer, $Video, $Camera, RoomTileSet, RoomLayer) {
  // Render the object as a square outline.
  const int size = ROOM_TILE_SIZE;
  Video *video = ecs_term(it, Video, 1);
  Camera *cam = ecs_term(it, Camera, 2);
  RoomTileSet *tile_set = ecs_term(it, RoomTileSet, 3);
  RoomLayer *room = ecs_term(it, RoomLayer, 4);

  // Determine the start and end of x index and y index iteration for
  // rendering only the tiles that are within view of the camera.
  int xi_init = cam->x / ROOM_TILE_SIZE;
  int yi_init = cam->y / ROOM_TILE_SIZE;
  const int xi_end = xi_init + CAMERA_PIXEL_WIDTH / ROOM_TILE_SIZE;
  const int yi_end = yi_init + CAMERA_PIXEL_HEIGHT / ROOM_TILE_SIZE;

  // Tile iterating can't start outside the upper-left bounds of the room.
  if (xi_init < 0) xi_init = 0;
  if (yi_init < 0) yi_init = 0;

  for (int i = 0; i < it->count; i ++) {
    // Tile iterating can't start outside the lower-right bounds of the room.
    const int room_xi_end = xi_end <= room[i].width ? xi_end : room[i].width;
    const int room_yi_end = yi_end <= room[i].height ? yi_end : room[i].height;

    // Render each tile in this room layer.
    for (int xi = xi_init; xi < room_xi_end; xi++) {
      for (int yi = yi_init; yi < room_yi_end; yi++) {
        // Get the tile type, or skip to the next iteration if no tile.
        const uint8_t tile_type = room[i].tiles[xi + (yi * room[i].width)];
        if (tile_type == (uint8_t)-1) continue;

        // Set the location to copy from in the tile set.
        const SDL_Rect src_rect = {
          .x = (tile_type % ROOM_TILE_SET_COLUMNS) * ROOM_TILE_SIZE,
          .y = (tile_type / ROOM_TILE_SET_COLUMNS) * ROOM_TILE_SIZE,
          .w = ROOM_TILE_SIZE,
          .h = ROOM_TILE_SIZE
        };

        // Set the location to copy to in the render canvas.
        const SDL_Rect dst_rect = {
          .x = VIDEO_SCALE * xi * ROOM_TILE_SIZE - (int)(VIDEO_SCALE * cam->x),
          .y = VIDEO_SCALE * yi * ROOM_TILE_SIZE - (int)(VIDEO_SCALE * cam->y),
          .w = VIDEO_SCALE * ROOM_TILE_SIZE,
          .h = VIDEO_SCALE * ROOM_TILE_SIZE
        };

        // Render the tile by copying from the source to the destination.
        SDL_RenderCopy(video->renderer, tile_set->texture, &src_rect, &dst_rect);
      }
    }
  }
}

// Render objects that have sprites defined.
WORLD_DEF_SYS(render_sprites,
  $Video, $Camera, PhysPosition, SpriteSheet, ?SpriteChoice
) {
  Video *video = ecs_term(it, Video, 1);
  Camera *cam = ecs_term(it, Camera, 2);
  PhysPosition *pos = ecs_term(it, PhysPosition, 3);
  SpriteSheet *sheet = ecs_term(it, SpriteSheet, 4);
  SpriteChoice *choice = ecs_term(it, SpriteChoice, 5);
  const bool has_choice = ecs_term_is_set(it, 5);

  for (int i = 0; i < it->count; i ++) {
    const SpriteSheetSpec* spec = sheet->spec;
    const float w = has_choice ? choice[i].rect->w : spec->each_width;
    const float h = has_choice ? choice[i].rect->h : spec->each_height;
    const SDL_Rect dst_rect = {
      .x = VIDEO_SCALE * ((int)pos[i].x - cam->x),
      .y = VIDEO_SCALE * ((int)pos[i].y - cam->y),
      .w = VIDEO_SCALE * w,
      .h = VIDEO_SCALE * h
    };

    if(has_choice) {
      SDL_RenderCopyEx(video->renderer,
        sheet->texture, choice[i].rect, &dst_rect, 0, NULL, choice[i].flip
      );
    } else {
      const SDL_Rect sprite_rect = { .x = 0, .y = 0, .w = w, .h = h };
      SDL_RenderCopy(video->renderer, sheet->texture, &sprite_rect, &dst_rect);
    }
  }
}

// Finish rendering the frame.
WORLD_DEF_SYS(render_finish, $Video) {
  Video *video = ecs_term(it, Video, 1);

	SDL_RenderPresent(video->renderer);

  SDL_Delay(8); // TODO: Remove?
}

// Set up all these systems in the correct order of operations.
void world_setup_sys_render(World* world) {
  WORLD_SETUP_SYS(world, render_background, EcsPostFrame);
  WORLD_SETUP_SYS(world, render_room_layer, EcsPostFrame);
  WORLD_SETUP_SYS(world, render_sprites, EcsPostFrame);
  WORLD_SETUP_SYS(world, render_finish, EcsPostFrame);
}

// Set up all entities for this module.
void world_setup_ent_render(World* world) {
  // Empty
}
