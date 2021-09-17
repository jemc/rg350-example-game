#include "input.h"
WORLD_IMPLEMENT_INPUT()

// Quit when Start and Select buttons are being pressed simultaneously.
WORLD_DEF_SYS(input_handle_quit, InputButtonStart, InputButtonSelect) {
  world_request_quit();
}

// Set up all these systems in the correct order of operations.
void world_setup_sys_input(World* world) {
  WORLD_SETUP_SYS(world, input_handle_quit, EcsPostFrame);
}

// Handle a button press event from SDL by setting a component corresponding
// to that particular button on an entity which represents all the buttons.
#define EACH_BUTTON_PRESS_CASE(id, sdl_code) \
  case sdl_code: ecs_set(world, InputButtons, id, {0}); break;
void world_input_button_press(World* world, SDL_Scancode code) {
  switch (code) {
    INPUT_FOR_EACH_BUTTON(EACH_BUTTON_PRESS_CASE)
    default: break;
  }
}

// Handle a button release event from SDL by removing a component corresponding
// to that particular button from an entity which represents all the buttons.
#define EACH_BUTTON_RELEASE_CASE(id, sdl_code) \
  case sdl_code: ecs_remove(world, InputButtons, id); break;
void world_input_button_release(World* world, SDL_Scancode code) {
  switch (code) {
    INPUT_FOR_EACH_BUTTON(EACH_BUTTON_RELEASE_CASE)
    default: break;
  }
}
