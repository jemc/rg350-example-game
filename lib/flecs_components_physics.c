/**
 * This source code was taken from the following URL:
 * https://raw.githubusercontent.com/flecs-hub/flecs-components-physics/6ff26878db489c8203a32254f712e06c6c0838f0/src/physics.c
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

#include <flecs_components_physics.h>

#include <string.h>

void FlecsComponentsPhysicsImport(
    ecs_world_t *world)
{
    ECS_MODULE(world, FlecsComponentsPhysics);

    ECS_IMPORT(world, FlecsMeta);
    ECS_IMPORT(world, FlecsComponentsGraphics);

    ecs_set_name_prefix(world, "Ecs");

    ECS_TAG(world, EcsCollider);
    ECS_TAG(world, EcsRigidBody);
    ECS_META(world, EcsVelocity2);
    ECS_META(world, EcsVelocity3);
    ECS_META(world, EcsAngularSpeed);
    ECS_META(world, EcsAngularVelocity);
    ECS_META(world, EcsBounciness);
    ECS_META(world, EcsFriction);

    ECS_EXPORT_ENTITY(EcsCollider);
    ECS_EXPORT_ENTITY(EcsRigidBody);
    ECS_EXPORT_COMPONENT(EcsVelocity2);
    ECS_EXPORT_COMPONENT(EcsVelocity3);
    ECS_EXPORT_COMPONENT(EcsAngularSpeed);
    ECS_EXPORT_COMPONENT(EcsAngularVelocity);
    ECS_EXPORT_COMPONENT(EcsBounciness);
    ECS_EXPORT_COMPONENT(EcsFriction);
}
