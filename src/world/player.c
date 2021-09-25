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
    if (v[i].x > -150)
      v[i].x -= 10;
  }
}
WORLD_DEF_SYS(player_move_right, EcsVelocity2, (InputButton, InputButtonRight)) {
  EcsVelocity2 *v = ecs_term(it, EcsVelocity2, 1);

  for (int i = 0; i < it->count; i++) {
    if (v[i].x < 150)
      v[i].x += 10;
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
      if (v[i].y == -PLAYER_JUMP_IMPULSE)
        v[i].y -= g[i].magnitude;
    }
  }
}

// Choose an appropriate sprite choice based on the player's velocity,
// as well as other factors that affect the look of the character.
WORLD_DEF_SYS(player_choose_sprite, SpriteChoice, EcsVelocity2) {
  SpriteChoice *sprite = ecs_term(it, SpriteChoice, 1);
  EcsVelocity2 *v = ecs_term(it, EcsVelocity2, 2);

  for (int i = 0; i < it->count; i++) {
    if (v[i].y == 0) {
      if (v[i].x > 0) {
        if (sprite[i].flip != 0) {
          sprite[i].rect = &sprite_eyeball_frontal_tall;
          if (v[i].x > 30) sprite[i].flip = 0;
        } else {
          sprite[i].rect = &sprite_eyeball_default;
        }
      } else if (v[i].x < 0) {
        if (sprite[i].flip != SDL_FLIP_HORIZONTAL) {
          sprite[i].rect = &sprite_eyeball_frontal_tall;
          if (v[i].x < -30) sprite[i].flip = SDL_FLIP_HORIZONTAL;
        } else {
          sprite[i].rect = &sprite_eyeball_default;
        }
      } else if (sprite[i].rect == &sprite_eyeball_jump_down) {
        sprite[i].rect = &sprite_eyeball_default;
      }
    } else if (v[i].y < -60) {
      sprite[i].rect = &sprite_eyeball_jump_up;
    } else if (v[i].y > 60) {
      sprite[i].rect = &sprite_eyeball_jump_down;
    } else {
      sprite[i].rect = &sprite_eyeball_jump_mid;
    }
  }
}

// Set up all these systems in the correct order of operations.
void world_setup_sys_player(World* world) {
  WORLD_SETUP_SYS(world, player_choose_sprite, EcsPostFrame);
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

  ecs_set_pair(world, Player, InRoom, Room1, {});
  ecs_set(world, Player, SpriteSheet, {&sprite_eyeball});
  ecs_set(world, Player, SpriteChoice, {&sprite_eyeball_frontal_tall});
  ecs_set(world, Player, EcsSquare, {PLAYER_HEIGHT});
  ecs_set(world, Player, Gravity, {PLAYER_GRAVITY});
  ecs_set(world, Player, FrictionHorizontal, {5});
  ecs_set(world, Player, EcsVelocity2, {0, 0});
  ecs_set(world, Player, EcsPosition2,
    {(VIDEO_WIDTH + PLAYER_HEIGHT) / 2, VIDEO_HEIGHT / 2});
}
