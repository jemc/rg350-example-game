#include <flecs_components_transform.h>
#include <flecs_components_physics.h>
#include <flecs_components_geometry.h>
#include <flecs_components_graphics.h>
#include <flecs_systems_transform.h>
#include <flecs_systems_physics.h>

#include "world.h"
#include "world/collide.h"
#include "world/force.h"
#include "world/render.h"

static ecs_entity_t player = 0;

World* world_init(Video* video, int argc, char** argv) {
  World* world = ecs_init_w_args(argc, argv);

  ECS_IMPORT(world, FlecsComponentsTransform); // EcsPosition2D
  ECS_IMPORT(world, FlecsComponentsPhysics);   // EcsVelocity2D
  ECS_IMPORT(world, FlecsComponentsGeometry);  // EcsSquare
  ECS_IMPORT(world, FlecsComponentsGraphics);  // EcsRgb

  WORLD_IMPORT_COLLIDE(world);
  WORLD_IMPORT_FORCE(world);
  WORLD_IMPORT_RENDER(world);

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

  // Set up all of the systems we use in the world, in the correct order.
  ECS_IMPORT(world, FlecsSystemsTransform);
  ECS_IMPORT(world, FlecsSystemsPhysics);
  world_setup_sys_force(world);
  world_setup_sys_collide(world);
  world_setup_sys_render(world);

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
