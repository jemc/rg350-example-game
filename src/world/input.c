#include "input.h"
WORLD_IMPLEMENT_INPUT();

#include "player.h"

// Quit when Start and Select buttons are being pressed simultaneously.
WORLD_DEF_SYS(input_handle_quit,
  (InputButton, InputButtonStart),
  (InputButton, InputButtonSelect),
) {
  world_request_quit();
}

// Set up all these systems in the correct order of operations.
void world_setup_sys_input(World* world) {
  WORLD_SETUP_SYS(world, input_handle_quit, EcsPostFrame);
}

// Set up all entities for this module.
void world_setup_ent_input(World* world) {
  // Empty
}

// Handle a button press event from SDL by setting a component corresponding
// to that particular button on the player entity.
#define EACH_BUTTON_PRESS_CASE(id, sdl_code) \
  case sdl_code: ecs_set_pair(world, Player, InputButton, id, {frame}); break;
void world_input_button_press(World* world, SDL_Scancode code) {
  int32_t frame = world_frame_number(world);
  switch (code) {
    INPUT_FOR_EACH_BUTTON(EACH_BUTTON_PRESS_CASE)
    default: break;
  }
}

// Handle a button release event from SDL by removing a component corresponding
// to that particular button from the player entity.
#define EACH_BUTTON_RELEASE_CASE(id, sdl_code) \
  case sdl_code: \
    ecs_remove_id(world, Player, ecs_pair(ecs_id(InputButton), id)); break;
void world_input_button_release(World* world, SDL_Scancode code) {
  switch (code) {
    INPUT_FOR_EACH_BUTTON(EACH_BUTTON_RELEASE_CASE)
    default: break;
  }
}
