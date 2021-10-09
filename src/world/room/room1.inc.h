#include "../room.h"

#include "../image.h"
#include "../phys.h"

#include "room1.h"

void world_setup_ent_room_1(World* world) {
  ECS_PREFAB(world, Room1TileSet, (ChildOf, Room));
  ecs_set(world, Room1TileSet, ImageSource, {room_room1_tileset_data});

  ECS_ENTITY(world, Room1VisualLayerParallax, (ChildOf, Room));
  ecs_add_pair(world, Room1VisualLayerParallax, EcsIsA, Room1TileSet);
  ecs_add(world, Room1VisualLayerParallax, RoomVisualLayerNeedsRender);
  ecs_set(world, Room1VisualLayerParallax, RoomVisualLayer, {
    .tiles = {
      room_room1_layer_parallax.tiles,
      NULL
    },
    .width = room_room1_layer_parallax.width,
    .height = room_room1_layer_parallax.height,
    .parallax_factor = 0.1
  });

  ECS_ENTITY(world, Room1VisualLayerMain, (ChildOf, Room));
  ecs_add_pair(world, Room1VisualLayerMain, EcsIsA, Room1TileSet);
  ecs_add(world, Room1VisualLayerMain, RoomVisualLayerNeedsRender);
  ecs_set(world, Room1VisualLayerMain, RoomVisualLayer, {
    .tiles = {
      room_room1_layer_backdrop.tiles,
      room_room1_layer_decor.tiles,
      room_room1_layer_solids.tiles,
      NULL
    },
    .width = room_room1_layer_solids.width,
    .height = room_room1_layer_solids.height,
  });

  ECS_ENTITY(world, Room1Solids, (ChildOf, Room));
  ecs_set_ptr(world, Room1Solids, RoomLayer, &room_room1_layer_solids);
  ecs_add(world, Room1Solids, RoomLayerIsSolid);

  ECS_ENTITY(world, Room1DoorTmp, (ChildOf, Room), (IsA, InteractDoor));
  ecs_set(world, Room1DoorTmp, PhysTilePosition, {36, 31});
  // ecs_set(world, Room1DoorTmp, PhysTargetTilePosition, {48, 9});
  // ecs_set(world, Room1DoorTmp, RoomTarget, {RoomIdRoom4A});
  ecs_set(world, Room1DoorTmp, PhysTargetTilePosition, {17, 9});
  ecs_set(world, Room1DoorTmp, RoomTarget, {RoomIdRoom4B});

  ECS_ENTITY(world, Room1DoorToTmp, (ChildOf, Room), (IsA, InteractDoor));
  ecs_set(world, Room1DoorToTmp, PhysTilePosition, {100, 24});
  ecs_set(world, Room1DoorToTmp, PhysTargetTilePosition, {4, 11});
  ecs_set(world, Room1DoorToTmp, RoomTarget, {RoomIdRoom2});

  ECS_ENTITY(world, Room1ChestTmp, (ChildOf, Room), (IsA, InteractChest));
  ecs_set(world, Room1ChestTmp, PhysTilePosition, {50, 43});
}
