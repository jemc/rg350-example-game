#ifndef VIDEO_H
#define VIDEO_H

#include <stdbool.h>

#include "SDL2/SDL.h"

// This is the screen size for the RG350 device (when at 2x scale).
// TODO: Define a different screen size scale default for desktop devices.
#define VIDEO_WIDTH 320
#define VIDEO_HEIGHT 240

#ifndef VIDEO_SCALE
	#define VIDEO_SCALE 2
#endif

#define VIDEO_FRAME_RATE 60

typedef struct Video {
	SDL_Window* window;
	SDL_Renderer* renderer;
} Video;

// Initialize video resources for the app.
bool video_init(Video* video, const char* window_name);

// Teardown video resources.
void video_destroy(Video* video);

#endif
