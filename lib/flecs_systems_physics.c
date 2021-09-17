/**
 * This source code was taken from the following URLs:
 * https://raw.githubusercontent.com/flecs-hub/flecs-systems-physics/04f815683ec1eb9ccf12602579399acd7c600a30/src/main.c
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

ECS_CTOR(EcsSpatialQuery, ptr, {
    ptr->query = NULL;
});

ECS_DTOR(EcsSpatialQuery, ptr, {
    if (ptr->query) {
        ecs_squery_free(ptr->query);
    }
});

ECS_CTOR(EcsSpatialQueryResult, ptr, {
    ptr->results = NULL;
});

ECS_DTOR(EcsSpatialQueryResult, ptr, {
    if (ptr->results) {
        ecs_vector_free(ptr->results);
    }
});

static
void EcsMove2(ecs_iter_t *it) {
    // EDIT: Replaced old API with `ecs_term` API.
    EcsPosition2 *p = ecs_term(it, EcsPosition2, 1);
    EcsVelocity2 *v = ecs_term(it, EcsVelocity2, 2);

    int i;
    for (i = 0; i < it->count; i ++) {
        p[i].x += v[i].x * it->delta_time;
        p[i].y += v[i].y * it->delta_time;
    }
}

static
void EcsMove3(ecs_iter_t *it) {
    // EDIT: Replaced old API with `ecs_term` API.
    EcsPosition3 *p = ecs_term(it, EcsPosition3, 1);
    EcsVelocity3 *v = ecs_term(it, EcsVelocity3, 2);

    int i;
    for (i = 0; i < it->count; i ++) {
        p[i].x += v[i].x * it->delta_time;
        p[i].y += v[i].y * it->delta_time;
        p[i].z += v[i].z * it->delta_time;
    }
}

static
void EcsAddBoxCollider(ecs_iter_t *it) {
    // EDIT: Replaced old API with `ecs_term` API.
    EcsBox *box = ecs_term(it, EcsBox, 2);
    ecs_entity_t C = it->ids[1]; // EDIT: replaced OLD column entity API
    ecs_entity_t B = it->ids[2]; // EDIT: replaced OLD column entity API

    int i;
    // EDIT: Replaced old API with `ecs_term_is_owned` API.
    if (ecs_term_is_owned(it, 2)) {
        for (i = 0; i < it->count; i ++) {
            // EDIT: Replaced old API with `ecs_pair`-based API.
            ecs_entity_t pair = ecs_pair(B, C);
            EcsBox *collider = ecs_get_mut_id(
                it->world, it->entities[i], pair, NULL);
            memcpy(collider, &box[i], sizeof(EcsBox));
        }
    } else {
        for (i = 0; i < it->count; i ++) {
            // EDIT: Replaced old API with `pair`-based API.
            ecs_entity_t pair = ecs_pair(B, C);
            EcsBox *collider = ecs_get_mut_id(
                it->world, it->entities[i], pair, NULL);
            memcpy(collider, box, sizeof(EcsBox));
        }
    }
}

static
void EcsUpdateSpatialQuery(ecs_iter_t *it) {
    // EDIT: Replaced old API with `ecs_term` API.
    EcsSpatialQuery *q = ecs_term(it, EcsSpatialQuery, 1);

    int i;
    for (i = 0; i < it->count; i ++) {
        ecs_squery_update(it->world, q->query);
    }
}

void FlecsSystemsPhysicsImport(
    ecs_world_t *world)
{
    ECS_MODULE(world, FlecsSystemsPhysics);

    ECS_IMPORT(world, FlecsComponentsTransform);
    ECS_IMPORT(world, FlecsComponentsPhysics);

    ecs_set_name_prefix(world, "Ecs");

    ECS_COMPONENT(world, EcsSpatialQuery);
    ECS_COMPONENT(world, EcsSpatialQueryResult);

    ecs_set_component_actions(world, EcsSpatialQuery, {
        .ctor = ecs_ctor(EcsSpatialQuery),
        .dtor = ecs_dtor(EcsSpatialQuery)
    });

    ecs_set_component_actions(world, EcsSpatialQueryResult, {
        .ctor = ecs_ctor(EcsSpatialQueryResult),
        .dtor = ecs_dtor(EcsSpatialQueryResult)
    });

    // EDIT: Updated query syntax.
    ECS_SYSTEM(world, EcsMove2, EcsOnUpdate,
        flecs.components.transform.Position2,
        flecs.components.physics.Velocity2);
        // EDIT: Removed `Hidden` term

    // EDIT: Updated query syntax.
    ECS_SYSTEM(world, EcsMove3, EcsOnUpdate,
        flecs.components.transform.Position3,
        flecs.components.physics.Velocity3);
        // EDIT: Removed `Hidden` term

    // EDIT: Updated query syntax.
    ECS_SYSTEM(world, EcsAddBoxCollider, EcsPostLoad,
        flecs.components.geometry.Box,
        flecs.components.physics.Collider,
        !flecs.components.physics.Collider(flecs.components.geometry.Box));

    // EDIT: Updated query syntax.
    ECS_SYSTEM(world, EcsUpdateSpatialQuery, EcsPreUpdate, (SpatialQuery, *));

    ECS_EXPORT_COMPONENT(EcsSpatialQuery);
}
