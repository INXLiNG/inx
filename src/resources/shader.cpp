#include "../resources.h"

#include "../platform/opengl.h"

namespace inx
{
    Scope<Shader> Shader::load(const std::filesystem::path& vertex_filepath, const std::filesystem::path& fragment_filepath)
    {
        // NOTE(selina): In the future this will change what it returns based on current rendering API - 15/08
        auto result = create_scope<OpenGLShader>(vertex_filepath, fragment_filepath);
        return result;
    }
} // namespace inx