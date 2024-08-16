#ifndef __INX_APP_H__
#define __INX_APP_H__

#include <iostream>                 // used for std::cout, std::cerr
#include <filesystem>               // used for std::filesystem::path

#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

#include "resources/resources.h"
#include "resources/shader.h"
#include "resources/texture.h"

#include "renderer/buffer.h"
#include "renderer/vertex_array.h"
#include "renderer/camera.h"

#define PATH(filepath) std::filesystem::path(RES_PATH) / filepath

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

        ResourceManager manager;
        {
            std::filesystem::path material_shader_vs = PATH("material.vs");
            std::filesystem::path material_shader_fs = PATH("material.fs");
            manager.load_resource<Shader>("material", material_shader_vs, material_shader_fs);

            std::filesystem::path cube_shader_vs = PATH("light_cube.vs");
            std::filesystem::path cube_shader_fs = PATH("light_cube.fs");
            manager.load_resource<Shader>("light_cube", cube_shader_vs, cube_shader_fs);
            
            std::filesystem::path container_path = PATH("container.png");
            manager.load_resource<Texture>("container", container_path);

            std::filesystem::path container_spec_path = PATH("container_spec.png");
            manager.load_resource<Texture>("container_spec", container_spec_path);
        }

        std::cout << "OpenGL Loaded\n";
        std::cout << " - Vendor:    " << glGetString(GL_VENDOR)   << "\n";
        std::cout << " - Renderer:  " << glGetString(GL_RENDERER) << "\n";
        std::cout << " - Version:   " << glGetString(GL_VERSION)  << "\n";

        float vertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
        };
        
        std::vector<glm::vec3> cube_positions;
        for (int x = -25; x < 25; x += 5)
            for (int y = -25; y < 25; y += 5)
                for (int z = -25; z < 25; z += 5)
                    cube_positions.push_back(glm::vec3((float)x, (float)y, (float)z));
        
        glm::vec3 light_positions[] = {
            glm::vec3( 0.7f,  0.2f,  2.0f),
            glm::vec3( 2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3( 0.0f,  0.0f, -3.0f)
        };

        auto cube_vbo = IVertexBuffer::create(vertices, sizeof(vertices));
        cube_vbo->set_layout({
            { "position",  BufferElementDataType::Float3 },
            { "normal",    BufferElementDataType::Float3 },
            { "tex_coord", BufferElementDataType::Float2 },
        });

        auto cube_vao = IVertexArray::create();
        cube_vao->add_vertex_buffer(cube_vbo);

        auto light_cube_vao = IVertexArray::create();
        light_cube_vao->add_vertex_buffer(cube_vbo);

        PerspectiveCamera camera{glm::vec3(0.f, 0.f, 3.f)};

        float delta_time = 0.f;
        float last_frame = 0.f;

        bool rotate_cubes = false;

        {
            auto& shader = manager.get_resource<Shader>("material");
            shader.bind();
            shader.set_int("material.diffuse", 0);
            shader.set_int("material.specular", 1);
        }

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

            auto& lighting_shader = manager.get_resource<Shader>("material");
            lighting_shader.bind();
            lighting_shader.set_vec3("viewPos", camera.get_position());
            lighting_shader.set_float("material.shininess", 32.f);

            // directional light
            lighting_shader.set_vec3("dirLight.direction", glm::vec3(-.2f, -1.f, -.3f));
            lighting_shader.set_vec3("dirLight.ambient", glm::vec3(.05f, .05f, .05f));
            lighting_shader.set_vec3("dirLight.diffuse", glm::vec3(.4f, .4f, .4f));
            lighting_shader.set_vec3("dirLight.specular", glm::vec3(.5f, .5f, .5f));

            // 1st point light
            lighting_shader.set_vec3("pointLights[0].position", light_positions[0]);
            lighting_shader.set_vec3("pointLights[0].ambient", glm::vec3(.05f, .05f, .05f));
            lighting_shader.set_vec3("pointLights[0].diffuse", glm::vec3(.8f, .8f, .8f));
            lighting_shader.set_vec3("pointLights[0].specular", glm::vec3(1.f, 1.f, 1.f));
            lighting_shader.set_float("pointLights[0].constant", 1.f);
            lighting_shader.set_float("pointLights[0].linear", .09f);
            lighting_shader.set_float("pointLights[0].quadratic", .032f);

            // 2nd point light
            lighting_shader.set_vec3("pointLights[1].position", light_positions[1]);
            lighting_shader.set_vec3("pointLights[1].ambient", glm::vec3(.05f, .05f, .05f));
            lighting_shader.set_vec3("pointLights[1].diffuse", glm::vec3(.8f, .8f, .8f));
            lighting_shader.set_vec3("pointLights[1].specular", glm::vec3(1.f, 1.f, 1.f));
            lighting_shader.set_float("pointLights[1].constant", 1.f);
            lighting_shader.set_float("pointLights[1].linear", .09f);
            lighting_shader.set_float("pointLights[1].quadratic", .032f);

            // 3rd point light
            lighting_shader.set_vec3("pointLights[2].position", light_positions[2]);
            lighting_shader.set_vec3("pointLights[2].ambient", glm::vec3(.05f, .05f, .05f));
            lighting_shader.set_vec3("pointLights[2].diffuse", glm::vec3(.8f, .8f, .8f));
            lighting_shader.set_vec3("pointLights[2].specular", glm::vec3(1.f, 1.f, 1.f));
            lighting_shader.set_float("pointLights[2].constant", 1.f);
            lighting_shader.set_float("pointLights[2].linear", .09f);
            lighting_shader.set_float("pointLights[2].quadratic", .032f);

            // 4th point light
            lighting_shader.set_vec3("pointLights[3].position", light_positions[3]);
            lighting_shader.set_vec3("pointLights[3].ambient", glm::vec3(.05f, .05f, .05f));
            lighting_shader.set_vec3("pointLights[3].diffuse", glm::vec3(.8f, .8f, .8f));
            lighting_shader.set_vec3("pointLights[3].specular", glm::vec3(1.f, 1.f, 1.f));
            lighting_shader.set_float("pointLights[3].constant", 1.f);
            lighting_shader.set_float("pointLights[3].linear", .09f);
            lighting_shader.set_float("pointLights[3].quadratic", .032f);

            // 4th point light
            lighting_shader.set_vec3("spotLight.position", camera.get_position());
            lighting_shader.set_vec3("spotLight.direction", camera.get_view_direction());
            lighting_shader.set_vec3("spotLight.ambient", glm::vec3(0.f, 0.f, 0.f));
            lighting_shader.set_vec3("spotLight.diffuse", glm::vec3(1.f, 1.f, 1.f));
            lighting_shader.set_vec3("spotLight.specular", glm::vec3(1.f, 1.f, 1.f));
            lighting_shader.set_float("spotLight.constant", 1.f);
            lighting_shader.set_float("spotLight.linear", .09f);
            lighting_shader.set_float("spotLight.quadratic", .032f);
            lighting_shader.set_float("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
            lighting_shader.set_float("spotLight.outerCutOff", glm::cos(glm::radians(15.f)));

            auto proj = glm::perspective(glm::radians(camera.get_fov()), (float)(screen_width / screen_height), .1f, 100.f);
            auto view = camera.get_view_matrix();
            lighting_shader.set_mat4("projection", proj);
            lighting_shader.set_mat4("view", view);

            glm::mat4 model = glm::mat4(1.f);
            lighting_shader.set_mat4("model", model);

            manager.get_resource<Texture>("container").bind(GL_TEXTURE0);
            manager.get_resource<Texture>("container_spec").bind(GL_TEXTURE1);

            cube_vao->bind();
            for (unsigned int i = 0; i < cube_positions.size(); i++)
            {
                float angle = rotate_cubes ? (20.f * i) * (SDL_GetTicks() / 1000.f / 50.f) : 0;

                model = glm::mat4(1.f);
                model = glm::translate(model, cube_positions[i]);
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.f, .3f, .5f));
                lighting_shader.set_mat4("model", model);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            auto& cube_shader = manager.get_resource<Shader>("light_cube");
            cube_shader.bind();
            cube_shader.set_mat4("projection", proj);
            cube_shader.set_mat4("view", view);

            light_cube_vao->bind();
            for (unsigned int i = 0; i < 4; i++)
            {
                model = glm::mat4(1.f);
                model = glm::translate(model, light_positions[i]);
                model = glm::scale(model, glm::vec3(.2f));
                cube_shader.set_mat4("model", model);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            
            SDL_GL_SwapWindow(window);
        }

        SDL_Quit();
    }
} // namespace inx

#endif // __INX_APP_H__