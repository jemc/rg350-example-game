#include "../room.h"

#include "../image.h"
#include "../phys.h"

#include "room4a.h"

void world_setup_ent_room_4a(World* world) {
  ECS_PREFAB(world, Room4ATileSet, (ChildOf, Room));
  ecs_set(world, Room4ATileSet, ImageSource, {room_room4a_tileset_data});

  ECS_ENTITY(world, Room4AVisualLayerParallax, (ChildOf, Room));
  ecs_add_pair(world, Room4AVisualLayerParallax, EcsIsA, Room4ATileSet);
  ecs_add(world, Room4AVisualLayerParallax, RoomVisualLayerNeedsRender);
  ecs_set(world, Room4AVisualLayerParallax, RoomVisualLayer, {
    .tiles = {
      room_room4a_layer_parallax.tiles,
      NULL
    },
    .width = room_room4a_layer_parallax.width,
    .height = room_room4a_layer_parallax.height,
    .parallax_factor = 0.1
  });

  ECS_ENTITY(world, Room4AVisualLayerMain, (ChildOf, Room));
  ecs_add_pair(world, Room4AVisualLayerMain, EcsIsA, Room4ATileSet);
  ecs_add(world, Room4AVisualLayerMain, RoomVisualLayerNeedsRender);
  ecs_set(world, Room4AVisualLayerMain, RoomVisualLayer, {
    .tiles = {
      room_room4a_layer_backdrop.tiles,
      room_room4a_layer_decor.tiles,
      room_room4a_layer_solids.tiles,
      NULL
    },
    .width = room_room4a_layer_solids.width,
    .height = room_room4a_layer_solids.height,
  });

  ECS_ENTITY(world, Room4ASolids, (ChildOf, Room));
  ecs_set_ptr(world, Room4ASolids, RoomLayer, &room_room4a_layer_solids);
  ecs_add(world, Room4ASolids, RoomLayerIsSolid);

  ECS_ENTITY(world, Room4ADoorL, (ChildOf, Room), (IsA, InteractDoor));
  ecs_set(world, Room4ADoorL, PhysTilePosition, {34, 9});
  ecs_set(world, Room4ADoorL, PhysTargetTilePosition, {41, 78});
  ecs_set(world, Room4ADoorL, RoomTarget, {RoomIdRoom3});

  ECS_ENTITY(world, Room4ADoorR, (ChildOf, Room), (IsA, InteractDoor));
  ecs_set(world, Room4ADoorR, PhysTilePosition, {35, 9});
  ecs_set(world, Room4ADoorR, PhysTargetTilePosition, {42, 78});
  ecs_set(world, Room4ADoorR, RoomTarget, {RoomIdRoom3});

  ECS_ENTITY(world, Room4APowerBall, (ChildOf, Room), (IsA, InteractPowerBall));
  ecs_set(world, Room4APowerBall, PhysTilePosition, {47, 8});
}
