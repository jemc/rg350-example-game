#ifndef WORLD_PHYS_H
#define WORLD_PHYS_H

#include "../world.h"

// PhysPosition is a 2D vector indicating the distance along each axis
// from the origin of the coordinate space, which is the upper-left
// corner of the room. Down and to the right are in the positive direction.
typedef struct {
  float x;
  float y;
} PhysPosition;

// PhysTilePosition is similar to PhysPosition, but rather than floating-point
// pixel-basd coordinates, it indicates the tile index along each axis.
typedef struct {
  int xi;
  int yi;
} PhysTilePosition;

// PhysBounds is a 2D vector indicating the bounding box size of an object.
// An object's bounding box upper-left corner is at its position vector point,
// and the lower-right corner is found by adding it bounds vector to that point.
typedef struct {
  float w;
  float h;
} PhysBounds;

// PhysVelocity is a 2D vector indicating the change in position per frame.
// Note that this is explicitly per frame rather than per second.
// If/when the frame rate slows, gameplay will slow rather than get choppy
// (slow frame rates won't cause objects to move further per frame).
typedef struct {
  float x;
  float y;
} PhysVelocity;

// Gravity causes a constant downward acceleration on the Y axis.
// The value here should be a positive number representing the magnitude.
typedef struct {
  float magnitude;
  float terminal_speed;
} Gravity;

// Horizontal friction causes a deceleration in both directions of the X axis.
typedef struct { float magnitude; } FrictionHorizontal;

// Forward-declare all components and entities for this module.
ECS_COMPONENT_EXTERN_DECLARE(PhysPosition);
ECS_COMPONENT_EXTERN_DECLARE(PhysTilePosition);
ECS_COMPONENT_EXTERN_DECLARE(PhysBounds);
ECS_COMPONENT_EXTERN_DECLARE(PhysVelocity);
ECS_COMPONENT_EXTERN_DECLARE(Gravity);
ECS_COMPONENT_EXTERN_DECLARE(FrictionHorizontal);

// Concretely declare all components and entities for this module.
#define WORLD_IMPLEMENT_PHYS() \
  ECS_COMPONENT_DECLARE(PhysPosition); \
  ECS_COMPONENT_DECLARE(PhysTilePosition); \
  ECS_COMPONENT_DECLARE(PhysBounds); \
  ECS_COMPONENT_DECLARE(PhysVelocity); \
  ECS_COMPONENT_DECLARE(Gravity); \
  ECS_COMPONENT_DECLARE(FrictionHorizontal); \

// Setup all components and entities for this module in the given world.
#define WORLD_IMPORT_PHYS(world) \
  ECS_COMPONENT_DEFINE(world, PhysPosition); \
  ECS_COMPONENT_DEFINE(world, PhysTilePosition); \
  ECS_COMPONENT_DEFINE(world, PhysBounds); \
  ECS_COMPONENT_DEFINE(world, PhysVelocity); \
  ECS_COMPONENT_DEFINE(world, Gravity); \
  ECS_COMPONENT_DEFINE(world, FrictionHorizontal); \

// Setup all systems for this module in the correct order of operations.
void world_setup_sys_phys(World* world);

// Set up all entities for this module.
void world_setup_ent_phys(World* world);

#endif
