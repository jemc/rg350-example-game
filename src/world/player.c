#include "player.h"
WORLD_IMPLEMENT_PLAYER();

#include <flecs.h>
#include <flecs_components_transform.h>
#include <flecs_components_physics.h>
#include <flecs_components_geometry.h>
#include <flecs_components_graphics.h>
#include "force.h"
#include "input.h"
#include "sprite.h"
#include "sprite/eyeball.h"
// #include "sprite/kirbo.h"

// Move left and right by affecting the player's horizontal velocity
// whenever the left or right buttons is pressed.
WORLD_DEF_SYS(player_move_left, EcsVelocity2, InputButtonLeft) {
  EcsVelocity2 *v = ecs_term(it, EcsVelocity2, 1);

  for (int i = 0; i < it->count; i ++) {
    if (v[i].x > -150)
      v[i].x -= 600 * it->delta_time;
  }
}
WORLD_DEF_SYS(player_move_right, EcsVelocity2, InputButtonRight) {
  EcsVelocity2 *v = ecs_term(it, EcsVelocity2, 1);

  for (int i = 0; i < it->count; i ++) {
    if (v[i].x < 150)
      v[i].x += 600 * it->delta_time;
  }
}

// Jump by affecting the player's vertical velocity
// whenever the A button is pressed.
WORLD_DEF_SYS(player_jump, EcsVelocity2, InputButtonA) {
  EcsVelocity2 *v = ecs_term(it, EcsVelocity2, 1);

  for (int i = 0; i < it->count; i ++) {
    if (v[i].y == 0) {
      if (v[i].x > 120 || v[i].x < -120)
        v[i].y -= PLAYER_JUMP_IMPULSE;
      else
        v[i].y -= PLAYER_JUMP_IMPULSE * 0.72;
    }
  }
}

// Set up all these systems in the correct order of operations.
void world_setup_sys_player(World* world) {
  WORLD_SETUP_SYS(world, player_move_left, EcsPostFrame);
  WORLD_SETUP_SYS(world, player_move_right, EcsPostFrame);
  WORLD_SETUP_SYS(world, player_jump, EcsPostFrame);
}

// Set up all entities for this module.
void world_setup_ent_player(World* world) {
  // TODO: Remove these.
  ECS_IMPORT(world, FlecsComponentsTransform);
  ECS_IMPORT(world, FlecsComponentsPhysics);
  ECS_IMPORT(world, FlecsComponentsGeometry);
  ECS_IMPORT(world, FlecsComponentsGraphics);

  ecs_set(world, Player, SpriteSheet, {&sprite_eyeball});
  ecs_set(world, Player, SpriteRect, {&sprite_eyeball_jump_up});
  ecs_set(world, Player, EcsSquare, {PLAYER_HEIGHT});
  ecs_set(world, Player, Gravity, {PLAYER_GRAVITY});
  ecs_set(world, Player, FrictionHorizontal, {290});
  ecs_set(world, Player, EcsVelocity2, {0, 0});
  ecs_set(world, Player, EcsPosition2,
    {(VIDEO_WIDTH + PLAYER_HEIGHT) / 2, VIDEO_HEIGHT / 2});
}
