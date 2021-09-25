#include "collide.h"
WORLD_IMPLEMENT_COLLIDE();

#include <flecs_components_transform.h>
#include <flecs_components_physics.h>
#include <flecs_components_geometry.h>
#include "room.h"

// TODO: Convert this into an ECS singleton?
typedef struct collide_ctx_t {
  ecs_query_t* query_room_solids;
} collide_ctx_t;
static collide_ctx_t collide_ctx = {};
void collide_ctx_init(World* world) {
  collide_ctx.query_room_solids =
    ecs_query_new(world, "RoomLayer, RoomLayerIsSolid");
}

// TODO: Revisit this logic and try to improve it.
WORLD_DEF_SYS(collide_actor_to_tile, EcsPosition2, EcsVelocity2, EcsSquare) {
  EcsPosition2* p = ecs_term(it, EcsPosition2, 1);
  EcsVelocity2* v = ecs_term(it, EcsVelocity2, 2);
  EcsSquare* square = ecs_term(it, EcsSquare, 3);

  for (int i = 0; i < it->count; i++) {
    float size = square[i].size;

    ecs_iter_t it_room_solids = ecs_query_iter(it->world, collide_ctx.query_room_solids);
    while (ecs_query_next(&it_room_solids)) {
      RoomLayer *room_layer = ecs_term(&it_room_solids, RoomLayer, 1);

      for (int ri = 0; ri < it_room_solids.count; ri++) {
        const uint8_t* tiles = room_layer[ri].tiles;

        // TODO: These can be calculated just by knowing the _room_ width,
        // not necessarily needing to correspond to a room _layer_.
        // And the player should have the LocatedIn relationship to a room.
        int tile_upper_left = (
          floor(p[i].x / ROOM_TILE_SIZE) +
          floor(p[i].y / ROOM_TILE_SIZE) * room_layer[ri].width
        );
        int tile_upper_right = (
          floor((p[i].x + size) / ROOM_TILE_SIZE) +
          floor(p[i].y / ROOM_TILE_SIZE) * room_layer[ri].width
        );
        int tile_lower_right = (
          floor((p[i].x + size) / ROOM_TILE_SIZE) +
          floor((p[i].y + size) / ROOM_TILE_SIZE) * room_layer[ri].width
        );
        int tile_lower_left = (
          floor(p[i].x / ROOM_TILE_SIZE) +
          floor((p[i].y + size) / ROOM_TILE_SIZE) * room_layer[ri].width
        );

        if (tiles[tile_upper_left] != (uint8_t)-1) {
          int ingress_x = ROOM_TILE_SIZE - floor((int)p[i].x % ROOM_TILE_SIZE);
          int ingress_y = ROOM_TILE_SIZE - floor((int)p[i].y % ROOM_TILE_SIZE);
          if (ingress_y > ingress_x) {
            p[i].x += ingress_x;
            v[i].x = 0;
          } else {
            p[i].y += ingress_y;
            v[i].y = 0;
          }
        }
        if (tiles[tile_upper_right] != (uint8_t)-1
        && tile_upper_right != tile_upper_left) {
          int ingress_x = floor((int)p[i].x % ROOM_TILE_SIZE);
          int ingress_y = ROOM_TILE_SIZE - floor((int)p[i].y % ROOM_TILE_SIZE);
          if (ingress_y > ingress_x) {
            p[i].x -= ingress_x;
            v[i].x = 0;
          } else {
            p[i].y += ingress_y;
            v[i].y = 0;
          }
        }
        if (tiles[tile_lower_right] != (uint8_t)-1
        && tile_lower_right != tile_upper_left
        && tile_lower_right != tile_upper_right) {
          int ingress_x = floor((int)p[i].x % ROOM_TILE_SIZE);
          int ingress_y = floor((int)p[i].y % ROOM_TILE_SIZE);
          if (ingress_y > ingress_x) {
            p[i].x -= ingress_x;
            v[i].x = 0;
          } else {
            p[i].y -= ingress_y;
            v[i].y = 0;
          }
        }
        if (tiles[tile_lower_left] != (uint8_t)-1
        && tile_lower_left != tile_upper_left
        && tile_lower_left != tile_upper_right
        && tile_lower_left != tile_lower_right) {
          int ingress_x = ROOM_TILE_SIZE - floor((int)p[i].x % ROOM_TILE_SIZE);
          int ingress_y = floor((int)p[i].y % ROOM_TILE_SIZE);
          if (ingress_y > ingress_x) {
            p[i].x += ingress_x;
            v[i].x = 0;
          } else {
            p[i].y -= ingress_y;
            v[i].y = 0;
          }
        }
      }
    }
  }
}

// Set up all these systems in the correct order of operations.
void world_setup_sys_collide(World* world) {
  collide_ctx_init(world);
  WORLD_SETUP_SYS(world, collide_actor_to_tile, EcsOnUpdate);
}

// Set up all entities for this module.
void world_setup_ent_collide(World* world) {
  // Empty
}
