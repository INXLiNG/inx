#include "shader.h"

#include "../platform/opengl/opengl_shader.h"

namespace inx
{
    std::unique_ptr<Shader> Shader::load(const path& vertex_filepath, const path& fragment_filepath)
    {
        // NOTE(selina): In the future this will change what it returns based on current rendering API - 15/08
        auto result = std::make_unique<OpenGLShader>(vertex_filepath, fragment_filepath);
        return result;
    }
} // namespace inx