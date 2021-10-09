#include "../room.h"

#include "../image.h"
#include "../phys.h"

#include "room4b.h"

#include "../sprite/proj_moon.h"

void world_setup_ent_room_4b(World* world) {
  ECS_PREFAB(world, Room4BTileSet, (ChildOf, Room));
  ecs_set(world, Room4BTileSet, ImageSource, {room_room4b_tileset_data});

  ECS_ENTITY(world, Room4BVisualLayerParallax, (ChildOf, Room));
  ecs_add_pair(world, Room4BVisualLayerParallax, EcsIsA, Room4BTileSet);
  ecs_add(world, Room4BVisualLayerParallax, RoomVisualLayerNeedsRender);
  ecs_set(world, Room4BVisualLayerParallax, RoomVisualLayer, {
    .tiles = {
      room_room4b_layer_parallax.tiles,
      NULL
    },
    .width = room_room4b_layer_parallax.width,
    .height = room_room4b_layer_parallax.height,
    .parallax_factor = 0.1
  });

  ECS_ENTITY(world, Room4BVisualLayerMain, (ChildOf, Room));
  ecs_add_pair(world, Room4BVisualLayerMain, EcsIsA, Room4BTileSet);
  ecs_add(world, Room4BVisualLayerMain, RoomVisualLayerNeedsRender);
  ecs_set(world, Room4BVisualLayerMain, RoomVisualLayer, {
    .tiles = {
      room_room4b_layer_backdrop.tiles,
      room_room4b_layer_decor.tiles,
      room_room4b_layer_solids.tiles,
      NULL
    },
    .width = room_room4b_layer_solids.width,
    .height = room_room4b_layer_solids.height,
  });

  ECS_ENTITY(world, Room4BSolids, (ChildOf, Room));
  ecs_set_ptr(world, Room4BSolids, RoomLayer, &room_room4b_layer_solids);
  ecs_add(world, Room4BSolids, RoomLayerIsSolid);

  ECS_ENTITY(world, Room4BDoorL, (ChildOf, Room), (IsA, InteractDoor));
  ecs_set(world, Room4BDoorL, PhysTilePosition, {17, 9});
  ecs_set(world, Room4BDoorL, PhysTargetTilePosition, {79, 78});
  ecs_set(world, Room4BDoorL, RoomTarget, {RoomIdRoom3});

  ECS_ENTITY(world, Room4BDoorR, (ChildOf, Room), (IsA, InteractDoor));
  ecs_set(world, Room4BDoorR, PhysTilePosition, {18, 9});
  ecs_set(world, Room4BDoorR, PhysTargetTilePosition, {80, 78});
  ecs_set(world, Room4BDoorR, RoomTarget, {RoomIdRoom3});

  ECS_ENTITY(world, Room4BPowerBall, (ChildOf, Room), (IsA, InteractPowerBall));
  ecs_set(world, Room4BPowerBall, PhysTilePosition, {5, 8});
  ecs_set(world, Room4BPowerBall, ImageSource, {sprite_proj_moon.data});
  ecs_set_ptr(world, Room4BPowerBall, SpriteSheet, &sprite_proj_moon);
}
