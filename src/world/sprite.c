#include "sprite.h"
WORLD_IMPLEMENT_SPRITE();

#include <SDL2/SDL_image.h>

#include "render.h"

// Ensure that the given SpriteSheet component is loaded when saving to ECS.
// Typically the entity definition code that sets the SpriteSheet component
// will just set the SpriteSheetSpec, and then it's the job of this function
// to load the data from the spec into an SDL surface and texture for use.
static void sprite_sheet_load(
  ecs_world_t *world,
  ecs_entity_t component,
  const ecs_entity_t *dst_entity,
  const ecs_entity_t *src_entity,
  void *dst_ptr,
  const void *src_ptr,
  size_t size,
  int32_t count,
  void *ctx
) {
  SpriteSheet* sheet = (SpriteSheet*)dst_ptr;

  // Copy the spec pointer from the source component.
  sheet->spec = ((SpriteSheet*)src_ptr)->spec;

  // Load the raw data from the sprite sheet spec into an SDL surface.
  sheet->surface = IMG_ReadXPMFromArray(sheet->spec->data);
  if (!sheet->surface) {
    printf("SpriteSheet failed to load: %s\n", sheet->spec->name);
    return;
  }

  // Create an SDL texture that maps onto the sprite sheet surface.
  sheet->texture = SDL_CreateTextureFromSurface(
    ecs_singleton_get(world, Video)->renderer,
    sheet->surface
  );
  if (!sheet->texture) {
    printf("SpriteSheet failed to create texture: %s\n", sheet->spec->name);
    return;
  }
}

// Ensure that the SDL resources for a given SpriteSheet get freed when
// the component gets destroyed.
static void sprite_sheet_free(
  ecs_world_t *world,
  ecs_entity_t component,
  const ecs_entity_t *entity_ptr,
  void *ptr,
  size_t size,
  int32_t count,
  void *ctx
) {
  SpriteSheet* sheet = (SpriteSheet*)ptr;

  if(sheet->texture) SDL_DestroyTexture(sheet->texture);
  if(sheet->surface) SDL_FreeSurface(sheet->surface);
}

// Set up all these systems in the correct order of operations.
void world_setup_sys_sprite(World* world) {
  // Set up lifecycle hooks for sprite sheet loading/freeing in SDL.
  ecs_set_component_actions(world, SpriteSheet, {
    .copy = sprite_sheet_load,
    .dtor = sprite_sheet_free,
  });
}

// Set up all entities for this module.
void world_setup_ent_sprite(World* world) {
  // Empty
}
