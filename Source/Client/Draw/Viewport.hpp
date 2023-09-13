#pragma once

#include <defines.h>

#include <GL/gl3w.h>

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/scalar_constants.hpp>

namespace Engine {
namespace Draw {

struct Viewport
{
    u32 screen_x;
    u32 screen_y;
    u32 screen_width;
    u32 screen_height;

    inline glm::highp_mat4 DefaultProjectionMatrix()
    {
        return glm::ortho(0.0f, (float)screen_width * .5f, (float)screen_height * .5f, 0.0f);
    }
};

struct Camera
{
    glm::vec2 position;
    float zoom;

    float world_left;
    float world_right;
    float world_top;
    float world_bottom;

    void update(const Viewport &viewport);


    inline glm::highp_mat4 ViewportProjectionMatrix()
    {
        return glm::ortho(world_left, world_right, world_bottom, world_top);
    }
};

void camera_viewport_mouse(i32 mouse_x, i32 mouse_y, Viewport &viewport, Camera &camera, float &out_x, float &out_y);

void SetViewport(const Viewport &viewport);

}
}