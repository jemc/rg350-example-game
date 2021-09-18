#ifndef SPRITE_KIRBO_H
#define SPRITE_KIRBO_H

#include "../sprite.h"
#include "kirbo.xpm"

static SpriteSheetSpec sprite_kirbo = {
  .name = "kirbo",
  .data = sprite_kirbo_data,
  .column_count = 1,
  .each_width = 32,
  .each_height = 32
};

#endif
