/**
 * This source code was taken from the following URL:
 * https://raw.githubusercontent.com/flecs-hub/flecs-systems-transform/7fb27afef1e44e1f9bf95637a0de96442e572603/include/flecs_systems_transform.h
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

#ifndef FLECS_SYSTEMS_TRANSFORM_H
#define FLECS_SYSTEMS_TRANSFORM_H

// EDIT: Manually inlined from <flecs-systems-transform/bake_config.h>
#include <flecs.h>
#include <cglm/cglm.h>
#include <flecs_components_transform.h>
#define FLECS_SYSTEMS_TRANSFORM_API

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FlecsSystemsTransform {
   int dummy;
} FlecsSystemsTransform;

FLECS_SYSTEMS_TRANSFORM_API
void FlecsSystemsTransformImport(
    ecs_world_t *world);

#define FlecsSystemsTransformImportHandles(handles)

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#ifndef FLECS_NO_CPP

namespace flecs {
namespace systems {

class transform : FlecsSystemsTransform {
public:
    transform(flecs::world& ecs) {
        FlecsSystemsTransformImport(ecs.c_ptr());

        ecs.module<flecs::systems::transform>();
    }
};

}
}

#endif
#endif

#endif
