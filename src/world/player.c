#include "player.h"
WORLD_IMPLEMENT_PLAYER();

#include "phys.h"
#include "image.h"
#include "input.h"
#include "room.h"
#include "sprite.h"
#include "sprite/eyeball.h"
#include "sprite/kirbo.h"

// Move left and right by affecting the player's horizontal velocity
// whenever the left or right buttons is pressed.
WORLD_DEF_SYS(player_move_left, PhysVelocity, (InputButton, InputButtonLeft)) {
  PhysVelocity *v = ecs_term(it, PhysVelocity, 1);

  for (int i = 0; i < it->count; i++) {
    if (v[i].x >= -(PLAYER_MAX_SPEED_HORIZONTAL - PLAYER_ACCEL_HORIZONTAL))
      v[i].x -= PLAYER_ACCEL_HORIZONTAL;
    v[i].x -= PLAYER_FRICTION_HORIZONTAL;
  }
}
WORLD_DEF_SYS(player_move_right, PhysVelocity, (InputButton, InputButtonRight)) {
  PhysVelocity *v = ecs_term(it, PhysVelocity, 1);

  for (int i = 0; i < it->count; i++) {
    if (v[i].x <= (PLAYER_MAX_SPEED_HORIZONTAL - PLAYER_ACCEL_HORIZONTAL))
      v[i].x += PLAYER_ACCEL_HORIZONTAL;
    v[i].x += PLAYER_FRICTION_HORIZONTAL;
  }
}

// Jump by affecting the player's vertical velocity
// whenever the A button is pressed.
WORLD_DEF_SYS(player_jump, PhysVelocity, Gravity, (InputButton, InputButtonA)) {
  PhysVelocity *v = ecs_term(it, PhysVelocity, 1);
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
WORLD_DEF_SYS(player_direction_horizontal, PlayerDirection, PhysVelocity) {
  PlayerDirection *dir = ecs_term(it, PlayerDirection, 1);
  PhysVelocity *v = ecs_term(it, PhysVelocity, 2);

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

// Figure out what vertical direction the player is looking, if any.
WORLD_DEF_SYS(player_direction_vertical,
  PlayerDirection,
  PhysVelocity,
  ?(InputButton, InputButtonUp),
  ?(InputButton, InputButtonDown),
) {
  PlayerDirection *dir = ecs_term(it, PlayerDirection, 1);
  PhysVelocity *v = ecs_term(it, PhysVelocity, 2);
  bool button_up = ecs_term_is_set(it, 3);
  bool button_down = ecs_term_is_set(it, 4);

  for (int i = 0; i < it->count; i++) {
    if (
      // If standing still and facing inward, or...
      (v[i].x == 0 && !dir[i].leftward && !dir[i].rightward) ||
      // Facing away from the camera to interact,
      dir[i].awayward
      // Do not face up or down, and stop facing away the next frame.
    ) {
      dir[i].upward = false;
      dir[i].downward = false;
      dir[i].awayward = false;
    } else if (button_up) {
      dir[i].upward = true;
      dir[i].downward = false;
    } else if (button_down) {
      dir[i].upward = false;
      dir[i].downward = true;
    } else {
      dir[i].upward = false;
      dir[i].downward = false;
    }
  }
}

// Choose an appropriate sprite choice based on the player's velocity,
// as well as other factors that affect the look of the character.
WORLD_DEF_SYS(player_choose_sprite,
  SpriteChoice, PlayerDirection, PhysVelocity
) {
  SpriteChoice *sprite = ecs_term(it, SpriteChoice, 1);
  PlayerDirection *dir = ecs_term(it, PlayerDirection, 2);
  PhysVelocity *v = ecs_term(it, PhysVelocity, 3);

  for (int i = 0; i < it->count; i++) {
    sprite[i].flip = dir[i].leftward ? SDL_FLIP_HORIZONTAL : 0;

    if (v[i].y == 0) {
      if (dir[i].leftward || dir[i].rightward) {
        if(v[i].x == 0) {
          // Standing.
          if (dir[i].upward) {
            sprite[i].rect = &sprite_eyeball_look_up_default;
          } else if (dir[i].downward) {
            sprite[i].rect = &sprite_eyeball_look_down_default;
          } else {
            sprite[i].rect = &sprite_eyeball_default;
          }
        } else {
          // Walking.
          switch (world_frame_number(it->world) / 3 % 10) {
          case 0: case 2:
            if (dir[i].upward) {
              sprite[i].rect = &sprite_eyeball_look_up_step_l1;
            } else if (dir[i].downward) {
              sprite[i].rect = &sprite_eyeball_look_down_step_l1;
            } else {
              sprite[i].rect = &sprite_eyeball_step_l1;
            }
            break;
          case 1:
            if (dir[i].upward) {
              sprite[i].rect = &sprite_eyeball_look_up_step_l2;
            } else if (dir[i].downward) {
              sprite[i].rect = &sprite_eyeball_look_down_step_l2;
            } else {
              sprite[i].rect = &sprite_eyeball_step_l2;
            }
            break;
          case 4: case 8:
            if (dir[i].upward) {
              sprite[i].rect = &sprite_eyeball_look_up_step_md;
            } else if (dir[i].downward) {
              sprite[i].rect = &sprite_eyeball_look_down_step_md;
            } else {
              sprite[i].rect = &sprite_eyeball_step_md;
            }
            break;
          case 5: case 7:
            if (dir[i].upward) {
              sprite[i].rect = &sprite_eyeball_look_up_step_r1;
            } else if (dir[i].downward) {
              sprite[i].rect = &sprite_eyeball_look_down_step_r1;
            } else {
              sprite[i].rect = &sprite_eyeball_step_r1;
            }
            break;
          case 6:
            if (dir[i].upward) {
              sprite[i].rect = &sprite_eyeball_look_up_step_r2;
            } else if (dir[i].downward) {
              sprite[i].rect = &sprite_eyeball_look_down_step_r2;
            } else {
              sprite[i].rect = &sprite_eyeball_step_r2;
            }
            break;
          default:
            if (dir[i].upward) {
              sprite[i].rect = &sprite_eyeball_look_up_default;
            } else if (dir[i].downward) {
              sprite[i].rect = &sprite_eyeball_look_down_default;
            } else {
              sprite[i].rect = &sprite_eyeball_default;
            }
          }
        }
      } else {
        // Standing, looking inward.
        sprite[i].rect = &sprite_eyeball_frontal_tall;
      }
    } else if (v[i].y < -1) {
      // Jumping (upward).
      if (dir[i].upward) {
        sprite[i].rect = &sprite_eyeball_look_up_jump_up;
      } else if (dir[i].downward) {
        sprite[i].rect = &sprite_eyeball_look_down_jump_up;
      } else {
        sprite[i].rect = &sprite_eyeball_jump_up;
      }
    } else if (v[i].y > 1) {
      // Jumping (mid-jump).
      if (dir[i].upward) {
        sprite[i].rect = &sprite_eyeball_look_up_jump_down;
      } else if (dir[i].downward) {
        sprite[i].rect = &sprite_eyeball_look_down_jump_down;
      } else {
        sprite[i].rect = &sprite_eyeball_jump_down;
      }
    } else {
      // Falling.
      if (dir[i].upward) {
        sprite[i].rect = &sprite_eyeball_look_up_jump_mid;
      } else if (dir[i].downward) {
        sprite[i].rect = &sprite_eyeball_look_down_jump_mid;
      } else {
        sprite[i].rect = &sprite_eyeball_jump_mid;
      }
    }
  }
}

// Set up all these systems in the correct order of operations.
void world_setup_sys_player(World* world) {
  WORLD_SETUP_SYS(world, player_move_left, EcsPreUpdate);
  WORLD_SETUP_SYS(world, player_move_right, EcsPreUpdate);
  WORLD_SETUP_SYS(world, player_jump, EcsPreUpdate);
  WORLD_SETUP_SYS(world, player_direction_horizontal, EcsPostUpdate);
  WORLD_SETUP_SYS(world, player_direction_vertical, EcsPostUpdate);
  WORLD_SETUP_SYS(world, player_choose_sprite, EcsPreStore);
}

// Set up all entities for this module.
void world_setup_ent_player(World* world) {
  // ecs_set_pair(world, Player, InRoom, Room1, {});
  ecs_add(world, Player, IsPlayer);
  ecs_set(world, Player, ImageSource, {sprite_eyeball.data});
  ecs_set_ptr(world, Player, SpriteSheet, &sprite_eyeball);
  ecs_set(world, Player, SpriteChoice, {&sprite_eyeball_frontal_tall});
  ecs_set(world, Player, PhysBounds, {PLAYER_HEIGHT, PLAYER_HEIGHT});
  ecs_set(world, Player, Gravity, {PLAYER_GRAVITY, PLAYER_GRAVITY_TERMINAL_SPEED});
  ecs_set(world, Player, FrictionHorizontal, {PLAYER_FRICTION_HORIZONTAL});
  ecs_set(world, Player, PlayerDirection, {});
  ecs_set(world, Player, PhysVelocity, {});


  ecs_set(world, Player, PhysPosition,
    {36 * ROOM_TILE_SIZE, 31 * ROOM_TILE_SIZE});
}
