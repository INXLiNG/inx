#include "vertex_array.h"

#include "../platform/opengl/opengl_vertex_array.h"

namespace inx
{
    std::shared_ptr<IVertexArray> IVertexArray::create()
    {
        // NOTE(selina): In the future this will change what it returns based on current rendering API - 16/08
        auto result = std::make_shared<OpenGLVertexArray>();
        return result;
    }
} // namespace inx