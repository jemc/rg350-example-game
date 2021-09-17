#ifndef WORLD_H
#define WORLD_H

#include "button.h"
#include "video.h"
#include "../lib/flecs.h"

typedef ecs_world_t World;

// Initialize world entities and components.
World* world_init(Video* video, int argc, char** argv);

// Handle input from a button press and its effect on the world.
void world_handle_button(World* world, ButtonCode button);

// Handle a frame's worth of action in the world.
void world_progress(World* world);

// Teardown world entities and components.
void world_destroy(World* world);

#endif
