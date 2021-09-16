#ifndef AVATAR_H
#define AVATAR_H

#include "SDL2/SDL.h"

typedef struct Avatar {
	int offset_x;
	int offset_y;
	int size_x;
	int size_y;
	unsigned char hue;
	unsigned char saturation;
	unsigned char value;
} Avatar;

// Convert avatar placement values into a rectangle for SDL rendering.
SDL_Rect avatar_rect(Avatar* avatar);

// Convert avatar HSV-space color values to an RGBA color for SDL rendering.
SDL_Color avatar_color(Avatar* avatar);

// Coerce the placement properties of the avatar to ensure they are on screen.
void avatar_normalize_placement(Avatar* avatar);

// Render the avatar into SDL.
void avatar_render(SDL_Renderer* renderer, Avatar* avatar);

#endif
