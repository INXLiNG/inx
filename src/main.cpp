#include <SDL3/SDL.h>   // used for SDL
#include <glad/glad.h>  // used for OpenGL

#include <iostream>     // used for std::cout, std::cerr
#include <filesystem>   // used for std::filesystem::path

#include "resources/shader.h"

#define PATH(file) std::filesystem::path(RES_PATH) / file

#define SCREEN_WIDTH    800
#define SCREEN_HEIGHT   600

int main(int argc, char** argv)
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Could not initialize SDL: " << SDL_GetError() << "\n";
        return 0;
    }

    SDL_Window* window = SDL_CreateWindow("SDL2 Window", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    if (!window)
    {
        std::cerr << "Could not create SDL window: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 0;
    }

    SDL_GL_CreateContext(window);
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cerr << "Could not intialize OpenGL: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 0;
    }

    inx::ResourceManager manager;

    inx::ShaderParams params;
    params.vert_filepath = PATH("tri.vs");
    params.frag_filepath = PATH("tri.fs");

    manager.load_resource<inx::Shader>("main", params);

    std::cout << "OpenGL Loaded\n";
    std::cout << " - Vendor:    " << glGetString(GL_VENDOR)   << "\n";
    std::cout << " - Renderer:  " << glGetString(GL_RENDERER) << "\n";
    std::cout << " - Version:   " << glGetString(GL_VERSION)  << "\n";

    glClearColor(0.5, 0.0, 0.5, 1.0);

    float vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    bool loop = true;
    while (loop)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch(e.type)
            {
                case SDL_EVENT_QUIT:
                    loop = false;
                    break;
                default:
                    break;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);

        manager.get<inx::Shader>("main")->use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        SDL_GL_SwapWindow(window);
    }

    SDL_Quit();

    return 0;
}