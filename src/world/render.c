#include "render.h"
WORLD_IMPLEMENT_RENDER();

#include <flecs_components_transform.h>
#include <flecs_components_geometry.h>
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

// Render objects that have a colored square area defined.
WORLD_DEF_SYS(render_squares, $Video, EcsPosition2, EcsSquare, EcsRgb) {
  Video *video = ecs_term(it, Video, 1);
  EcsPosition2 *pos = ecs_term(it, EcsPosition2, 2);
  EcsSquare *square = ecs_term(it, EcsSquare, 3);
  EcsRgb *colors = ecs_term(it, EcsRgb, 4);

  for (int i = 0; i < it->count; i ++) {
    // Render the object as a square outline.
    const float size = square[i].size;
    const SDL_Rect player_rect =
      { .x = pos[i].x - size, .y = pos[i].y - size, .w = size, .h = size };

    const ecs_rgb_t color = colors[i].value;
    SDL_SetRenderDrawColor(video->renderer, color.r, color.g, color.b, 0xFF);
    SDL_RenderDrawRect(video->renderer, &player_rect);
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
  WORLD_SETUP_SYS(world, render_squares, EcsPostFrame);
  WORLD_SETUP_SYS(world, render_sprites, EcsPostFrame);
  WORLD_SETUP_SYS(world, render_finish, EcsPostFrame);
}

// Set up all entities for this module.
void world_setup_ent_render(World* world) {
  // Empty
}
