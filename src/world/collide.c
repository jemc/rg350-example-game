#include "collide.h"
WORLD_IMPLEMENT_COLLIDE();

#include "phys.h"
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
WORLD_DEF_SYS(collide_actor_to_tile, PhysPosition, PhysVelocity, PhysBounds) {
  PhysPosition* p = ecs_term(it, PhysPosition, 1);
  PhysVelocity* v = ecs_term(it, PhysVelocity, 2);
  PhysBounds* b = ecs_term(it, PhysBounds, 3);

  for (int i = 0; i < it->count; i++) {
    // TODO: Account for case where size != ROOM_TILE_SIZE

    int offset_x = (int)p[i].x % ROOM_TILE_SIZE;
    int offset_y = (int)p[i].y % ROOM_TILE_SIZE;

    bool is_lefter = offset_x < (ROOM_TILE_SIZE / 2);
    bool is_upper = offset_y < (ROOM_TILE_SIZE / 2);

    ecs_iter_t it_room_solids = ecs_query_iter(it->world, collide_ctx.query_room_solids);
    while (ecs_query_next(&it_room_solids)) {
      RoomLayer *room_layer = ecs_term(&it_room_solids, RoomLayer, 1);

      for (int ri = 0; ri < it_room_solids.count; ri++) {
        const uint8_t* tiles = room_layer[ri].tiles;

        // TODO: These can be calculated just by knowing the _room_ width,
        // not necessarily needing to correspond to a room _layer_.
        // And the player should have the LocatedIn relationship to a room.
        bool collide_upper_left = (uint8_t)-1 != tiles[(int)(
          floor(p[i].x / ROOM_TILE_SIZE) +
          floor(p[i].y / ROOM_TILE_SIZE) * room_layer[ri].width
        )];
        bool collide_upper_right = (uint8_t)-1 != tiles[(int)(
          floor((p[i].x + b[i].w) / ROOM_TILE_SIZE) +
          floor(p[i].y / ROOM_TILE_SIZE) * room_layer[ri].width
        )];
        bool collide_lower_right = (uint8_t)-1 != tiles[(int)(
          floor((p[i].x + b[i].w) / ROOM_TILE_SIZE) +
          floor((p[i].y + b[i].h) / ROOM_TILE_SIZE) * room_layer[ri].width
        )];
        bool collide_lower_left = (uint8_t)-1 != tiles[(int)(
          floor(p[i].x / ROOM_TILE_SIZE) +
          floor((p[i].y + b[i].h) / ROOM_TILE_SIZE) * room_layer[ri].width
        )];

        // TODO: Implement for arbitrary-sized rectangles, not just
        // a square the same size as the ROOM_TILE_SIZE.

        bool collided_left = false;
        bool collided_right = false;
        bool collided_upper = false;
        bool collided_lower = false;

        if (is_lefter && is_upper) {
          if (collide_lower_left) {
            if (collide_upper_right) {
              collided_lower = true;
              collided_right = true;
            } else {
              collided_lower = true;
            }
          } else {
            if (collide_upper_right) {
              collided_right = true;
            } else {
              if (collide_lower_right) {
                if (offset_x > offset_y) {
                  collided_lower = true;
                } else {
                  collided_right = true;
                }
              }
            }
          }
        } else if (is_lefter) { // && is_lower
          if (collide_upper_left) {
            if (collide_lower_right) {
              collided_upper = true;
              collided_right = true;
            } else {
              collided_upper = true;
            }
          } else {
            if (collide_lower_right) {
              collided_right = true;
            } else {
              if (collide_upper_right) {
                if (offset_x > offset_y) {
                  collided_upper = true;
                } else {
                  collided_right = true;
                }
              }
            }
          }
        } else if (is_upper) { // && is_righter
          if (collide_lower_right) {
            if (collide_upper_left) {
              collided_lower = true;
              collided_left = true;
            } else {
              collided_lower = true;
            }
          } else {
            if (collide_upper_left) {
              collided_left = true;
            } else {
              if (collide_lower_left) {
                if (offset_x > offset_y) {
                  collided_lower = true;
                } else {
                  collided_left = true;
                }
              }
            }
          }
        } else { // is_righter && is_lower
          if (collide_lower_left) {
            if (collide_upper_right) {
              collided_left = true;
              collided_upper = true;
            } else {
              collided_left = true;
            }
          } else {
            if (collide_upper_right) {
              collided_upper = true;
            } else {
              if (collide_upper_left) {
                if (offset_x > offset_y) {
                  collided_left = true;
                } else {
                  collided_upper = true;
                }
              }
            }
          }
        }

        if (collided_right && v[i].x > 0) {
          p[i].x -= offset_x;
          v[i].x = 0;
        }
        if (collided_left && v[i].x < 0) {
          p[i].x += (ROOM_TILE_SIZE - offset_x);
          v[i].x = 0;
        }
        if (collided_lower && v[i].y > 0) {
          p[i].y -= offset_y;
          v[i].y = 0;
        }
        if (collided_upper && v[i].y < 0) {
          p[i].y += (ROOM_TILE_SIZE - offset_y);
          v[i].y = 0;
        }
      }
    }
  }
}

// Set up all these systems in the correct order of operations.
void world_setup_sys_collide(World* world) {
  collide_ctx_init(world);
  WORLD_SETUP_SYS(world, collide_actor_to_tile, EcsOnValidate);
}

// Set up all entities for this module.
void world_setup_ent_collide(World* world) {
  // Empty
}
