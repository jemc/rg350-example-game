#include "../room.h"

#include "../image.h"
#include "../phys.h"

#include "room2.h"

void world_setup_ent_room_2(World* world) {
  ECS_PREFAB(world, Room2TileSet, (ChildOf, Room));
  ecs_set(world, Room2TileSet, ImageSource, {room_room2_tileset_data});

  ECS_ENTITY(world, Room2VisualLayerParallax, (ChildOf, Room));
  ecs_add_pair(world, Room2VisualLayerParallax, EcsIsA, Room2TileSet);
  ecs_add(world, Room2VisualLayerParallax, RoomVisualLayerNeedsRender);
  ecs_set(world, Room2VisualLayerParallax, RoomVisualLayer, {
    .tiles = {
      room_room2_layer_parallax.tiles,
      NULL
    },
    .width = room_room2_layer_parallax.width,
    .height = room_room2_layer_parallax.height,
    .parallax_factor = 0.1
  });

  ECS_ENTITY(world, Room2VisualLayerMain, (ChildOf, Room));
  ecs_add_pair(world, Room2VisualLayerMain, EcsIsA, Room2TileSet);
  ecs_add(world, Room2VisualLayerMain, RoomVisualLayerNeedsRender);
  ecs_set(world, Room2VisualLayerMain, RoomVisualLayer, {
    .tiles = {
      room_room2_layer_backdrop.tiles,
      room_room2_layer_decor.tiles,
      room_room2_layer_solids.tiles,
      NULL
    },
    .width = room_room2_layer_solids.width,
    .height = room_room2_layer_solids.height,
  });

  ECS_ENTITY(world, Room2Solids, (ChildOf, Room));
  ecs_set_ptr(world, Room2Solids, RoomLayer, &room_room2_layer_solids);
  ecs_add(world, Room2Solids, RoomLayerIsSolid);

  ECS_ENTITY(world, Room2DoorTmp, (ChildOf, Room), (IsA, InteractDoor));
  ecs_set(world, Room2DoorTmp, PhysTilePosition, {4, 11});
  ecs_set(world, Room2DoorTmp, PhysTargetTilePosition, {100, 24});
  ecs_set(world, Room2DoorTmp, RoomTarget, {RoomIdRoom1});

  ECS_ENTITY(world, Room2DoorToTmp, (ChildOf, Room), (IsA, InteractDoor));
  ecs_set(world, Room2DoorToTmp, PhysTilePosition, {20, 5});
  ecs_set(world, Room2DoorToTmp, PhysTargetTilePosition, {4, 11});

  ECS_ENTITY(world, Room2SavePointTmp, (ChildOf, Room), (IsA, InteractSavePoint));
  ecs_set(world, Room2SavePointTmp, PhysTilePosition, {12, 8});
}
