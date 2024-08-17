#ifndef __INX_APP_H__
#define __INX_APP_H__

#include <iostream>                 // used for std::cout, std::cerr
#include <filesystem>               // used for std::filesystem::path

#include <SDL3/SDL.h>
#include <glad/glad.h>

#include "resources/resources.h"
#include "resources/shader.h"
#include "resources/texture.h"

#include "renderer/buffer.h"
#include "renderer/vertex_array.h"
#include "renderer/camera.h"

#include "cubes.h"

namespace inx
{
    void run_app(const char* title = "SDL3 Window - ESC to close", int screen_width = 800, int screen_height = 600)
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            std::cerr << "Could not initialize SDL: " << SDL_GetError() << "\n";
            return;
        }

        SDL_Window* window = SDL_CreateWindow(title, screen_width, screen_height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
        if (!window)
        {
            std::cerr << "Could not create SDL window: " << SDL_GetError() << "\n";
            SDL_Quit();
            return;
        }

        bool capture_mouse = true;
        SDL_SetWindowRelativeMouseMode(window, capture_mouse);

        SDL_GL_CreateContext(window);
        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
        {
            std::cerr << "Could not intialize OpenGL: " << SDL_GetError() << "\n";
            SDL_Quit();
            return;
        }

        glEnable(GL_DEPTH_TEST);

        std::cout << "OpenGL Loaded\n";
        std::cout << " - Vendor:    " << glGetString(GL_VENDOR)   << "\n";
        std::cout << " - Renderer:  " << glGetString(GL_RENDERER) << "\n";
        std::cout << " - Version:   " << glGetString(GL_VERSION)  << "\n";

        ResourceManager manager;
        PerspectiveCamera camera{glm::vec3(0.f, 0.f, 3.f)};

        float delta_time = 0.f, last_frame = 0.f;

        bool rotate_cubes = false;
        // init_cubes(manager);

        bool loop = true;
        while (loop)
        {

            float current_frame = (float)SDL_GetTicks() / 1000.f;
            delta_time = current_frame - last_frame;
            last_frame = current_frame;

            SDL_Event e;
            while (SDL_PollEvent(&e))
            {
                switch(e.type)
                {
                    case SDL_EVENT_QUIT:
                    {
                        loop = false;
                    } break;

                    case SDL_EVENT_WINDOW_RESIZED:
                    {
                        screen_width = e.window.data1;
                        screen_height = e.window.data2;
                        glViewport(0, 0, screen_width, screen_height);
                    } break;

                    case SDL_EVENT_KEY_DOWN:
                    {
                        switch(e.key.key)
                        {
                            case SDLK_W: camera.change_position(PerspectiveCamera::DIRECTION::FORWARD, delta_time); break;
                            case SDLK_S: camera.change_position(PerspectiveCamera::DIRECTION::BACKWARD, delta_time); break;
                            case SDLK_A: camera.change_position(PerspectiveCamera::DIRECTION::LEFT, delta_time); break;
                            case SDLK_D: camera.change_position(PerspectiveCamera::DIRECTION::RIGHT, delta_time); break;
                            case SDLK_Q: camera.change_position(PerspectiveCamera::DIRECTION::UP, delta_time); break;
                            case SDLK_E: camera.change_position(PerspectiveCamera::DIRECTION::DOWN, delta_time); break;

                            case SDLK_R: rotate_cubes = !rotate_cubes; break;

                            case SDLK_TAB:
                            {
                                capture_mouse = !capture_mouse;
                                SDL_SetWindowRelativeMouseMode(window, capture_mouse);
                            } break;

                            case SDLK_ESCAPE: loop = false; break;
                        }
                    } break;

                    case SDL_EVENT_MOUSE_MOTION:
                    {
                        if (capture_mouse)
                        {
                            float xoffset = e.motion.xrel;
                            float yoffset = -e.motion.yrel;
                            camera.change_pov(xoffset, yoffset);
                        }
                    } break;

                    case SDL_EVENT_MOUSE_WHEEL:
                    {
                        if (capture_mouse)
                        {
                            float dy = e.wheel.y;
                            camera.change_fov(dy);
                        }
                    } break;
                }
            }

            glClearColor(0.1, 0.1, 0.1, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // draw_cubes(manager, camera, screen_width, screen_height, rotate_cubes);
            
            SDL_GL_SwapWindow(window);
        }

        SDL_Quit();
    }
} // namespace inx

#endif // __INX_APP_H__