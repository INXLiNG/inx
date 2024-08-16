#ifndef __INX_APP_H__
#define __INX_APP_H__

#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

#include <iostream>     // used for std::cout, std::cerr
#include <filesystem>   // used for std::filesystem::path

#include "resources/resources.h"
#include "resources/shader.h"
#include "resources/texture.h"

#include "renderer/buffer.h"
#include "renderer/vertex_array.h"
#include "renderer/camera.h"

#define PATH(filepath) std::filesystem::path(RES_PATH) / filepath

namespace inx
{
    void run_app(const char* title = "SDL3 Window", int screen_width = 800, int screen_height = 600)
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

        SDL_SetWindowRelativeMouseMode(window, SDL_TRUE);

        SDL_GL_CreateContext(window);
        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
        {
            std::cerr << "Could not intialize OpenGL: " << SDL_GetError() << "\n";
            SDL_Quit();
            return;
        }

        glEnable(GL_DEPTH_TEST);

        ResourceManager manager;

        std::filesystem::path vert = PATH("tri.vs");
        std::filesystem::path frag = PATH("tri.fs");
        manager.load_resource<Shader>("tri", vert, frag);
        
        std::filesystem::path img_path = PATH("container.jpg");
        manager.load_resource<Texture>("wall", img_path);

        std::cout << "OpenGL Loaded\n";
        std::cout << " - Vendor:    " << glGetString(GL_VENDOR)   << "\n";
        std::cout << " - Renderer:  " << glGetString(GL_RENDERER) << "\n";
        std::cout << " - Version:   " << glGetString(GL_VERSION)  << "\n";

        float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };
        
        glm::vec3 cube_positions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
        };

        auto vertex_buffer = IVertexBuffer::create(vertices, sizeof(vertices));
        vertex_buffer->set_layout({
            { "position",  BufferElementDataType::Float3 },
            { "tex_coord", BufferElementDataType::Float2 },
        });

        auto vertex_array = IVertexArray::create();
        vertex_array->add_vertex_buffer(vertex_buffer);

        PerspectiveCamera camera{glm::vec3(0.f, 0.f, 3.f)};

        float delta_time = 0.f;
        float last_frame = 0.f;

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
                        float speed = 10.f * delta_time;
                        switch(e.key.key)
                        {
                            case SDLK_W: camera.change_position(PerspectiveCamera::DIRECTION::FORWARD, delta_time); break;
                            case SDLK_S: camera.change_position(PerspectiveCamera::DIRECTION::BACKWARD, delta_time); break;
                            case SDLK_A: camera.change_position(PerspectiveCamera::DIRECTION::LEFT, delta_time); break;
                            case SDLK_D: camera.change_position(PerspectiveCamera::DIRECTION::RIGHT, delta_time); break;

                            case SDLK_ESCAPE: loop = false;
                        }
                    } break;

                    case SDL_EVENT_MOUSE_MOTION:
                    {
                        float xoffset = e.motion.xrel;
                        float yoffset = -e.motion.yrel;
                        camera.change_pov(xoffset, yoffset);
                    } break;

                    case SDL_EVENT_MOUSE_WHEEL:
                    {
                        float dy = e.wheel.y;
                        camera.change_fov(dy);
                    } break;
                }
            }

            glClearColor(0.5, 0.0, 0.5, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            manager.get_resource<Texture>("wall").bind();
            auto& shader = manager.get_resource<Shader>("tri");
            shader.bind();

            glm::mat4 projection = glm::perspective(glm::radians(camera.get_fov()), (float)screen_width / (float)screen_height, .1f, 100.f);
            shader.set_mat4("projection", projection);

            glm::mat4 view = camera.get_view_matrix();
            shader.set_mat4("view", view);

            vertex_array->bind();
            for (size_t i = 0; i < 10; i++)
            {
                float angle = (20.f * i) + ((float)SDL_GetTicks() / 10.f);

                glm::mat4 model = glm::mat4(1.f);
                model = glm::translate(model, cube_positions[i]);
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.f, .3f, .5f));
                
                shader.set_mat4("model", model);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            
            SDL_GL_SwapWindow(window);
        }

        SDL_Quit();
    }
} // namespace inx

#endif // __INX_APP_H__