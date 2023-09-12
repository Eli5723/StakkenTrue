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
    i32 screen_x;
    i32 screen_y;
    i32 screen_width;
    i32 screen_height;

    inline glm::highp_mat4 DefaultProjectionMatrix()
    {
        return glm::ortho(0.0f, (float)screen_width * .5f, (float)screen_height * .5f, 0.0f);
    }

    void Activate()
    {
        glViewport(0, 0, screen_width, screen_height);
        glScissor(screen_x, screen_y, screen_width, screen_height);
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