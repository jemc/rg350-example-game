#ifndef WORLD_SPRITE_H
#define WORLD_SPRITE_H

#include "../world.h"

typedef struct {
  const char* name;
  char** data;
  const int each_width;
  const int each_height;
} SpriteSheetSpec;

typedef struct {
  const SpriteSheetSpec* spec;
  SDL_Surface* surface;
  SDL_Texture* texture;
} SpriteSheet;

typedef struct {
  const SDL_Rect* rect;
} SpriteRect;

// Convenience macro for declaring specs in sprite sheet files.
#define DEF_SPRITE(id, w, h) \
  static const int _sprite_##sprite_id##_each_width = w;\
  static const int _sprite_##sprite_id##_each_height = h;\
  static const SpriteSheetSpec sprite_##id = {#id, sprite_##id##_data, w, h}; \

// Convenience macro for declaring SDL_rect values in sprite sheet files.
#define DEF_SPRITE_RECT(sprite_id, id, row, col) \
  static const SDL_Rect sprite_##sprite_id##_##id = { \
    .x = _sprite_##sprite_id_each_width * col, \
    .y = _sprite_##sprite_id_each_height * row, \
    .w = _sprite_##sprite_id_each_width, \
    .h = _sprite_##sprite_id_each_height, \
  }; \

// Forward-declare all components and entities for this module.
ECS_COMPONENT_EXTERN_DECLARE(SpriteSheet);
ECS_COMPONENT_EXTERN_DECLARE(SpriteRect);

// Concretely declare all components and entities for this module.
#define WORLD_IMPLEMENT_SPRITE() \
  ECS_COMPONENT_DECLARE(SpriteSheet); \
  ECS_COMPONENT_DECLARE(SpriteRect); \

// Setup all components and entities for this module in the given world.
#define WORLD_IMPORT_SPRITE(world) \
  ECS_COMPONENT_DEFINE(world, SpriteSheet); \
  ECS_COMPONENT_DEFINE(world, SpriteRect); \

// Setup all systems for this module in the correct order of operations.
void world_setup_sys_sprite(World* world);

// Set up all entities for this module.
void world_setup_ent_sprite(World* world);

#endif
