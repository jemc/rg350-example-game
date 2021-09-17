#include <stdbool.h>

#include "SDL2/SDL.h"

#include "video.h"
#include "world.h"
#include "world/input.h"

int main(int argc, char** argv) {
	Video video;
	if (!video_init(&video, "Example Game")) {
		video_destroy(&video);
		return 2;
	}

	World* world = world_init(&video, argc, argv);

	while (!world_has_requested_quit())
	{
		SDL_Event event;
		while (SDL_PollEvent(&event) != 0)
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
					if (event.key.repeat == 0)
						world_input_button_press(world, event.key.keysym.scancode);
					break;
				case SDL_KEYUP:
					world_input_button_release(world, event.key.keysym.scancode);
					break;
				case SDL_QUIT:
					world_request_quit();
					break;
			}
		}

		world_progress(world);
	}

	world_destroy(world);
	video_destroy(&video);
	return 0;
}
