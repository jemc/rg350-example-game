#include "../room.h"

#include "../image.h"
#include "../phys.h"

#include "room3.h"

void world_setup_ent_room_3(World* world) {
  ECS_PREFAB(world, Room3TileSet, (ChildOf, Room));
  ecs_set(world, Room3TileSet, ImageSource, {room_room3_tileset_data});

  ECS_ENTITY(world, Room3VisualLayerParallax, (ChildOf, Room));
  ecs_add_pair(world, Room3VisualLayerParallax, EcsIsA, Room3TileSet);
  ecs_add(world, Room3VisualLayerParallax, RoomVisualLayerNeedsRender);
  ecs_set(world, Room3VisualLayerParallax, RoomVisualLayer, {
    .tiles = {
      room_room3_layer_parallax.tiles,
      NULL
    },
    .width = room_room3_layer_parallax.width,
    .height = room_room3_layer_parallax.height,
    .parallax_factor = 0.1
  });

  ECS_ENTITY(world, Room3VisualLayerMain, (ChildOf, Room));
  ecs_add_pair(world, Room3VisualLayerMain, EcsIsA, Room3TileSet);
  ecs_add(world, Room3VisualLayerMain, RoomVisualLayerNeedsRender);
  ecs_set(world, Room3VisualLayerMain, RoomVisualLayer, {
    .tiles = {
      room_room3_layer_backdrop.tiles,
      room_room3_layer_decor.tiles,
      room_room3_layer_solids.tiles,
      NULL
    },
    .width = room_room3_layer_solids.width,
    .height = room_room3_layer_solids.height,
  });

  ECS_ENTITY(world, Room3Solids, (ChildOf, Room));
  ecs_set_ptr(world, Room3Solids, RoomLayer, &room_room3_layer_solids);
  ecs_add(world, Room3Solids, RoomLayerIsSolid);

  ECS_ENTITY(world, Room3DoorTmp, (ChildOf, Room), (IsA, InteractDoor));
  ecs_set(world, Room3DoorTmp, PhysTilePosition, {5, 41});
  ecs_set(world, Room3DoorTmp, PhysTargetTilePosition, {30, 5});
  ecs_set(world, Room3DoorTmp, RoomTarget, {RoomIdRoom2});
}
