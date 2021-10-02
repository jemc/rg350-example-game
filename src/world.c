#include "world.h"
#include "world/camera.h"
#include "world/collide.h"
#include "world/image.h"
#include "world/input.h"
#include "world/interact.h"
#include "world/phys.h"
#include "world/player.h"
#include "world/render.h"
#include "world/room.h"
#include "world/sprite.h"

World* world_init(Video* video, int argc, char** argv) {
  World* world = ecs_init_w_args(argc, argv);

  ecs_set_target_fps(world, VIDEO_FRAME_RATE);

  WORLD_IMPORT_CAMERA(world);
  WORLD_IMPORT_COLLIDE(world);
  WORLD_IMPORT_INPUT(world);
  WORLD_IMPORT_IMAGE(world);
  WORLD_IMPORT_INTERACT(world);
  WORLD_IMPORT_PHYS(world);
  WORLD_IMPORT_PLAYER(world);
  WORLD_IMPORT_RENDER(world);
  WORLD_IMPORT_ROOM(world);
  WORLD_IMPORT_SPRITE(world);

  // Create the video/rendering singleton.
  ecs_singleton_set(world, Video, {
    video->window,
    video->renderer,
  });

  // Set up all of the systems we use in the world, in the correct order.
  world_setup_sys_image(world);
  world_setup_sys_input(world);
  world_setup_sys_interact(world);
  world_setup_sys_room(world);
  world_setup_sys_phys(world);
  world_setup_sys_player(world);
  world_setup_sys_collide(world);
  world_setup_sys_sprite(world);
  world_setup_sys_camera(world);
  world_setup_sys_render(world);

  // Set up all the entities from all modules.
  world_setup_ent_camera(world);
  world_setup_ent_collide(world);
  world_setup_ent_image(world);
  world_setup_ent_input(world);
  world_setup_ent_interact(world);
  world_setup_ent_phys(world);
  world_setup_ent_player(world);
  world_setup_ent_render(world);
  world_setup_ent_room(world, RoomIdRoom1);
  world_setup_ent_sprite(world);

  return world;
}

void world_progress(World* world) {
  ecs_progress(world, 0);
}

int32_t world_frame_number(World* world) {
  return ecs_get_world_info(world)->frame_count_total;
}

void world_destroy(World* world) {
  ecs_fini(world);
}

static bool has_requested_quit = false;
void world_request_quit() { has_requested_quit = true; }
bool world_has_requested_quit() { return has_requested_quit; }
