#include "../opengl.h"

#include <glad/glad.h>

namespace inx
{
    void OpenGLRenderAPI::init() const 
    {
        glEnable(GL_DEPTH_TEST);
    }

    void OpenGLRenderAPI::viewport(i32 width, i32 height) const 
    {
        glViewport(0, 0, width, height);
    }

    void OpenGLRenderAPI::clear_colour(const glm::vec3& colour) const
    {
        glClearColor(colour.x, colour.y, colour.z, 1.f);
    }

    void OpenGLRenderAPI::clear() const
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
} // namespace inx