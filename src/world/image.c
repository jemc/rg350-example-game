#include "image.h"
WORLD_IMPLEMENT_IMAGE();

#include <SDL2/SDL_image.h>

#include "render.h"

// Ensure that the given ImageSource component is loaded when saving to ECS.
// Typically the entity definition code that sets the ImageSource component
// will just set the XPM data, and then it's the job of this function
// to load the data from into an SDL surface and texture for use.
static void image_source_load(
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
  ImageSource* image = (ImageSource*)dst_ptr;

  // Copy the spec pointer from the source component.
  image->xpm_data = ((ImageSource*)src_ptr)->xpm_data;

  // Load the raw data from the sprite image spec into an SDL surface.
  image->surface = IMG_ReadXPMFromArray(image->xpm_data);
  ecs_assert(image->surface != NULL, ECS_INVALID_PARAMETER, NULL);
  if (!image->surface) {
    printf("ImageSource failed to load.\n");
    return;
  }

  // Create an SDL texture that maps onto the sprite image surface.
  image->texture = SDL_CreateTextureFromSurface(
    ecs_singleton_get(world, Video)->renderer,
    image->surface
  );
  ecs_assert(image->texture != NULL, ECS_INVALID_PARAMETER, NULL);
  if (!image->texture) {
    printf("ImageSource failed to create texture.\n");
    return;
  }
}

// Ensure that the SDL resources for a given ImageSource get freed when
// the component gets destroyed.
static void image_source_free(
  ecs_world_t *world,
  ecs_entity_t component,
  const ecs_entity_t *entity_ptr,
  void *ptr,
  size_t size,
  int32_t count,
  void *ctx
) {
  ImageSource* image = (ImageSource*)ptr;

  if(image->texture) SDL_DestroyTexture(image->texture);
  if(image->surface) SDL_FreeSurface(image->surface);
}

// Set up all these systems in the correct order of operations.
void world_setup_sys_image(World* world) {
  // Set up lifecycle hooks for tile set loading/freeing in SDL.
  ecs_set_component_actions(world, ImageSource, {
    .copy = image_source_load,
    .dtor = image_source_free,
  });
}

// Set up all entities for this module.
void world_setup_ent_image(World* world) {
  // Empty
}
