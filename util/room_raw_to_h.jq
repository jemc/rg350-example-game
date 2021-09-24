"
#ifndef ROOM_\($ENV.ROOM_ID | ascii_upcase)_H
#define ROOM_\($ENV.ROOM_ID | ascii_upcase)_H
#include \"../room.h\"
#include \"\($ENV.ROOM_ID).xpm\"
" as $preamble

| "\n\n#endif" as $postamble

| .map_width as $w
| .map_height as $h
| .layers
| map({name: .name, tiles: [
  range(0; $h) as $y
  | range(0; $w) as $x
  | (
    .positions[]
    | select(.x == $x)
    | select(.y == $y)
    | .id
  )
  // -1
]})
| map("DEF_ROOM_LAYER(\($ENV.ROOM_ID), \(.name), \($w), \($h), {\(.tiles | @csv)});")
| reduce .[] as $line ($preamble; "\(.)\n\($line)")
+ $postamble