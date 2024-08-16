#ifndef __INX_OPENGL_TEXTURE__
#define __INX_OPENGL_TEXTURE__

#include <glad/glad.h>

#include "../../resources/texture.h"

namespace inx
{
    struct OpenGLTexture : public Texture
    {
    public:
        OpenGLTexture(const std::filesystem::path& texture_filepath);

        void bind(unsigned int slot = 0) const override;

    private:
        GLuint _id;

        int _width;
        int _height;
    };
} // namespace inx

#endif // __INX_OPENGL_TEXTURE__