#include "opengl_texture.h"

#include <stb/stb_image.h>

namespace inx
{
    OpenGLTexture::OpenGLTexture(const std::filesystem::path& texture_filepath)
    {
        int width, height, channels;
        unsigned char* data = stbi_load(texture_filepath.string().c_str(), &width, &height, &channels, 0);

        _width = width;
        _height = height;

        if (!data)
        {
            throw std::runtime_error(texture_filepath.string());
        }

        glGenTextures(1, &_id);
        glBindTexture(GL_TEXTURE_2D, _id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }

    void OpenGLTexture::bind(unsigned int slot) const
    {
        glActiveTexture(slot);
        glBindTexture(GL_TEXTURE_2D, _id);
    }
} // namespace inx