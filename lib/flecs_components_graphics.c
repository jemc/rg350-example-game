/**
 * This source code was taken from the following URL:
 * https://raw.githubusercontent.com/flecs-hub/flecs-components-graphics/5421da4b315ff8ec47d822a581982243b456a485/src/graphics.c
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

#include <flecs_components_graphics.h>
#include <string.h>

void FlecsComponentsGraphicsImport(
    ecs_world_t *world)
{
    ECS_MODULE(world, FlecsComponentsGraphics);

    ecs_set_name_prefix(world, "Ecs");

    ECS_IMPORT(world, FlecsMeta);

    ECS_COMPONENT(world, EcsCamera);
    ECS_COMPONENT(world, EcsDirectionalLight);
    ECS_COMPONENT(world, EcsRgb);
    ECS_COMPONENT(world, EcsRgba);
    ECS_COMPONENT(world, EcsSpecular);
    ECS_COMPONENT(world, EcsEmissive);

    ECS_EXPORT_COMPONENT(EcsCamera);
    ECS_EXPORT_COMPONENT(EcsDirectionalLight);
    ECS_EXPORT_COMPONENT(EcsRgb);
    ECS_EXPORT_COMPONENT(EcsRgba);
    ECS_EXPORT_COMPONENT(EcsSpecular);
    ECS_EXPORT_COMPONENT(EcsEmissive);
}
