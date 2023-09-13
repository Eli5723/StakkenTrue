#include "./Application.h"

#include <spdlog/spdlog.h>

#include <Assets/Assets.hpp>
#include <Draw/Draw.hpp>
#include <UI/UI.hpp>
#include <UI/Components.hpp>
#include <Game/DrawGame.hpp>
#include <Input/Input.hpp>
#include <Draw/Background.hpp>
#include <Game/Game.hpp>
#include <Audio/Music.hpp>
#include <View.hpp>

#include <glm/ext/matrix_transform.hpp>

namespace Application
{
    void MessageCallback(GLenum source,
                         GLenum type,
                         GLuint id,
                         GLenum severity,
                         GLsizei length,
                         const GLchar *message,
                         const void *userParam)
    {
        bool error = type == GL_DEBUG_TYPE_ERROR;
        if (error)
            spdlog::error("GL Error: {}", message);
        else
            spdlog::warn("GL Warning: {}", message);
    }

    const int DEFAULT_WIDTH = 1920;
    const int DEFAULT_HEIGHT = 1080;

    Engine::Draw::Viewport viewport{0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT};

    SDL_Window *window;
    SDL_GLContext context;

    bool running = true;

    Core::Game::Game testGame;

    Client::Input::Keyboard keyboard;
    Engine::Assets::Font *font;

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

        gl3wInit();
        glDebugMessageCallback(MessageCallback, 0);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        UI::Init();
        Audio::Music::Setup();
        Engine::View::Initialize(glm::vec2{DEFAULT_WIDTH, DEFAULT_HEIGHT});
        Game::setActiveTexture(Engine::Assets::Texture::Load("./Resources/Textures/Pieces/TGF.png"));
        font = Engine::Assets::Font::Load("./Resources/Fonts/Roboto-Regular.ttf");

        keyboard.profile.ReadKeyboardProfile("./Settings/Keyboard.toml");
        Engine::Draw::Quad::Setup();
        Engine::Draw::Line::Setup();
        Engine::Draw::Background::Init(glm::vec2{DEFAULT_WIDTH, DEFAULT_HEIGHT});
        testGame.StartGame();

        set_resolution(DEFAULT_WIDTH, DEFAULT_HEIGHT);

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

            keyboard.Update(2);

            Core::Game::EventStream stream = keyboard.GetInputStream();
            testGame.ApplyEventStream(stream);
            keyboard.Clear();

            Engine::View::StartFrame();
            Engine::Draw::Background::Draw();

            Engine::Draw::Quad::SetProjectionMatrix(viewport.DefaultProjectionMatrix());
            Engine::Draw::Line::SetProjectionMatrix(viewport.DefaultProjectionMatrix());

            Engine::Draw::Quad::BeginBatch();
            Engine::Draw::Line::BeginBatch();

            testGame.Tick(2);
            Game::drawGame({20.0f, 20.0f}, testGame);

            UI::Render();
            Engine::Draw::Quad::Finish();
            Engine::Draw::Line::Finish();

            char seedString[64];
            sprintf(seedString, "Seed: %d", testGame.seed);
            Engine::Draw::Text::Immediate({20.0f, 20.0f}, seedString, *font);
            Engine::View::EndFrame();

            SDL_GL_SwapWindow(window);
            SDL_Delay(2);
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
            if (event.key.repeat)
                return;

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

        case SDL_WINDOWEVENT_RESIZED:
            set_resolution(event.data1, event.data2);
            break;
        }
    }

    void events_keydown(const SDL_KeyboardEvent &event)
    {
        if (event.keysym.sym == SDLK_ESCAPE)
            stop();

        if (event.keysym.sym == SDLK_F2)
            testGame.StartGame();

        if (event.keysym.sym == SDLK_F3)
            Audio::Music::NextSong();

        keyboard.KeyDown(event);
    }

    void events_keyup(const SDL_KeyboardEvent &event)
    {
    }

    void events_mousemotion(const SDL_MouseMotionEvent &event)
    {
    }

    void events_mousedown(const SDL_MouseButtonEvent &event)
    {
        UI::Click(glm::vec2{event.x, event.y});
    }

    void events_mouseup(const SDL_MouseButtonEvent &event)
    {
    }

    void set_resolution(u32 x, u32 y)
    {
        SDL_SetWindowSize(window, x, y);
        viewport = Engine::Draw::Viewport{0, 0, x, y};
        Engine::Draw::Quad::SetProjectionMatrix(viewport.DefaultProjectionMatrix());
        Engine::Draw::Line::SetProjectionMatrix(viewport.DefaultProjectionMatrix());
        Engine::View::Resize({x, y});
        Engine::Draw::Background::SetResolution({x, y});
    }
}