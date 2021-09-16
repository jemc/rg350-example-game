#include "avatar.h"

#include "video.h"

SDL_Rect avatar_rect(Avatar* avatar) {
	return (SDL_Rect){
		.x = (VIDEO_WIDTH  / 2) + avatar->offset_x - avatar->size_x / 2,
		.y = (VIDEO_HEIGHT / 2) + avatar->offset_y - avatar->size_y / 2,
		.w = avatar->size_x,
		.h = avatar->size_y,
	};
}

SDL_Color avatar_color(Avatar* avatar) {
	// Convert the inputs to unsigned integer locals to prevent overflow.
	unsigned int h = avatar->hue;
	unsigned int s = avatar->saturation;
	unsigned int v = avatar->value;

	// Deal with grayscale as a simple and fast special case.
	if (avatar->saturation == 0)
		return (SDL_Color){ .r = v, .g = v, .b = v, .a = 0xFF };

	// Divide the hue spectrum (0-255) into 6 roughly equal regions (~43 each),
	// then determine the placement of the color within those regions.
	unsigned char region = h / 43;
	unsigned int remainder = (h - (region * 43)) * 6;
	unsigned char p = (v * (0xFF - s)) >> 8;
	unsigned char q = (v * (0xFF - ((s * remainder) >> 8))) >> 8;
	unsigned char t = (v * (0xFF - ((s * (0xFF - remainder)) >> 8))) >> 8;
	switch (region) {
		case 0:  return (SDL_Color){ .r = v, .g = t, .b = p, .a = 0xFF };
		case 1:  return (SDL_Color){ .r = q, .g = v, .b = p, .a = 0xFF };
		case 2:  return (SDL_Color){ .r = p, .g = v, .b = t, .a = 0xFF };
		case 3:  return (SDL_Color){ .r = p, .g = q, .b = v, .a = 0xFF };
		case 4:  return (SDL_Color){ .r = t, .g = p, .b = v, .a = 0xFF };
		default: return (SDL_Color){ .r = v, .g = p, .b = q, .a = 0xFF };
	}
}

void avatar_normalize_placement(Avatar* avatar) {
	const SDL_Rect rect = avatar_rect(avatar);

	const int margin_x1 = rect.x;
	const int margin_y1 = rect.y;
	const int margin_x2 = VIDEO_WIDTH - rect.x - rect.w;
	const int margin_y2 = VIDEO_HEIGHT - rect.y - rect.h;

	if (margin_x1 < 0) avatar->offset_x -= margin_x1;
	if (margin_y1 < 0) avatar->offset_y -= margin_y1;
	if (margin_x2 < 0) avatar->offset_x += margin_x2;
	if (margin_y2 < 0) avatar->offset_y += margin_y2;
}

void avatar_render(SDL_Renderer* renderer, Avatar* avatar) {
	SDL_Rect rect = avatar_rect(avatar);
	SDL_Color color = avatar_color(avatar);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, &rect);
}
