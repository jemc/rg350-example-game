/**
 * This source code was taken from the following URLs:
 * https://raw.githubusercontent.com/flecs-hub/flecs-systems-physics/04f815683ec1eb9ccf12602579399acd7c600a30/include/flecs_systems_physics.h
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

#ifndef FLECS_SYSTEMS_PHYSICS_H
#define FLECS_SYSTEMS_PHYSICS_H

// EDIT: Manually inlined from <flecs-systems-physics/bake_config.h>
#include <flecs.h>
#include <cglm/cglm.h>
#include <flecs_components_transform.h>
#include <flecs_components_geometry.h>
#include <flecs_components_physics.h>
#define FLECS_SYSTEMS_PHYSICS_API

// EDIT: Renamed co-located header files.
#include <flecs_systems_physics_octree.h>
#include <flecs_systems_physics_spatial_query.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __cplusplus

typedef struct EcsSpatialQuery {
    ecs_squery_t *query;
} EcsSpatialQuery;

typedef struct EcsSpatialQueryResult {
    ecs_vector_t *results;
} EcsSpatialQueryResult;

#else

typedef struct EcsSpatialQuery {
    flecs::squery query;
} EcsSpatialQuery;

typedef struct EcsSpatialQueryResult {
    flecs::vector<flecs::squery::entity> results;
} EcsSpatialQueryResult;

#endif

typedef struct FlecsSystemsPhysics {
    ECS_DECLARE_COMPONENT(EcsSpatialQuery);
} FlecsSystemsPhysics;

FLECS_SYSTEMS_PHYSICS_API
void FlecsSystemsPhysicsImport(
    ecs_world_t *world);

#define FlecsSystemsPhysicsImportHandles(handles)\
    ECS_IMPORT_COMPONENT(handles, EcsSpatialQuery) // EDIT: added handles arg

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

namespace flecs {
namespace systems {

class physics : FlecsSystemsPhysics {
public:
    using SpatialQuery = EcsSpatialQuery;
    using SpatialQueryResult = EcsSpatialQueryResult;

    physics(flecs::world& ecs) {
        FlecsSystemsPhysicsImport(ecs.c_ptr());

        ecs.module<flecs::systems::physics>();

        ecs.component<SpatialQuery>("flecs::systems::physics::SpatialQuery");
        ecs.component<SpatialQueryResult>("flecs::systems::physics::SpatialQueryResult");
    }
};

}
}

#endif

#endif
