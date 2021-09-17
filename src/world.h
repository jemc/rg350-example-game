#ifndef WORLD_H
#define WORLD_H

#include <flecs.h>

// TODO: refactor these into ECS-oriented modules.
#include "button.h"
#include "video.h"

typedef ecs_world_t World;

// Initialize world entities and components.
World* world_init(Video* video, int argc, char** argv);

// Handle input from a button press and its effect on the world.
void world_handle_button(World* world, ButtonCode button);

// Handle a frame's worth of action in the world.
void world_progress(World* world);

// Teardown world entities and components.
void world_destroy(World* world);

// This is a convenience macro for declaring an ECS system function and
// its associated filter expression, for use with the WORLD_SETUP_SYS macro.
#define WORLD_DEF_SYS(id, ...) \
  static const char* world_sys_do_##id##_filter_expr = #__VA_ARGS__; \
  static void world_sys_do_##id(ecs_iter_t *it)

// This is a modified version of the ECS_SYSTEM macro,
// allowing us to use the system terms and function declared by WORLD_DEF_SYS.
#define WORLD_SETUP_SYS(world, id, kind) \
  ecs_iter_action_t ecs_iter_action(id) = world_sys_do_##id;\
  ecs_entity_t id = ecs_system_init(world, &(ecs_system_desc_t){\
    .entity = { .name = #id, .add = {kind} },\
    .query.filter.expr = world_sys_do_##id##_filter_expr,\
    .callback = ecs_iter_action(id)\
  });\
  ecs_assert(id != 0, ECS_INVALID_PARAMETER, NULL);\
  (void)ecs_iter_action(id);\
  (void)id;

#endif
