#include "render.h"
WORLD_IMPLEMENT_RENDER();

#include <flecs_components_transform.h>
#include <flecs_components_geometry.h>
#include "room.h"
#include "sprite.h"

// Render the background.
WORLD_DEF_SYS(render_background, $Video) {
  Video *video = ecs_term(it, Video, 1);

	const SDL_Rect background_rect =
		{ .x = 0, .y = 0, .w = VIDEO_WIDTH, .h = VIDEO_HEIGHT };

	SDL_SetRenderDrawColor(video->renderer, 0x66, 0x66, 0x99, 0xFF);
	SDL_RenderFillRect(video->renderer, &background_rect);

	SDL_SetRenderDrawColor(video->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderDrawRect(video->renderer, &background_rect);
}

// Render the tiles of each room layer.
WORLD_DEF_SYS(render_room_layer, $Video, RoomLayer) {
  // Render the object as a square outline.
  const int size = ROOM_TILE_SIZE;
  Video *video = ecs_term(it, Video, 1);
  RoomLayer *room = ecs_term(it, RoomLayer, 2);

  SDL_SetRenderDrawColor(video->renderer, 0xBB, 0x88, 0xFF, 0xFF);

  for (int i = 0; i < it->count; i ++) {
    // printf("tile_type for x=y=0: %i\n", room[i].tiles[0]);
    for (int xi = 0; xi < room[i].width; xi++) {
      for (int yi = 0; yi < room[i].height; yi++) {
        const uint8_t tile_type = room[i].tiles[xi + (yi * room[i].width)];
        if (tile_type == (uint8_t)-1) continue;

        const SDL_Rect tile_rect = {
          .x = xi * ROOM_TILE_SIZE,
          .y = yi * ROOM_TILE_SIZE,
          .w = ROOM_TILE_SIZE,
          .h = ROOM_TILE_SIZE
        };

        SDL_RenderDrawRect(video->renderer, &tile_rect);
      }
    }
  }
}

// Render objects that have sprites defined.
WORLD_DEF_SYS(render_sprites,
  $Video, EcsPosition2, SpriteSheet, ?SpriteChoice
) {
  Video *video = ecs_term(it, Video, 1);
  EcsPosition2 *pos = ecs_term(it, EcsPosition2, 2);
  SpriteSheet *sheet = ecs_term(it, SpriteSheet, 3);
  SpriteChoice *choice = ecs_term(it, SpriteChoice, 4);
  const bool has_choice = ecs_term_is_set(it, 4);

  for (int i = 0; i < it->count; i ++) {
    const SpriteSheetSpec* spec = sheet->spec;
    const float w = has_choice ? choice[i].rect->w : spec->each_width;
    const float h = has_choice ? choice[i].rect->h : spec->each_height;
    const SDL_Rect dst_rect =
      { .x = pos[i].x - w, .y = pos[i].y - h, .w = w, .h = h };

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
