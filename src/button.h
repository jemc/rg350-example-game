#ifndef BUTTON_H
#define BUTTON_H

#include "SDL2/SDL.h"

typedef SDL_Scancode ButtonCode;

// This is the button mapping for the RG350 device.
// TODO: Define a different button mapping for desktop devices.
#define BUTTON_DPAD_LEFT   SDL_SCANCODE_LEFT
#define BUTTON_DPAD_RIGHT  SDL_SCANCODE_RIGHT
#define BUTTON_DPAD_UP     SDL_SCANCODE_UP
#define BUTTON_DPAD_DOWN   SDL_SCANCODE_DOWN
#define BUTTON_A           SDL_SCANCODE_LCTRL
#define BUTTON_B           SDL_SCANCODE_LALT
#define BUTTON_X           SDL_SCANCODE_LSHIFT
#define BUTTON_Y           SDL_SCANCODE_SPACE
#define BUTTON_L1          SDL_SCANCODE_TAB
#define BUTTON_R1          SDL_SCANCODE_BACKSPACE
#define BUTTON_L2          SDL_SCANCODE_PAGEUP
#define BUTTON_R2          SDL_SCANCODE_PAGEDOWN
#define BUTTON_L3          SDL_SCANCODE_KP_DIVIDE
#define BUTTON_R3          SDL_SCANCODE_KP_PERIOD
#define BUTTON_SELECT      SDL_SCANCODE_ESCAPE
#define BUTTON_START       SDL_SCANCODE_RETURN
#define BUTTON_POWER       SDL_SCANCODE_HOME
#define BUTTON_VOLUME_DOWN SDL_SCANCODE_VOLUMEDOWN
#define BUTTON_VOLUME_UP   SDL_SCANCODE_VOLUMEUP

#endif
