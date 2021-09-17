#include "render.h"
WORLD_IMPLEMENT_RENDER()

#include <flecs_components_transform.h>
#include <flecs_components_geometry.h>

// Render the background.
WORLD_DEF_SYS(render_background, $Video) {
  Video *video = ecs_term(it, Video, 1);

	const SDL_Rect background_rect =
		{ .x = 0, .y = 0, .w = VIDEO_WIDTH, .h = VIDEO_HEIGHT };

	SDL_SetRenderDrawColor(video->renderer, 0, 0, 0, 0xFF);
	SDL_RenderFillRect(video->renderer, &background_rect);

	SDL_SetRenderDrawColor(video->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderDrawRect(video->renderer, &background_rect);
}

// Render the visible square objects.
WORLD_DEF_SYS(render_squares, $Video, EcsPosition2, EcsSquare, EcsRgb) {
  Video *video = ecs_term(it, Video, 1);
  EcsPosition2 *pos = ecs_term(it, EcsPosition2, 2);
  EcsSquare *square = ecs_term(it, EcsSquare, 3);
  EcsRgb *colors = ecs_term(it, EcsRgb, 4);

	// Render the object as a square outline.
  for (int i = 0; i < it->count; i ++) {
    const float size = square[i].size;
    const ecs_rgb_t color = colors[i].value;
    const SDL_Rect player_rect =
      { .x = pos[i].x - size, .y = pos[i].y - size, .w = size, .h = size };
    SDL_SetRenderDrawColor(video->renderer, color.r, color.g, color.b, 0xFF);
    SDL_RenderDrawRect(video->renderer, &player_rect);
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
  WORLD_SETUP_SYS(world, render_finish, EcsPostFrame);
}
