#include "video.h"

#define CHECK_OK(ACTION) \
  if (!ok) { \
		printf("%s failed: %s\n", ACTION, SDL_GetError()); \
		return false; \
	}

bool video_init(Video* video, const char* window_name) {
  bool ok;

  // Turn on SDL2.
  ok = 0 <= SDL_Init(SDL_INIT_VIDEO);
	CHECK_OK("SDL_Init");

  // Hide the cursor.
	ok = 0 <= SDL_ShowCursor(SDL_DISABLE);
	CHECK_OK("SDL_ShowCursor");

  // Create the window.
	video->window = SDL_CreateWindow(window_name,
#ifdef SDL_WINDOWED
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		VIDEO_WIDTH * VIDEO_SCALE, VIDEO_HEIGHT * VIDEO_SCALE,
		0
#else
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		0, 0,
		SDL_WINDOW_FULLSCREEN_DESKTOP
#endif
	);
  ok = video->window != NULL;
  CHECK_OK("SDL_CreateWindow");

  // Create the renderer.
	video->renderer = SDL_CreateRenderer(video->window, -1, SDL_RENDERER_PRESENTVSYNC);
  ok = video->renderer != NULL;
  CHECK_OK("SDL_CreateRenderer");

  // Set the video rendering scale.
	ok = 0 <= SDL_RenderSetScale(video->renderer, VIDEO_SCALE, VIDEO_SCALE);
  CHECK_OK("SDL_RenderSetScale");

  return true;
}

void video_destroy(Video* video) {
  if (video->renderer) SDL_DestroyRenderer(video->renderer);
  if (video->window) SDL_DestroyWindow(video->window);
  SDL_Quit();
}
