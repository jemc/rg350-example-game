/**
 * This source code was taken from the following URLs:
 * https://raw.githubusercontent.com/flecs-hub/flecs-systems-physics/04f815683ec1eb9ccf12602579399acd7c600a30/include/flecs-systems-physics/spatial_query.h
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

#ifndef FLECS_SYSTEMS_PHYSICS_SQUERY_H
#define FLECS_SYSTEMS_PHYSICS_SQUERY_H

// EDIT: Manually inlined from "bake_config.h"
#include <flecs.h>
#include <cglm/cglm.h>
#include <flecs_components_transform.h>
#include <flecs_components_geometry.h>
#include <flecs_components_physics.h>
#define FLECS_SYSTEMS_PHYSICS_API

// EDIT: Renamed co-located header file.
#include <flecs_systems_physics_octree.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ecs_squery_t ecs_squery_t;

FLECS_SYSTEMS_PHYSICS_API
ecs_squery_t* ecs_squery_new(
    ecs_world_t *world,
    const char *expr,
    vec3 center,
    float size);

FLECS_SYSTEMS_PHYSICS_API
void ecs_squery_free(
    ecs_squery_t *sq);

FLECS_SYSTEMS_PHYSICS_API
void ecs_squery_update(
    ecs_world_t *world, // EDIT: added world parameter
    ecs_squery_t *sq);

FLECS_SYSTEMS_PHYSICS_API
void ecs_squery_findn(
    const ecs_squery_t *sq,
    vec3 position,
    float range,
    ecs_vector_t **result);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

namespace flecs {

class squery {
public:
    using entity = ecs_oct_entity_t;

    squery() {
        sq_ = nullptr;
    }

    squery(flecs::world& world, const char *expr, vec3 center, float size) {
        sq_ = ecs_squery_new(world.c_ptr(), expr, center, size);
    }

    void update() {
        ecs_squery_update(sq_);
    }

    void findn(vec3 pos, float range, flecs::vector<squery::entity>& results) const {
        ecs_vector_t *v = results.ptr();
        ecs_squery_findn(sq_, pos, range, &v);
        results.ptr(v);
    }

private:
    ecs_squery_t *sq_;
};

}

#endif

#endif
