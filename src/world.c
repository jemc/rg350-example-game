#include <flecs_components_transform.h>
#include <flecs_components_physics.h>
#include <flecs_components_geometry.h>
#include <flecs_components_graphics.h>
#include <flecs_systems_transform.h>
#include <flecs_systems_physics.h>

#include "world.h"
#include "world/collide.h"
#include "world/force.h"
#include "world/input.h"
#include "world/player.h"
#include "world/render.h"
#include "world/room.h"

World* world_init(Video* video, int argc, char** argv) {
  World* world = ecs_init_w_args(argc, argv);

  ecs_set_target_fps(world, 60);

  ECS_IMPORT(world, FlecsComponentsTransform); // EcsPosition2D
  ECS_IMPORT(world, FlecsComponentsPhysics);   // EcsVelocity2D
  ECS_IMPORT(world, FlecsComponentsGeometry);  // EcsSquare
  ECS_IMPORT(world, FlecsComponentsGraphics);  // EcsRgb

  WORLD_IMPORT_COLLIDE(world);
  WORLD_IMPORT_FORCE(world);
  WORLD_IMPORT_INPUT(world);
  WORLD_IMPORT_PLAYER(world);
  WORLD_IMPORT_RENDER(world);
  WORLD_IMPORT_ROOM(world);

  // Create the video/rendering singleton.
  ecs_singleton_set(world, Video, {
    video->window,
    video->renderer,
  });

  // Set up all of the systems we use in the world, in the correct order.
  world_setup_sys_input(world);
  world_setup_sys_room(world);
  world_setup_sys_force(world);
  world_setup_sys_player(world);
  ECS_IMPORT(world, FlecsSystemsTransform);
  ECS_IMPORT(world, FlecsSystemsPhysics);
  world_setup_sys_collide(world);
  world_setup_sys_render(world);

  return world;
}

void world_progress(World* world) {
  ecs_progress(world, 0);
}

void world_destroy(World* world) {
  ecs_fini(world);
}

static bool has_requested_quit = false;
void world_request_quit() { has_requested_quit = true; }
bool world_has_requested_quit() { return has_requested_quit; }
