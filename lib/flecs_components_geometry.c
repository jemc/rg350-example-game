/**
 * This source code was taken from the following URL:
 * https://raw.githubusercontent.com/flecs-hub/flecs-components-geometry/6b14f5b92bcd085afbc328b75926bfa73a16e265/src/geometry.c
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

#include <flecs_components_geometry.h>
#include <string.h>

ECS_COPY(EcsMesh, dst, src, {
    if (dst->vertices) {
        ecs_os_free(dst->vertices);
    }

    if (src->vertices) {
        size_t size = sizeof(vec3) * src->vertex_count;
        dst->vertices = ecs_os_malloc(size);
        dst->vertex_count = src->vertex_count;
        memcpy(dst->vertices, src->vertices, size);
    } else {
        dst->vertices = NULL;
        dst->vertex_count = 0;
    }
});

ECS_MOVE(EcsMesh, dst, src, {
    if (dst->vertices) {
        ecs_os_free(dst->vertices);
    }

    dst->vertices = src->vertices;
    dst->vertex_count = src->vertex_count;
    src->vertices = NULL;
    src->vertex_count = 0;
});

ECS_DTOR(EcsMesh, ptr, {
    ecs_os_free(ptr->vertices);
});

void FlecsComponentsGeometryImport(
    ecs_world_t *world)
{
    ECS_MODULE(world, FlecsComponentsGeometry);

    ecs_set_name_prefix(world, "Ecs");

    ECS_IMPORT(world, FlecsMeta);
    ECS_IMPORT(world, FlecsComponentsGraphics);

    ECS_TAG(world, EcsPoint);
    ECS_COMPONENT(world, EcsLine2);
    ECS_COMPONENT(world, EcsLine3);
    ECS_COMPONENT(world, EcsRectangle);
    ECS_COMPONENT(world, EcsSquare);
    ECS_COMPONENT(world, EcsBox);
    ECS_COMPONENT(world, EcsCircle);
    ECS_COMPONENT(world, EcsMesh);
    ECS_COMPONENT(world, EcsLineColor);
    ECS_COMPONENT(world, EcsLineWidth);

    ECS_EXPORT_ENTITY(EcsPoint);
    ECS_EXPORT_COMPONENT(EcsLine2);
    ECS_EXPORT_COMPONENT(EcsLine3);
    ECS_EXPORT_COMPONENT(EcsRectangle);
    ECS_EXPORT_COMPONENT(EcsSquare);
    ECS_EXPORT_COMPONENT(EcsCircle);
    ECS_EXPORT_COMPONENT(EcsBox);
    ECS_EXPORT_COMPONENT(EcsLineColor);
    ECS_EXPORT_COMPONENT(EcsLineWidth);
}
