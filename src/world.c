#include <flecs_components_transform.h>
#include <flecs_components_physics.h>
#include <flecs_components_geometry.h>
#include <flecs_components_graphics.h>
#include <flecs_systems_transform.h>
#include <flecs_systems_physics.h>

#include "world.h"

// Gravity causes a constant downward acceleration on the Y axis.
// The value here should be a positive number representing the magnitude.
typedef struct { float magnitude; } Gravity;
ECS_COMPONENT_DECLARE(Gravity);

// Apply gravity by modifying the velocity based on the gravity for that object.
#define DO_APPLY_GRAVITY(phase) ECS_SYSTEM(world, \
  do_apply_gravity, phase, \
  EcsVelocity2, Gravity)
void do_apply_gravity(ecs_iter_t *it) {
  EcsVelocity2 *v = ecs_term(it, EcsVelocity2, 1);
  Gravity *g = ecs_term(it, Gravity, 2);

  for (int i = 0; i < it->count; i ++) {
    v[i].y += g[i].magnitude * it->delta_time;
  }
}

// TODO: Flesh this out some more with real tiles.
#define DO_ACTOR_TO_TILE_COLLISIONS(phase) ECS_SYSTEM(world, \
  do_actor_to_tile_collisions, phase, \
  EcsPosition2)
void do_actor_to_tile_collisions(ecs_iter_t *it) {
  EcsPosition2 *p = ecs_term(it, EcsPosition2, 1);

  for (int i = 0; i < it->count; i ++) {
    if (p[i].y > VIDEO_HEIGHT)
      p[i].y = VIDEO_HEIGHT;
  }
}

// The Video struct will be used as a singleton entity for rendering.
ECS_COMPONENT_DECLARE(Video);

// Render the background.
#define DO_RENDER_BACKGROUND(phase) ECS_SYSTEM(world, \
  do_render_background, phase, \
  $Video)
void do_render_background(ecs_iter_t *it) {
  Video *video = ecs_term(it, Video, 1);

	const SDL_Rect background_rect =
		{ .x = 0, .y = 0, .w = VIDEO_WIDTH, .h = VIDEO_HEIGHT };

	SDL_SetRenderDrawColor(video->renderer, 0, 0, 0, 0xFF);
	SDL_RenderFillRect(video->renderer, &background_rect);

	SDL_SetRenderDrawColor(video->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderDrawRect(video->renderer, &background_rect);
}

// Render the visible square objects.
#define DO_RENDER_SQUARES(phase) ECS_SYSTEM(world, \
  do_render_squares, phase, \
  $Video, EcsPosition2, EcsSquare, EcsRgb)
void do_render_squares(ecs_iter_t *it) {
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
#define DO_RENDER_FINISH(phase) ECS_SYSTEM(world, \
  do_render_finish, phase, \
  $Video)
void do_render_finish(ecs_iter_t *it) {
  Video *video = ecs_term(it, Video, 1);

	SDL_RenderPresent(video->renderer);

  SDL_Delay(8); // TODO: Remove?
}

static ecs_entity_t player = 0;

World* world_init(Video* video, int argc, char** argv) {
  World* world = ecs_init_w_args(argc, argv);

  ECS_IMPORT(world, FlecsComponentsTransform); // EcsPosition2D
  ECS_IMPORT(world, FlecsComponentsPhysics);   // EcsVelocity2D
  ECS_IMPORT(world, FlecsComponentsGeometry);  // EcsSquare
  ECS_IMPORT(world, FlecsComponentsGraphics);  // EcsRgb
  ECS_IMPORT(world, FlecsSystemsTransform);    // Matrix transformations
  ECS_IMPORT(world, FlecsSystemsPhysics);      // Collision detection, movement

  ECS_COMPONENT_DEFINE(world, Video);
  ECS_COMPONENT_DEFINE(world, Gravity);

  // Create the video/rendering singleton.
  ecs_singleton_set(world, Video, {
    video->window,
    video->renderer,
  });

  // Create the player.
  player = ecs_set_name(world, 0, "player");
  ecs_set(world, player, EcsSquare, {16});
  ecs_set(world, player, EcsRgb, {0xFF, 0xBB, 0x88});
  ecs_set(world, player, EcsPosition2, {100, 100});
  ecs_set(world, player, EcsVelocity2, {60, -200});
  ecs_set(world, player, Gravity, {350});

  // Setup movement systemms.
  DO_APPLY_GRAVITY(EcsOnUpdate);
  DO_ACTOR_TO_TILE_COLLISIONS(EcsOnUpdate);

  // Setup rendering systemms.
  DO_RENDER_BACKGROUND(EcsPostFrame);
  DO_RENDER_SQUARES(EcsPostFrame);
  DO_RENDER_FINISH(EcsPostFrame);

  return world;
}

void world_handle_button(World* world, ButtonCode button) {

}

void world_progress(World* world) {
  ecs_progress(world, 0);
}

void world_destroy(World* world) {
  ecs_fini(world);
}
