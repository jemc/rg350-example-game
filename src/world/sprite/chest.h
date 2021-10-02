#ifndef SPRITE_CHEST_H
#define SPRITE_CHEST_H

#include "../sprite.h"
#include "chest.xpm"

DEF_SPRITE(chest, 16, 16);

DEF_SPRITE_RECT(chest, 16, 16, default, 0, 0);
DEF_SPRITE_RECT(chest, 16, 16, opened, 0, 1);

#endif
