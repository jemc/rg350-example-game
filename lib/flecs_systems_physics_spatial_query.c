/**
 * This source code was taken from the following URLs:
 * https://raw.githubusercontent.com/flecs-hub/flecs-systems-physics/04f815683ec1eb9ccf12602579399acd7c600a30/src/spatial_query.c
 *
 * That repository has the following license:
 *
 * MIT License
 *
 * Copyright (c) 2021 Sander Mertens
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <flecs_systems_physics.h>

struct ecs_squery_t {
    ecs_query_t *q;
    ecs_octree_t *ot;
};

#define EXPR_PREFIX\
    "[in] flecs.components.transform.Position3,"\
    "[in] ANY:flecs.components.physics.Collider FOR flecs.components.geometry.Box || ANY:flecs.components.geometry.Box,"

ecs_squery_t* ecs_squery_new(
    ecs_world_t *world,
    const char *expr,
    vec3 center,
    float size)
{
    ecs_assert(world != NULL, ECS_INVALID_PARAMETER, NULL);
    ecs_assert(expr != NULL, ECS_INVALID_PARAMETER, NULL);
    ecs_assert(size > 0, ECS_INVALID_PARAMETER, NULL);

    ecs_squery_t *result = ecs_os_calloc(sizeof(ecs_squery_t));

    /* Prepend position to query so we have fast access to entity position when
     * updating the octree */
    char *full_expr = ecs_os_malloc(strlen(expr) + strlen(EXPR_PREFIX) + 1);
    sprintf(full_expr, EXPR_PREFIX "%s", expr);
    result->q = ecs_query_new(world, full_expr);
    result->ot = ecs_octree_new(center, size);

    ecs_assert(result->q != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(result->ot != NULL, ECS_INTERNAL_ERROR, NULL);

    return result;
}

void ecs_squery_free(
    ecs_squery_t *sq)
{
    ecs_query_fini(sq->q); // EDIT: updated from old `ecs_query_free` API
    ecs_octree_free(sq->ot);
    ecs_os_free(sq);
}

void ecs_squery_update(
    ecs_world_t *world, // EDIT: added world parameter
    ecs_squery_t *sq)
{
    ecs_assert(sq != NULL, ECS_INVALID_PARAMETER, NULL);
    ecs_assert(sq->q != NULL, ECS_INVALID_PARAMETER, NULL);
    ecs_assert(sq->ot != NULL, ECS_INVALID_PARAMETER, NULL);

    if (ecs_query_changed(sq->q)) {
        ecs_octree_clear(sq->ot);

        ecs_iter_t it = ecs_query_iter(world, sq->q);
        while (ecs_query_next(&it)) {
            // EDIT: Replaced old API with `ecs_term` API.
            EcsPosition3 *p = ecs_term(&it, EcsPosition3, 1);
            EcsBox *b = ecs_term(&it, EcsBox, 2);

            // EDIT: Replaced old API with `ecs_term_is_owned` API.
            if (ecs_term_is_owned(&it, 2)) {
                int i;
                for (i = 0; i < it.count; i ++) {
                    vec3 vp, vs;
                    vp[0] = p[i].x;
                    vp[1] = p[i].y;
                    vp[2] = p[i].z;

                    vs[0] = b[i].width;
                    vs[1] = b[i].height;
                    vs[2] = b[i].depth;

                    ecs_octree_insert(sq->ot, it.entities[i], vp, vs);
                }
            } else {
                int i;
                for (i = 0; i < it.count; i ++) {
                    vec3 vp, vs;
                    vp[0] = p[i].x;
                    vp[1] = p[i].y;
                    vp[2] = p[i].z;

                    vs[0] = b->width;
                    vs[1] = b->height;
                    vs[2] = b->depth;

                    ecs_octree_insert(sq->ot, it.entities[i], vp, vs);
                }
            }
        }
    }
}

void ecs_squery_findn(
    const ecs_squery_t *sq,
    vec3 position,
    float range,
    ecs_vector_t **result)
{
    ecs_assert(sq != NULL, ECS_INVALID_PARAMETER, NULL);
    ecs_assert(sq->q != NULL, ECS_INVALID_PARAMETER, NULL);
    ecs_assert(sq->ot != NULL, ECS_INVALID_PARAMETER, NULL);

    ecs_octree_findn(sq->ot, position, range, result);
}
