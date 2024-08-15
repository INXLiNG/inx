#include "texture.h"

#include <stb/stb_image.h>

#include "../platform/opengl/opengl_texture.h"

namespace inx
{
    std::unique_ptr<Texture> Texture::load(const std::filesystem::path& texture_filepath)
    {
        // NOTE(selina): In the future this will change what it returns based on current rendering API - 15/08
        auto result = std::make_unique<OpenGLTexture>(texture_filepath);
        return result;
    }
} // namespace inx