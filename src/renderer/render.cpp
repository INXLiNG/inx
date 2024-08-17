#include "render.h"

#include "../platform/opengl/opengl_render.h"

struct RenderData
{
    std::unique_ptr<inx::RenderAPI> api;
};

static RenderData s_render_data;

namespace inx
{
    void render_api::init()
    {
        s_render_data.api = std::make_unique<OpenGLRenderAPI>();
        s_render_data.api->init();
    }

    void render_api::set_viewport(int width, int height)
    {
        s_render_data.api->set_viewport(width, height);
    }

    void render_api::clear_colour(glm::vec3 colour)
    {
        s_render_data.api->clear_colour(colour);
    }

    void render_api::clear()
    {
        s_render_data.api->clear();
    }
} // namespace inx