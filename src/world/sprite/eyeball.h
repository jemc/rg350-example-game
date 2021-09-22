#ifndef SPRITE_EYEBALL_H
#define SPRITE_EYEBALL_H

#include "../sprite.h"
#include "eyeball.xpm"

DEF_SPRITE(eyeball, 16, 16);

DEF_SPRITE_RECT(eyeball, default,   0, 0);
DEF_SPRITE_RECT(eyeball, step_l1,   0, 1);
DEF_SPRITE_RECT(eyeball, step_l2,   0, 2);
// TODO: either get rid of or use the (1, 0) frame
DEF_SPRITE_RECT(eyeball, step_r1,   1, 1);
DEF_SPRITE_RECT(eyeball, step_r2,   1, 2);

DEF_SPRITE_RECT(eyeball, jump_up,   2, 0);
DEF_SPRITE_RECT(eyeball, jump_mid,  2, 1);
DEF_SPRITE_RECT(eyeball, jump_down, 2, 2);

DEF_SPRITE_RECT(eyeball, partial_frontal_tall,  0, 7);
DEF_SPRITE_RECT(eyeball, partial_frontal_short, 0, 8);
DEF_SPRITE_RECT(eyeball, frontal_tall,          1, 7);
DEF_SPRITE_RECT(eyeball, frontal_short,         1, 8);
DEF_SPRITE_RECT(eyeball, frontal_droop_tall,    2, 7);
DEF_SPRITE_RECT(eyeball, frontal_droop_short,   2, 8);
DEF_SPRITE_RECT(eyeball, frontal_closed_tall,   3, 7);
DEF_SPRITE_RECT(eyeball, frontal_closed_short,  3, 8);

#endif
