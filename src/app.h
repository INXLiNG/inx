#ifndef __INX_APP_H__
#define __INX_APP_H__

#include <filesystem>
#include <iostream>

#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>

#include "renderer.h"
#include "resources.h"

#include "cubes.h"

namespace inx
{
    void run_app(const char* title = "SDL3 Window - ESC to close", int screen_width = 800, int screen_height = 600)
    {
        // initialize sdl3

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

        // initialize opengl

        auto gl_context = SDL_GL_CreateContext(window);
        SDL_GL_MakeCurrent(window, gl_context);
        SDL_GL_SetSwapInterval(0);

        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
        {
            std::cerr << "Could not intialize OpenGL: " << SDL_GetError() << "\n";
            SDL_Quit();
            return;
        }

        render_api::init();

        // initialize imgui

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

        ImGui::StyleColorsDark();

        ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
        ImGui_ImplOpenGL3_Init("#version 410");

        std::cout << "OpenGL Loaded\n";
        std::cout << " - Vendor:    " << glGetString(GL_VENDOR)   << "\n";
        std::cout << " - Renderer:  " << glGetString(GL_RENDERER) << "\n";
        std::cout << " - Version:   " << glGetString(GL_VERSION)  << "\n";

        ResourceManager manager;
        PerspectiveCamera camera{glm::vec3(0.f, 0.f, 3.f)};

        init_cubes(manager);

        // state for imgui
        bool rotate_cubes = false;
        glm::vec3 clear_colour = glm::vec3(.1f, .1f, .1f);

        // loop state
        float delta_time = 0.f, last_frame = 0.f;
        bool loop = true;

        while (loop)
        {
            float current_frame = (float)SDL_GetTicks() / 1000.f;
            delta_time = current_frame - last_frame;
            last_frame = current_frame;

            SDL_Event e;
            while (SDL_PollEvent(&e))
            {
                ImGui_ImplSDL3_ProcessEvent(&e);
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
                        render_api::viewport(screen_width, screen_height);
                    } break;

                    case SDL_EVENT_KEY_DOWN:
                    {
                        switch(e.key.key)
                        {
                            case SDLK_W: camera.position(PerspectiveCamera::Direction::Forward, delta_time); break;
                            case SDLK_S: camera.position(PerspectiveCamera::Direction::Backward, delta_time); break;
                            case SDLK_A: camera.position(PerspectiveCamera::Direction::Left, delta_time); break;
                            case SDLK_D: camera.position(PerspectiveCamera::Direction::Right, delta_time); break;
                            case SDLK_Q: camera.position(PerspectiveCamera::Direction::Up, delta_time); break;
                            case SDLK_E: camera.position(PerspectiveCamera::Direction::Down, delta_time); break;

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
                            camera.pov(xoffset, yoffset);
                        }
                    } break;

                    case SDL_EVENT_MOUSE_WHEEL:
                    {
                        if (capture_mouse)
                        {
                            float dy = e.wheel.y;
                            camera.fov(dy);
                        }
                    } break;
                }
            }

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();

            {
                ImGui::Begin("Menu");

                ImGui::Checkbox("Rotate Cubes", &rotate_cubes);
                ImGui::ColorEdit3("Clear Colour", (float*)&clear_colour.x);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
                ImGui::End();
            }

            render_api::clear_colour(clear_colour);
            render_api::clear();
            
            draw_cubes(manager, camera, screen_width, screen_height, rotate_cubes);
            
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            
            SDL_GL_SwapWindow(window);
        }

        // cleanup

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();

        SDL_GL_DestroyContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
} // namespace inx

#endif // __INX_APP_H__