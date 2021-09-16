#include <stdbool.h>

#include "SDL2/SDL.h"

#include "video.h"
#include "button.h"
#include "avatar.h"

static Avatar player = {
	.offset_x = 0,
	.offset_y = 0,
	.size_x = 50,
	.size_y = 50,
	.hue = 0,
	.saturation = 128,
	.value = 128,
};

static void handle_key_down(SDL_Scancode code) {
	switch (code) {
		// The directional pad buttons move the player rectangle around.
		case BUTTON_DPAD_LEFT:
			player.offset_x -= 8;
			avatar_normalize_placement(&player);
			break;
		case BUTTON_DPAD_RIGHT:
			player.offset_x += 8;
			avatar_normalize_placement(&player);
			break;
		case BUTTON_DPAD_UP:
			player.offset_y -= 8;
			avatar_normalize_placement(&player);
			break;
		case BUTTON_DPAD_DOWN:
			player.offset_y += 8;
			avatar_normalize_placement(&player);
			break;

		// The primary buttons change the player rectangle's size/shape.
		case BUTTON_A:
			if (player.size_x > 4) player.size_x -= 4;
			avatar_normalize_placement(&player);
			break;
		case BUTTON_B:
			if (player.size_y > 4) player.size_y -= 4;
			avatar_normalize_placement(&player);
			break;
		case BUTTON_X:
			if ((player.size_x + 4) <= 100) player.size_x += 4;
			avatar_normalize_placement(&player);
			break;
		case BUTTON_Y:
			if ((player.size_y + 4) <= 100) player.size_y += 4;
			avatar_normalize_placement(&player);
			break;

		// The left/right shoulder buttons and volume buttons change the color.
		case BUTTON_L1:
			player.hue -= 8;
			break;
		case BUTTON_R1:
			player.hue += 8;
			break;
		case BUTTON_L2:
			if (player.saturation > 8) player.saturation -= 8;
			break;
		case BUTTON_R2:
			if (player.saturation <= (0xFF - 8)) player.saturation += 8;
			break;
		case BUTTON_VOLUME_DOWN:
			if (player.value > 8) player.value -= 8;
			break;
		case BUTTON_VOLUME_UP:
			if (player.value <= (0xFF - 8)) player.value += 8;
			break;

		// Other buttons do nothing.
		default:
			break;
	}
}

static void render(SDL_Renderer* renderer) {
	const SDL_Rect background_rect =
		{ .x = 0, .y = 0, .w = VIDEO_WIDTH, .h = VIDEO_HEIGHT };

	// Render the Background Fill.
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
	SDL_RenderFillRect(renderer, &background_rect);

	// Render the Background Border.
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderDrawRect(renderer, &background_rect);

	// Render the Avatar.
	avatar_render(renderer, &player);

	SDL_RenderPresent(renderer);

	SDL_Delay(8);
}

int main(int argc, char** argv) {
	Video video;

	if (!video_init(&video, "Example Game")) {
		video_destroy(&video);
		return 2;
	}

	bool should_exit = false;
	while (!should_exit)
	{
		SDL_Event Event;
		while (SDL_PollEvent(&Event) != 0)
		{
			switch (Event.type)
			{
				case SDL_KEYDOWN:
					handle_key_down(Event.key.keysym.scancode);
					if (BUTTON_POWER == Event.key.keysym.scancode) should_exit = true;
					break;
				case SDL_QUIT:
					should_exit = true;
					break;
			}
		}

		render(video.renderer);
	}

	video_destroy(&video);
	return 0;
}
