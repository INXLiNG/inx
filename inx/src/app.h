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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

#include "inx/core.h"
#include "inx/renderer.h"
#include "inx/resources.h"

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
        SDL_GL_SetSwapInterval(1);

        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
        {
            std::cerr << "Could not intialize OpenGL: " << SDL_GetError() << "\n";
            SDL_Quit();
            return;
        }

        render_api::init();
        Keyboard::init();

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

        float ar = (float)screen_width / (float)screen_height;
        float zoom = 1.f;

        ResourceManager manager;
        PerspectiveCamera camera{glm::vec3(0.f, 0.f, 3.f)};
        OrthographicCamera orth_camera{-ar * zoom, ar * zoom, -zoom, zoom};

        init_cubes(manager);

        // state for imgui
        bool rotate_cubes = false;
        glm::vec3 clear_colour = glm::vec3(.1f, .1f, .1f);

        render2d::init(manager);

        // loop state
        float delta_time = 0.f, last_frame = 0.f;
        bool loop = true;

        {
            auto quad_vs = PATH("quad.vs");
            auto quad_fs = PATH("quad.fs");
            manager.load_resource<Shader>("quad", quad_vs, quad_fs);

            auto& shader = manager.get_resource<Shader>("quad");
            shader.bind();
            int samplers[32];
            for (int i = 0; i < 32; i++)
                samplers[i] = i;
            shader.set_ints("u_textures", samplers, 32);
        }

        float timer = 0.f;

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

                        ar = (float)screen_width / (float)screen_height;
                        orth_camera.projection(-ar * zoom, ar * zoom, -zoom, zoom);
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

                            zoom -= dy;
                            orth_camera.projection(-ar * zoom, ar * zoom, -zoom, zoom);
                        }
                    } break;
                }
            }

            Keyboard::update();
            
            if (Keyboard::key_down(Key::KEY_ESCAPE)) loop = false;

            if (Keyboard::key(Key::KEY_W)) camera.position(PerspectiveCamera::Direction::Forward, delta_time);
            if (Keyboard::key(Key::KEY_S)) camera.position(PerspectiveCamera::Direction::Backward, delta_time);
            if (Keyboard::key(Key::KEY_A)) camera.position(PerspectiveCamera::Direction::Left, delta_time);
            if (Keyboard::key(Key::KEY_D)) camera.position(PerspectiveCamera::Direction::Right, delta_time);
            if (Keyboard::key(Key::KEY_Q)) camera.position(PerspectiveCamera::Direction::Up, delta_time);
            if (Keyboard::key(Key::KEY_E)) camera.position(PerspectiveCamera::Direction::Down, delta_time);


            if (Keyboard::key_down(Key::KEY_C))
            {
                capture_mouse = !capture_mouse;
                SDL_SetWindowRelativeMouseMode(window, capture_mouse);
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

            // draw quad test where we draw a bunch of squares
            {
                auto& shader = manager.get_resource<Shader>("quad");
                manager.get_resource<Texture>("r2d_white").bind();
                shader.bind();
                
                auto proj = glm::perspective(glm::radians(camera.fov()), (float)screen_width / (float)screen_height, .1f, 100.f);
                shader.set_mat4("u_vp_matrix", proj * camera.view_matrix());
                
                auto model = glm::mat4(1.f);
                model = glm::translate(model, glm::vec3(0.f, 0.f, 0.f));
                shader.set_mat4("u_model", model);
                
                render2d::begin_batch();

                glm::vec4 colour = glm::vec4(4.f, 0.f, .6f, 1.f);
                for (float y = -10.f; y < 10.f; y += .25f)
                    for (float x = -10.f; x < 10.f; x += .25f)
                        render2d::draw_quad(glm::vec3(x, y, 0.f), glm::vec2(.2f, .2f), colour);

                render2d::end_batch();
                render2d::flush();
            }
            
            // draw_cubes(manager, camera, screen_width, screen_height, rotate_cubes);
            
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            
            SDL_GL_SwapWindow(window);
        }

        // cleanup

        render2d::shutdown();

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();

        SDL_GL_DestroyContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
} // namespace inx

#endif // __INX_APP_H__