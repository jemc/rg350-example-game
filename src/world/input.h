#ifndef WORLD_INPUT_H
#define WORLD_INPUT_H

#include <SDL2/SDL.h>

#include "../world.h"

typedef struct InputButton {
  int initial_frame; // the frame number of the first frame of button press.
} InputButton;

// Define the button mapping as a macro that yields to the given macro once
// for each button that is defined in the button mapping.
// TODO: Define a different button mapping for desktop devices.
#define INPUT_FOR_EACH_BUTTON(yield) \
  yield(InputButtonLeft,       SDL_SCANCODE_LEFT); \
  yield(InputButtonRight,      SDL_SCANCODE_RIGHT); \
  yield(InputButtonUp,         SDL_SCANCODE_UP); \
  yield(InputButtonDown,       SDL_SCANCODE_DOWN); \
  yield(InputButtonA,          SDL_SCANCODE_LCTRL); \
  yield(InputButtonB,          SDL_SCANCODE_LALT); \
  yield(InputButtonX,          SDL_SCANCODE_LSHIFT); \
  yield(InputButtonY,          SDL_SCANCODE_SPACE); \
  yield(InputButtonL1,         SDL_SCANCODE_TAB); \
  yield(InputButtonR1,         SDL_SCANCODE_BACKSPACE); \
  yield(InputButtonL2,         SDL_SCANCODE_PAGEUP); \
  yield(InputButtonR2,         SDL_SCANCODE_PAGEDOWN); \
  yield(InputButtonL3,         SDL_SCANCODE_KP_DIVIDE); \
  yield(InputButtonR3,         SDL_SCANCODE_KP_PERIOD); \
  yield(InputButtonSelect,     SDL_SCANCODE_ESCAPE); \
  yield(InputButtonStart,      SDL_SCANCODE_RETURN); \
  yield(InputButtonPower,      SDL_SCANCODE_HOME); \
  yield(InputButtonVolumeDown, SDL_SCANCODE_VOLUMEDOWN); \
  yield(InputButtonVolumeUp,   SDL_SCANCODE_VOLUMEUP); \

// And these are the macros that we use as inputs to the above macro.
#define INPUT_EACH_BUTTON_EXTERN_DECLARE(id, sdl_code) \
  ECS_ENTITY_EXTERN_DECLARE(id)
#define INPUT_EACH_BUTTON_DECLARE(id, sdl_code) \
  ECS_ENTITY_DECLARE(id)
#define INPUT_EACH_BUTTON_DEFINE(id, sdl_code) \
  ECS_ENTITY_DEFINE(world, id)

// Forward-declare all components and entities for this module.
ECS_COMPONENT_EXTERN_DECLARE(InputButton);
INPUT_FOR_EACH_BUTTON(INPUT_EACH_BUTTON_EXTERN_DECLARE);

// Concretely declare all components and entities for this module.
#define WORLD_IMPLEMENT_INPUT() \
  ECS_COMPONENT_DECLARE(InputButton); \
  INPUT_FOR_EACH_BUTTON(INPUT_EACH_BUTTON_DECLARE); \

// Setup all components and entities for this module in the given world.
#define WORLD_IMPORT_INPUT(world) \
  ECS_COMPONENT_DEFINE(world, InputButton); \
  INPUT_FOR_EACH_BUTTON(INPUT_EACH_BUTTON_DEFINE); \

// Setup all systems for this module in the correct order of operations.
void world_setup_sys_input(World* world);

// Set up all entities for this module.
void world_setup_ent_input(World* world);

// Handle button press and release events from outside the world.
void world_input_button_press(World* world, SDL_Scancode code);
void world_input_button_release(World* world, SDL_Scancode code);

#endif
