#ifndef WORLD_SPRITE_H
#define WORLD_SPRITE_H

#include "../world.h"

typedef struct {
  const char* name;
  char** data;
  const int each_width;
  const int each_height;
} SpriteSheet;

typedef struct {
  const SDL_Rect* rect;
  SDL_RendererFlip flip;
  double angle;
} SpriteChoice;

// Convenience macro for declaring specs in sprite sheet files.
#define DEF_SPRITE(id, w, h) \
  static const SpriteSheet sprite_##id = { #id, sprite_##id##_data, w, h };

// Convenience macro for declaring SDL_rect values in sprite sheet files.
#define DEF_SPRITE_RECT(sprite_id, each_width, each_height, id, row, col) \
  static const SDL_Rect sprite_##sprite_id##_##id = { \
    .x = each_width * col, \
    .y = each_height * row, \
    .w = each_width, \
    .h = each_height, \
  }; \

// Forward-declare all components and entities for this module.
ECS_COMPONENT_EXTERN_DECLARE(SpriteSheet);
ECS_COMPONENT_EXTERN_DECLARE(SpriteChoice);

// Concretely declare all components and entities for this module.
#define WORLD_IMPLEMENT_SPRITE() \
  ECS_COMPONENT_DECLARE(SpriteSheet); \
  ECS_COMPONENT_DECLARE(SpriteChoice); \

// Setup all components and entities for this module in the given world.
#define WORLD_IMPORT_SPRITE(world) \
  ECS_COMPONENT_DEFINE(world, SpriteSheet); \
  ECS_COMPONENT_DEFINE(world, SpriteChoice); \

// Setup all systems for this module in the correct order of operations.
void world_setup_sys_sprite(World* world);

// Set up all entities for this module.
void world_setup_ent_sprite(World* world);

#endif
