/**
 * This source code was taken from the following URL:
 * https://raw.githubusercontent.com/flecs-hub/flecs-systems-transform/7fb27afef1e44e1f9bf95637a0de96442e572603/src/main.c
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

#include <flecs_systems_transform.h>

void EcsAddTransform3(ecs_iter_t *it) {
    ecs_world_t *world = it->world;
    ecs_entity_t comp = it->ids[1]; // EDIT: replaced OLD column entity API

    int i;
    for (i = 0; i < it->count; i ++) {
        // EDIT: Replaced old API with `ecs_get_mut_id` API.
        ecs_get_mut_id(world, it->entities[i], comp, NULL);
    }
}

void EcsApplyTransform3(ecs_iter_t *it) {
    // EDIT: Replaced old API with `ecs_term` API.
    EcsTransform3 *m = ecs_term(it, EcsTransform3, 1);
    EcsTransform3 *m_parent = ecs_term(it, EcsTransform3, 2);
    EcsPosition3 *p = ecs_term(it, EcsPosition3, 3);
    EcsRotation3 *r = ecs_term(it, EcsRotation3, 4);
    EcsScale3 *s = ecs_term(it, EcsScale3, 5);
    int i;

    if (!m_parent) {
        // EDIT: Replaced old API with `ecs_term_is_owned` API.
        if (ecs_term_is_owned(it, 3)) {
            for (i = 0; i < it->count; i ++) {
                glm_translate_make(m[i].value, *(vec3*)&p[i]);
            }
        } else {
            for (i = 0; i < it->count; i ++) {
                glm_translate_make(m[i].value, *(vec3*)p);
            }
        }
    } else {
        // EDIT: Replaced old API with `ecs_term_is_owned` API.
        if (ecs_term_is_owned(it, 3)) {
            for (i = 0; i < it->count; i ++) {
                glm_translate_to(m_parent[0].value, *(vec3*)&p[i], m[i].value);
            }
        } else {
            for (i = 0; i < it->count; i ++) {
                glm_translate_to(m_parent[0].value, *(vec3*)p, m[i].value);
            }
        }
    }

    if (r) {
        for (i = 0; i < it->count; i ++) {
            glm_rotate(m[i].value, r[i].x, (vec3){1.0, 0.0, 0.0});
            glm_rotate(m[i].value, r[i].y, (vec3){0.0, 1.0, 0.0});
            glm_rotate(m[i].value, r[i].z, (vec3){0.0, 0.0, 1.0});
        }
    }

    if (s) {
        for (i = 0; i < it->count; i ++) {
            glm_scale(m[i].value, *(vec3*)&s[i]);
        }
    }
}

void FlecsSystemsTransformImport(
    ecs_world_t *world)
{
    ECS_MODULE(world, FlecsSystemsTransform);

    ECS_IMPORT(world, FlecsComponentsTransform);

    /* System that adds transform matrix to every entity with transformations */
    // EDIT: Updated query syntax.
    ECS_SYSTEM(world, EcsAddTransform3, EcsPostLoad,
        [out] !flecs.components.transform.Transform3,
        flecs.components.transform.Position3 ||
        flecs.components.transform.Rotation3 ||
        flecs.components.transform.Scale3);
        // EDIT: Removed `Hidden` term


    // EDIT: Updated query syntax.
    ECS_SYSTEM(world, EcsApplyTransform3, EcsOnValidate,
        flecs.components.transform.Transform3,
        flecs.components.transform.Transform3(cascade|super(ChildOf)),
        flecs.components.transform.Position3,
        ?flecs.components.transform.Rotation3,
        ?flecs.components.transform.Scale3);
        // EDIT: Removed `Hidden` term
}
