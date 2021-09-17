/**
 * This source code was taken from the following URLs:
 * https://raw.githubusercontent.com/flecs-hub/flecs-systems-physics/04f815683ec1eb9ccf12602579399acd7c600a30/include/flecs-systems-physics/octree.h
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

#ifndef FLECS_SYSTEMS_PHYSICS_OCTREE_H
#define FLECS_SYSTEMS_PHYSICS_OCTREE_H

// EDIT: Manually inlined from "bake_config.h"
#include <flecs.h>
#include <cglm/cglm.h>
#include <flecs_components_transform.h>
#include <flecs_components_geometry.h>
#include <flecs_components_physics.h>
#define FLECS_SYSTEMS_PHYSICS_API

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ecs_octree_t ecs_octree_t;

typedef struct ecs_oct_entity_t {
    ecs_entity_t e;
    vec3 pos;
    vec3 size;
} ecs_oct_entity_t;

FLECS_SYSTEMS_PHYSICS_API
ecs_octree_t* ecs_octree_new(
    vec3 center,
    float size);

FLECS_SYSTEMS_PHYSICS_API
void ecs_octree_free(
    ecs_octree_t *ot);

FLECS_SYSTEMS_PHYSICS_API
void ecs_octree_clear(
    ecs_octree_t *ot);

FLECS_SYSTEMS_PHYSICS_API
int32_t ecs_octree_insert(
    ecs_octree_t *ot,
    ecs_entity_t e,
    vec3 pos,
    vec3 size);

FLECS_SYSTEMS_PHYSICS_API
void ecs_octree_findn(
    ecs_octree_t *ot,
    vec3 pos,
    float range,
    ecs_vector_t **result);

FLECS_SYSTEMS_PHYSICS_API
int32_t ecs_octree_dump(
    ecs_octree_t *ot);

#ifdef __cplusplus
}
#endif

#endif
