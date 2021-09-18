#include "player.h"
WORLD_IMPLEMENT_PLAYER();

#include <flecs_components_physics.h>
#include "input.h"

// Jump by affecting the player's vertical velocity
// whenever the A button is pressed.
WORLD_DEF_SYS(player_jump, EcsVelocity2, InputButtonA) {
  EcsVelocity2 *v = ecs_term(it, EcsVelocity2, 1);

  for (int i = 0; i < it->count; i ++) {
    if (v[i].y == 0) {
      if (v[i].x > 50 || v[i].x < -50)
        v[i].y -= PLAYER_JUMP_IMPULSE;
      else
        v[i].y -= PLAYER_JUMP_IMPULSE * 0.72;
    }
  }
}

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

// Set up all these systems in the correct order of operations.
void world_setup_sys_player(World* world) {
  WORLD_SETUP_SYS(world, player_jump, EcsPostFrame);
  WORLD_SETUP_SYS(world, player_move_left, EcsPostFrame);
  WORLD_SETUP_SYS(world, player_move_right, EcsPostFrame);
}
