#ifndef WORLD_CAMERA_H
#define WORLD_CAMERA_H

#include "../world.h"

// The camera has a target it is moving toward.
typedef struct CameraTarget {
  float x;
  float y;
  bool next_immediate;
} CameraTarget;

// Define some characteristics of the camera.
#define CAMERA_PIXEL_WIDTH 320
#define CAMERA_PIXEL_HEIGHT 240
#define CAMERA_OFFSET_HORIZONTAL 38 // rounded golden ratio out of 320
#define CAMERA_OFFSET_VERTICAL 62
#define CAMERA_SLOWDOWN_HORIZONTAL 41
#define CAMERA_SLOWDOWN_VERTICAL 14
#define CAMERA_PARALLAX_FACTOR 0.1

// Forward-declare all components and entities for this module.
ECS_ENTITY_EXTERN_DECLARE(Camera);
ECS_COMPONENT_EXTERN_DECLARE(CameraTarget);

// Concretely declare all components and entities for this module.
#define WORLD_IMPLEMENT_CAMERA() \
  ECS_ENTITY_DECLARE(Camera); \
  ECS_COMPONENT_DECLARE(CameraTarget); \

// Setup all components and entities for this module in the given world.
#define WORLD_IMPORT_CAMERA(world) \
  ECS_ENTITY_DEFINE(world, Camera); \
  ECS_COMPONENT_DEFINE(world, CameraTarget); \

// Setup all systems for this module in the correct order of operations.
void world_setup_sys_camera(World* world);

// Set up all entities for this module.
void world_setup_ent_camera(World* world);

#endif
