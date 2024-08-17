#include "../resources.h"

#include "../platform/opengl.h"

namespace inx
{
    Scope<Texture> Texture::load(const std::filesystem::path& texture_filepath)
    {
        // NOTE(selina): In the future this will change what it returns based on current rendering API - 15/08
        auto result = create_scope<OpenGLTexture>(texture_filepath);
        return result;
    }
} // namespace inx