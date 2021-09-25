#include "player.h"
WORLD_IMPLEMENT_PLAYER();

#include <flecs.h>
#include <flecs_components_transform.h>
#include <flecs_components_physics.h>
#include <flecs_components_geometry.h>
#include <flecs_components_graphics.h>
#include "force.h"
#include "input.h"
#include "room.h"
#include "sprite.h"
#include "sprite/eyeball.h"
#include "sprite/kirbo.h"

// Move left and right by affecting the player's horizontal velocity
// whenever the left or right buttons is pressed.
WORLD_DEF_SYS(player_move_left, EcsVelocity2, (InputButton, InputButtonLeft)) {
  EcsVelocity2 *v = ecs_term(it, EcsVelocity2, 1);

  for (int i = 0; i < it->count; i++) {
    if (v[i].x >= -(PLAYER_MAX_SPEED_HORIZONTAL - PLAYER_ACCEL_HORIZONTAL))
      v[i].x -= PLAYER_ACCEL_HORIZONTAL;
    v[i].x -= PLAYER_FRICTION_HORIZONTAL;
  }
}
WORLD_DEF_SYS(player_move_right, EcsVelocity2, (InputButton, InputButtonRight)) {
  EcsVelocity2 *v = ecs_term(it, EcsVelocity2, 1);

  for (int i = 0; i < it->count; i++) {
    if (v[i].x <= (PLAYER_MAX_SPEED_HORIZONTAL - PLAYER_ACCEL_HORIZONTAL))
      v[i].x += PLAYER_ACCEL_HORIZONTAL;
    v[i].x += PLAYER_FRICTION_HORIZONTAL;
  }
}

// Jump by affecting the player's vertical velocity
// whenever the A button is pressed.
WORLD_DEF_SYS(player_jump, EcsVelocity2, Gravity, (InputButton, InputButtonA)) {
  EcsVelocity2 *v = ecs_term(it, EcsVelocity2, 1);
  Gravity *g = ecs_term(it, Gravity, 2);
  ecs_entity_t button_pair = ecs_term_id(it, 3);

  for (int i = 0; i < it->count; i++) {
    InputButton* button = ECS_CAST(InputButton*,
      ecs_get_id(it->world, it->entities[i], button_pair)
    );
    int32_t hold_frames = world_frame_number(it->world) - button->initial_frame;

    // For the first frame of button press, start the jump.
    if (hold_frames == 0) {
      if (v[i].y == 0)
        v[i].y -= PLAYER_JUMP_IMPULSE;
    }

    // For the first few frames of holding the button, also defy gravity a bit.
    if (hold_frames <= PLAYER_JUMP_DEFY_GRAVITY_FRAMES) {
      if (v[i].y <= PLAYER_JUMP_IMPULSE * -0.9)
        v[i].y -= g[i].magnitude;
    }
  }
}

// Figure out what horizontal direction the player is facing.
WORLD_DEF_SYS(player_direction_horizontal, PlayerDirection, EcsVelocity2) {
  PlayerDirection *dir = ecs_term(it, PlayerDirection, 1);
  EcsVelocity2 *v = ecs_term(it, EcsVelocity2, 2);

  for (int i = 0; i < it->count; i++) {
    if (v[i].y == 0) {
      if (v[i].x > 0) {
        // If the player is walking rightward, but not yet facing rightward,
        // cancel leftward facing immediately, and face rightward if the
        // movement is fast enough. Otherwise the player will face inward.
        if (!dir[i].rightward) {
          dir[i].leftward = false;
          if (v[i].x > 0.5) {
            dir[i].rightward = true;
            dir[i].recently_rightward = true;
            dir[i].recently_leftward = false;
          }
        }
      } else if (v[i].x < 0) {
        // If the player is walking leftward, but not yet facing leftward,
        // cancel rightward facing immediately, and face leftward if the
        // movement is fast enough. Otherwise the player will face inward.
        if (!dir[i].leftward) {
          dir[i].rightward = false;
          if (v[i].x < -0.5) {
            dir[i].leftward = true;
            dir[i].recently_leftward = true;
            dir[i].recently_rightward = false;
          }
        }
      }
    } else {
      // If moving up or down, it is not possible to face "inward",
      // so face whichever way the player was most recently facing,
      // or face rightward if no direction was recently known.
      if (!dir[i].leftward && !dir[i].rightward) {
        if (dir[i].recently_leftward) {
          dir[i].leftward = true;
        } else {
          dir[i].rightward = true;
        }
      }
    }
  }
}

// Choose an appropriate sprite choice based on the player's velocity,
// as well as other factors that affect the look of the character.
WORLD_DEF_SYS(player_choose_sprite,
  SpriteChoice, PlayerDirection, EcsVelocity2
) {
  SpriteChoice *sprite = ecs_term(it, SpriteChoice, 1);
  PlayerDirection *dir = ecs_term(it, PlayerDirection, 2);
  EcsVelocity2 *v = ecs_term(it, EcsVelocity2, 3);

  for (int i = 0; i < it->count; i++) {
    sprite[i].flip = dir[i].leftward ? SDL_FLIP_HORIZONTAL : 0;

    if (v[i].y == 0) {
      if (dir[i].leftward || dir[i].rightward) {
        sprite[i].rect = &sprite_eyeball_default;
      } else {
        sprite[i].rect = &sprite_eyeball_frontal_tall;
      }
    } else if (v[i].y < -1) {
      sprite[i].rect = &sprite_eyeball_jump_up;
    } else if (v[i].y > 1) {
      sprite[i].rect = &sprite_eyeball_jump_down;
    } else {
      sprite[i].rect = &sprite_eyeball_jump_mid;
    }
  }
}

// Set up all these systems in the correct order of operations.
void world_setup_sys_player(World* world) {
  WORLD_SETUP_SYS(world, player_direction_horizontal, EcsPostUpdate);
  WORLD_SETUP_SYS(world, player_choose_sprite, EcsPreStore);
  WORLD_SETUP_SYS(world, player_move_left, EcsPostFrame);  // TODO: different phase?
  WORLD_SETUP_SYS(world, player_move_right, EcsPostFrame); // TODO: different phase?
  WORLD_SETUP_SYS(world, player_jump, EcsPostFrame);       // TODO: different phase?
}

// Set up all entities for this module.
void world_setup_ent_player(World* world) {
  // TODO: Remove these.
  ECS_IMPORT(world, FlecsComponentsTransform);
  ECS_IMPORT(world, FlecsComponentsPhysics);
  ECS_IMPORT(world, FlecsComponentsGeometry);
  ECS_IMPORT(world, FlecsComponentsGraphics);

  ecs_set_pair(world, Player, InRoom, Room1, {});
  ecs_add(world, Player, IsPlayer);
  ecs_set(world, Player, SpriteSheet, {&sprite_eyeball});
  ecs_set(world, Player, SpriteChoice, {&sprite_eyeball_frontal_tall});
  ecs_set(world, Player, EcsSquare, {PLAYER_HEIGHT});
  ecs_set(world, Player, Gravity, {PLAYER_GRAVITY, PLAYER_GRAVITY_TERMINAL_SPEED});
  ecs_set(world, Player, FrictionHorizontal, {PLAYER_FRICTION_HORIZONTAL});
  ecs_set(world, Player, PlayerDirection, {});
  ecs_set(world, Player, EcsVelocity2, {0, 0});
  ecs_set(world, Player, EcsPosition2,
    {(VIDEO_WIDTH + PLAYER_HEIGHT) / 2, VIDEO_HEIGHT / 2});
}
