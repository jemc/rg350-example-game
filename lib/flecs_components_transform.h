/**
 * This source code was taken from the following URL:
 * https://raw.githubusercontent.com/flecs-hub/flecs-components-transform/72c265550b1532a0b6313cf73a95d18bf623f488/include/flecs_components_transform.h
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

#ifndef FLECS_COMPONENTS_TRANSFORM_H
#define FLECS_COMPONENTS_TRANSFORM_H

// EDIT: Manually inlined from <flecs-components-transform/bake_config.h>
#include <flecs.h>
#include <flecs_meta.h>
#include <cglm/cglm.h>
#define FLECS_COMPONENTS_TRANSFORM_API

#ifndef FLECS_LEGACY

ECS_STRUCT(EcsPosition2, {
    float x;
    float y;
});

ECS_STRUCT(EcsPosition3, {
    float x;
    float y;
    float z;
});

ECS_STRUCT(EcsScale2, {
    float x;
    float y;
});

ECS_STRUCT(EcsScale3, {
    float x;
    float y;
    float z;
});

ECS_STRUCT(EcsRotation2, {
    float angle;
});

ECS_STRUCT(EcsRotation3, {
    float x;
    float y;
    float z;
});

ECS_STRUCT(EcsQuaternion, {
    float x;
    float y;
    float z;
    float w;
});

#endif

typedef struct EcsTransform2 {
    mat3 value;
} EcsTransform2;

typedef struct EcsTransform3 {
    mat4 value;
} EcsTransform3;

typedef struct EcsProject2 {
    mat3 value;
} EcsProject2;

typedef struct EcsProject3 {
    mat4 value;
} EcsProject3;

typedef struct FlecsComponentsTransform {
    ECS_DECLARE_COMPONENT(EcsPosition2);
    ECS_DECLARE_COMPONENT(EcsPosition3);
    ECS_DECLARE_COMPONENT(EcsScale2);
    ECS_DECLARE_COMPONENT(EcsScale3);
    ECS_DECLARE_COMPONENT(EcsRotation2);
    ECS_DECLARE_COMPONENT(EcsRotation3);
    ECS_DECLARE_COMPONENT(EcsQuaternion);
    ECS_DECLARE_COMPONENT(EcsTransform2);
    ECS_DECLARE_COMPONENT(EcsTransform3);
    ECS_DECLARE_COMPONENT(EcsProject2);
    ECS_DECLARE_COMPONENT(EcsProject3);
} FlecsComponentsTransform;

#ifdef __cplusplus
extern "C" {
#endif

FLECS_COMPONENTS_TRANSFORM_API
void FlecsComponentsTransformImport(
    ecs_world_t *world);

#define FlecsComponentsTransformImportHandles(handles)\
    ECS_IMPORT_COMPONENT(handles, EcsPosition2);\
    ECS_IMPORT_COMPONENT(handles, EcsPosition3);\
    ECS_IMPORT_COMPONENT(handles, EcsScale2);\
    ECS_IMPORT_COMPONENT(handles, EcsScale3);\
    ECS_IMPORT_COMPONENT(handles, EcsRotation2);\
    ECS_IMPORT_COMPONENT(handles, EcsRotation3);\
    ECS_IMPORT_COMPONENT(handles, EcsQuaternion);\
    ECS_IMPORT_COMPONENT(handles, EcsTransform2);\
    ECS_IMPORT_COMPONENT(handles, EcsTransform3);\
    ECS_IMPORT_COMPONENT(handles, EcsProject2);\
    ECS_IMPORT_COMPONENT(handles, EcsProject3);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#ifndef FLECS_NO_CPP

namespace flecs {
namespace components {

class transform : FlecsComponentsTransform {
public:
    using Position2 = EcsPosition2;

    struct Position3 : EcsPosition3 {
        Position3() { }

        Position3(float x, float y, float z) {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        operator float*() {
            return reinterpret_cast<float*>(this);
        }
    };

    using Scale2 = EcsScale2;
    using Scale3 = EcsScale3;

    using Rotation2 = EcsRotation2;
    using Rotation3 = EcsRotation3;

    using Quaternion = EcsQuaternion;

    using Transform2 = EcsTransform2;
    using Transform3 = EcsTransform3;

    using Project2 = EcsProject2;
    using Project3 = EcsProject3;

    transform(flecs::world& ecs) {
        FlecsComponentsTransformImport(ecs.c_ptr());

        ecs.module<flecs::components::transform>();

        ecs.pod_component<Position2>("flecs::components::transform::Position2");
        ecs.pod_component<Position3>("flecs::components::transform::Position3");

        ecs.pod_component<Scale2>("flecs::components::transform::Scale2");
        ecs.pod_component<Scale3>("flecs::components::transform::Scale3");

        ecs.pod_component<Rotation2>("flecs::components::transform::Rotation2");
        ecs.pod_component<Rotation3>("flecs::components::transform::Rotation3");

        ecs.pod_component<Quaternion>("flecs::components::transform::Quaternion");

        ecs.pod_component<Transform2>("flecs::components::transform::Transform2");
        ecs.pod_component<Transform3>("flecs::components::transform::Transform3");

        ecs.pod_component<Project2>("flecs::components::transform::Project2");
        ecs.pod_component<Project3>("flecs::components::transform::Project3");
    }
};

}
}

#endif
#endif

#endif
