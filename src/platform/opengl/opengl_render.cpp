#include "opengl_render.h"

#include <glad/glad.h>

namespace inx
{
    void OpenGLRenderAPI::init() const 
    {
        glEnable(GL_DEPTH_TEST);
    }

    void OpenGLRenderAPI::set_viewport(int width, int height) const 
    {
        glViewport(0, 0, width, height);
    }

    void OpenGLRenderAPI::clear_colour(glm::vec3 colour) const
    {
        glClearColor(colour.x, colour.y, colour.z, 1.f);
    }

    void OpenGLRenderAPI::clear() const
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
} // namespace inx