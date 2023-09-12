#include <Draw/Viewport.hpp>

#include <GL/gl3w.h>

namespace Engine {
namespace Draw {

void SetViewport(const Viewport &viewport)
{
    glViewport(viewport.screen_x, viewport.screen_y, viewport.screen_width, viewport.screen_height);
    glScissor(viewport.screen_x, viewport.screen_y, viewport.screen_width, viewport.screen_height);
}

void camera_viewport_mouse(i32 mouse_x, i32 mouse_y, Viewport &viewport, Camera &camera, float &out_x, float &out_y)
{
    float mouse_viewport_center_rel_x = mouse_x - viewport.screen_x - viewport.screen_width / 2;
    float mouse_viewport_center_rel_y = mouse_y - viewport.screen_y - viewport.screen_height / 2;

    out_x = camera.position.x + mouse_viewport_center_rel_x / camera.zoom;
    out_y = camera.position.y + mouse_viewport_center_rel_y / camera.zoom;
}

}
}