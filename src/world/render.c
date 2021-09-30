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
WORLD_DEF_SYS(render_room_layer, $Video, $Camera, RoomVisualLayer) {
  // Render the object as a square outline.
  const int size = ROOM_TILE_SIZE;
  Video *video = ecs_term(it, Video, 1);
  Camera *cam = ecs_term(it, Camera, 2);
  RoomVisualLayer *room = ecs_term(it, RoomVisualLayer, 3);

  for (int i = 0; i < it->count; i ++) {
    // We can't draw the room to the canvas if it hasn't been rendered yet.
    if (room[i].texture == NULL) return;

    // If parallax is enabled for this layer, it scales the camera motion.
    const float parallax_factor = room[i].parallax_factor;
    const float cam_x = parallax_factor ? cam->x * parallax_factor : cam->x;
    const float cam_y = parallax_factor ? cam->y * parallax_factor : cam->y;

    // Determine the start and end of x index and y index iteration for
    // rendering only the tiles that are within view of the camera.
    int x_init = cam_x;
    int y_init = cam_y;
    const int x_end = x_init + CAMERA_PIXEL_WIDTH;
    const int y_end = y_init + CAMERA_PIXEL_HEIGHT;

    const int x_smoothing = VIDEO_SCALE * (cam_x - x_init);
    const int y_smoothing = VIDEO_SCALE * (cam_y - y_init);

    // Tile iterating can't start outside the upper-left bounds of the room.
    if (x_init < 0) x_init = 0;
    if (y_init < 0) y_init = 0;

    // Tile iterating can't start outside the lower-right bounds of the room.
    const int room_x_end = x_end <= room[i].width ? x_end : room[i].width;
    const int room_y_end = y_end <= room[i].height ? y_end : room[i].height;

    // Set the location to copy from in the tile set.
    const SDL_Rect src_rect = {
      .x = x_init,
      .y = y_init,
      .w = x_end - x_init + 1,
      .h = y_end - y_init + 1
    };

    // Set the location to copy to in the render canvas.
    const SDL_Rect dst_rect = {
      .x = - x_smoothing,
      .y = - y_smoothing,
      .w = VIDEO_SCALE * (CAMERA_PIXEL_WIDTH + 1),
      .h = VIDEO_SCALE * (CAMERA_PIXEL_HEIGHT + 1)
    };

    // Render the tile by copying from the source to the destination.
    SDL_RenderCopy(video->renderer, room[i].texture, &src_rect, &dst_rect);
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
