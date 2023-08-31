#include <SDL2/SDL.h>
#include <defines.h>

namespace Application
{
    void init();
    void stop();

    // Main Thread Functions
    void application_thread();
    void network_thread();
    void render_thread();

    // Window API
    void set_resolution(u32 x, u32 y);

    // Event handlers
    void events(const SDL_Event &event);

    // Window Events
    void events_window(const SDL_WindowEvent &event);

    void events_mousemotion(const SDL_MouseMotionEvent &event);
    void events_mousedown(const SDL_MouseButtonEvent &event);
    void events_mouseup(const SDL_MouseButtonEvent &event);

    void events_keydown(const SDL_KeyboardEvent& event);
    void events_keyup(const SDL_KeyboardEvent& event);
};