#include "../renderer.h"
#include "renderer_internal.h"

#include "../platform/opengl.h"

struct APIData
{
    inx::Ref<inx::RenderAPI> api;
};

static APIData API_DATA;

namespace inx
{
    void render_api::init()
    {
        API_DATA.api = create_ref<OpenGLRenderAPI>();
        API_DATA.api->init();
    }

    void render_api::viewport(i32 width, i32 height)
    {
        API_DATA.api->viewport(width, height);
    }

    void render_api::clear_colour(const glm::vec3& colour)
    {
        API_DATA.api->clear_colour(colour);
    }

    void render_api::clear()
    {
        API_DATA.api->clear();
    }
} // namespace inx