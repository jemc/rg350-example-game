/**
 * This source code was taken from the following URL:
 * https://raw.githubusercontent.com/flecs-hub/flecs-components-physics/6ff26878db489c8203a32254f712e06c6c0838f0/include/flecs_components_physics.h
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

#ifndef FLECS_COMPONENTS_PHYSICS_H
#define FLECS_COMPONENTS_PHYSICS_H

// EDIT: Manually inlined from <flecs-components-physics/bake_config.h>
#include <flecs.h>
#include <flecs_meta.h>
#include <flecs_components_graphics.h>
#define FLECS_COMPONENTS_PHYSICS_API

ECS_STRUCT(EcsVelocity2, {
    float x;
    float y;
});

ECS_STRUCT(EcsVelocity3, {
    float x;
    float y;
    float z;
});

ECS_STRUCT(EcsAngularSpeed, {
    float value;
});

ECS_STRUCT(EcsAngularVelocity, {
    float x;
    float y;
    float z;
});

ECS_STRUCT(EcsBounciness, {
    float value;
});

ECS_STRUCT(EcsFriction, {
    float value;
});

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FlecsComponentsPhysics {
    ECS_DECLARE_ENTITY(EcsCollider);
    ECS_DECLARE_ENTITY(EcsRigidBody);
    ECS_DECLARE_COMPONENT(EcsVelocity2);
    ECS_DECLARE_COMPONENT(EcsVelocity3);
    ECS_DECLARE_COMPONENT(EcsAngularSpeed);
    ECS_DECLARE_COMPONENT(EcsAngularVelocity);
    ECS_DECLARE_COMPONENT(EcsBounciness);
    ECS_DECLARE_COMPONENT(EcsFriction);
} FlecsComponentsPhysics;

FLECS_COMPONENTS_PHYSICS_API
void FlecsComponentsPhysicsImport(
    ecs_world_t *world);

#define FlecsComponentsPhysicsImportHandles(handles)\
    ECS_IMPORT_ENTITY(handles, EcsCollider);\
    ECS_IMPORT_ENTITY(handles, EcsRigidBody);\
    ECS_IMPORT_COMPONENT(handles, EcsVelocity2);\
    ECS_IMPORT_COMPONENT(handles, EcsVelocity3);\
    ECS_IMPORT_COMPONENT(handles, EcsAngularSpeed);\
    ECS_IMPORT_COMPONENT(handles, EcsAngularVelocity);\
    ECS_IMPORT_COMPONENT(handles, EcsBounciness);\
    ECS_IMPORT_COMPONENT(handles, EcsFriction);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

namespace flecs {
namespace components {

class physics : FlecsComponentsPhysics {
public:
    using Velocity2 = EcsVelocity2;
    using Velocity3 = EcsVelocity3;
    using AngularSpeed = EcsAngularSpeed;
    using AngularVelocity = EcsAngularVelocity;
    using Bounciness = EcsBounciness;
    using Friction = EcsFriction;

    physics(flecs::world& ecs) {
        FlecsComponentsPhysicsImport(ecs.c_ptr());

        ecs.module<flecs::components::physics>();

        ecs.pod_component<Velocity2>("flecs::components::physics::Velocity2");
        ecs.pod_component<Velocity3>("flecs::components::physics::Velocity3");
        ecs.pod_component<AngularSpeed>("flecs::components::physics::AngularSpeed");
        ecs.pod_component<AngularVelocity>("flecs::components::physics::AngularVelocity");
        ecs.pod_component<Bounciness>("flecs::components::physics::Bounciness");
        ecs.pod_component<Friction>("flecs::components::physics::Friction");
    }
};

}
}

#endif

#endif
