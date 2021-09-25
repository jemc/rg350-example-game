#ifndef SPRITE_EYEBALL_H
#define SPRITE_EYEBALL_H

#include "../sprite.h"
#include "eyeball.xpm"

DEF_SPRITE(eyeball, 16, 16);

DEF_SPRITE_RECT(eyeball, default, 0, 0);
DEF_SPRITE_RECT(eyeball, step_l1, 0, 1);
DEF_SPRITE_RECT(eyeball, step_l2, 0, 2);
// TODO: either get rid of or use the (1, 0) frame
DEF_SPRITE_RECT(eyeball, step_r1, 1, 1);
DEF_SPRITE_RECT(eyeball, step_r2, 1, 2);

DEF_SPRITE_RECT(eyeball, look_up_default, 0, 3);
DEF_SPRITE_RECT(eyeball, look_up_step_l1, 0, 4);
DEF_SPRITE_RECT(eyeball, look_up_step_l2, 0, 5);
// TODO: either get rid of or use the (1, 3) frame
DEF_SPRITE_RECT(eyeball, look_up_step_r1, 1, 4);
DEF_SPRITE_RECT(eyeball, look_up_step_r2, 1, 5);

DEF_SPRITE_RECT(eyeball, look_down_default, 3, 3);
DEF_SPRITE_RECT(eyeball, look_down_step_l1, 3, 4);
DEF_SPRITE_RECT(eyeball, look_down_step_l2, 3, 5);
// TODO: either get rid of or use the (4, 3) frame
DEF_SPRITE_RECT(eyeball, look_down_step_r1, 4, 4);
DEF_SPRITE_RECT(eyeball, look_down_step_r2, 4, 5);

DEF_SPRITE_RECT(eyeball, jump_up,   2, 0);
DEF_SPRITE_RECT(eyeball, jump_mid,  2, 1);
DEF_SPRITE_RECT(eyeball, jump_down, 2, 2);

DEF_SPRITE_RECT(eyeball, look_up_jump_up,   2, 3);
DEF_SPRITE_RECT(eyeball, look_up_jump_mid,  2, 4);
DEF_SPRITE_RECT(eyeball, look_up_jump_down, 2, 5);

DEF_SPRITE_RECT(eyeball, look_down_jump_up,   5, 3);
DEF_SPRITE_RECT(eyeball, look_down_jump_mid,  5, 4);
DEF_SPRITE_RECT(eyeball, look_down_jump_down, 5, 5);

DEF_SPRITE_RECT(eyeball, partial_frontal_tall,  0, 7);
DEF_SPRITE_RECT(eyeball, partial_frontal_short, 0, 8);
DEF_SPRITE_RECT(eyeball, frontal_tall,          1, 7);
DEF_SPRITE_RECT(eyeball, frontal_short,         1, 8);
DEF_SPRITE_RECT(eyeball, frontal_droop_tall,    2, 7);
DEF_SPRITE_RECT(eyeball, frontal_droop_short,   2, 8);
DEF_SPRITE_RECT(eyeball, frontal_closed_tall,   3, 7);
DEF_SPRITE_RECT(eyeball, frontal_closed_short,  3, 8);

#endif
