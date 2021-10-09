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
| map({name: .name, tiles: (
    .positions | sort_by([.y, .x]) as $tiles
    | [
      range(0; $tiles | length) as $i
      | ((($tiles[$i+1].y // $h) * $w) + ($tiles[$i+1].x // 0)) as $next_abs
      | (($tiles[$i].y * $w) + $tiles[$i].x) as $this_abs
      | ($next_abs - $this_abs - 1) as $next_skip_count
      | [
        $tiles[$i].id,
        (range(0; $next_skip_count) | -1)
      ]
    ]
    | flatten as $tile_ids
    | (($w * $h) - ($tile_ids | length)) as $initial_skip_count
    | [
      (range(0; $initial_skip_count) | -1),
      $tile_ids[]
    ]
  )
})
| map("DEF_ROOM_LAYER(\($ENV.ROOM_ID), \(.name), \($w), \($h), {\(.tiles | @csv)});")
| reduce .[] as $line ($preamble; "\(.)\n\($line)")
+ $postamble