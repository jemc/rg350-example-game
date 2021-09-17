/**
 * This source code was taken from the following URL:
 * https://raw.githubusercontent.com/flecs-hub/flecs-components-transform/72c265550b1532a0b6313cf73a95d18bf623f488/src/transform.c
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

#include <flecs_components_transform.h>
#include <string.h>

void FlecsComponentsTransformImport(
    ecs_world_t *world)
{
    ECS_MODULE(world, FlecsComponentsTransform);

    ecs_set_name_prefix(world, "Ecs");

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, EcsPosition2);
    ECS_META(world, EcsPosition3);
    ECS_META(world, EcsScale2);
    ECS_META(world, EcsScale3);
    ECS_META(world, EcsRotation2);
    ECS_META(world, EcsRotation3);
    ECS_META(world, EcsQuaternion);

    ECS_COMPONENT(world, EcsTransform2);
    ECS_COMPONENT(world, EcsTransform3);
    ECS_COMPONENT(world, EcsProject2);
    ECS_COMPONENT(world, EcsProject3);

    ECS_EXPORT_COMPONENT(EcsPosition2);
    ECS_EXPORT_COMPONENT(EcsPosition3);
    ECS_EXPORT_COMPONENT(EcsScale2);
    ECS_EXPORT_COMPONENT(EcsScale3);
    ECS_EXPORT_COMPONENT(EcsRotation2);
    ECS_EXPORT_COMPONENT(EcsRotation3);
    ECS_EXPORT_COMPONENT(EcsQuaternion);
    ECS_EXPORT_COMPONENT(EcsTransform2);
    ECS_EXPORT_COMPONENT(EcsTransform3);
    ECS_EXPORT_COMPONENT(EcsProject2);
    ECS_EXPORT_COMPONENT(EcsProject3);
}
