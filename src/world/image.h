#ifndef WORLD_IMAGE_H
#define WORLD_IMAGE_H

#include "../world.h"

// An ImageSource loads an image into memory to use as a source for rendering.
// This may be, for example, a sprite sheet image, or a tile set image.
// It uses XPM-formatted source data, and SDL resources loaded from that.
typedef struct {
  char** xpm_data;
  SDL_Surface* surface;
  SDL_Texture* texture;
} ImageSource;

// Forward-declare all components and entities for this module.
ECS_COMPONENT_EXTERN_DECLARE(ImageSource);

// Concretely declare all components and entities for this module.
#define WORLD_IMPLEMENT_IMAGE() \
  ECS_COMPONENT_DECLARE(ImageSource); \

// Setup all components and entities for this module in the given world.
#define WORLD_IMPORT_IMAGE(world) \
  ECS_COMPONENT_DEFINE(world, ImageSource); \

// Setup all systems for this module in the correct order of operations.
void world_setup_sys_image(World* world);

// Set up all entities for this module.
void world_setup_ent_image(World* world);

#endif
