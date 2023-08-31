#include "./Application.h"

#include <spdlog/spdlog.h>

namespace Application
{
    const int DEFAULT_WIDTH = 720;
    const int DEFAULT_HEIGHT = 640;

    SDL_Window *window;
    SDL_GLContext context;

    bool running = true;

    void init()
    {
        SDL_Init(SDL_INIT_EVERYTHING);
        SDL_EventState(SDL_DROPFILE, SDL_ENABLE);

        window = SDL_CreateWindow("Stakken", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DEFAULT_WIDTH, DEFAULT_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        context = SDL_GL_CreateContext(window);
        SDL_GL_SetSwapInterval(0);
        SDL_ShowCursor(true);

        application_thread();
    }

    void stop()
    {
        running = false;
    }

    void application_thread()
    {
        while (running)
        {
            SDL_Event event;

            while (SDL_PollEvent(&event))
            {
                events(event);
            }

            SDL_GL_SwapWindow(window);
            SDL_Delay(16);
        }
    }

    void render_thread()
    {
    }

    void events(const SDL_Event &event)
    {
        switch (event.type)
        {
            case SDL_QUIT:
            {
                running = false;
            }
            break;

            case SDL_WINDOWEVENT:
            {
                events_window(event.window);
                return;
            }
            break;

            case SDL_KEYDOWN:
            {
                events_keydown(event.key);
            }
            break;
            

            case SDL_KEYUP:
            {
                events_keyup(event.key);
            }
            break;
            

            case SDL_MOUSEMOTION:
            {
                events_mousemotion(event.motion);
            }
            break;
             
            case SDL_MOUSEBUTTONDOWN:
            {
                events_mousedown(event.button);
            }
            break;
            
            case SDL_MOUSEBUTTONUP:
            {
                events_mouseup(event.button);
            }
            break;
            
        }
    }

    void events_window(const SDL_WindowEvent &event)
    {
        switch (event.event)
        {
        case SDL_WINDOWEVENT_CLOSE:
            stop();
            return;
            break;
        }
    }

    void events_keydown(const SDL_KeyboardEvent &event)
    {
        spdlog::info("{}", event.keysym.sym);
        if (event.keysym.sym == SDLK_ESCAPE)
            stop();
    }

    void events_keyup(const SDL_KeyboardEvent &event)
    {
    }

    void events_mousemotion(const SDL_MouseMotionEvent &event)
    {
    }

    void events_mousedown(const SDL_MouseButtonEvent &event)
    {
    }

    void events_mouseup(const SDL_MouseButtonEvent &event)
    {
    }
}