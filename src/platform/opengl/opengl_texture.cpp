#include "opengl_texture.h"

#include <stb/stb_image.h>
#include <iostream>

namespace inx
{
    OpenGLTexture::OpenGLTexture(const std::filesystem::path& texture_filepath)
    {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(texture_filepath.string().c_str(), &width, &height, &channels, 0);

        _width = width;
        _height = height;
        
        glGenTextures(1, &_id);
        glBindTexture(GL_TEXTURE_2D, _id);

        GLenum format = 0;
        switch (channels)
        {
            case 1: format = GL_RED; break;
            case 3: format = GL_RGB; break;
            case 4: format = GL_RGBA; break;
        }

        if (data && format != 0)
        {
            std::cout << "Loaded: " << texture_filepath.string() << "\n";
            glTexImage2D(GL_TEXTURE_2D, 0, format, _width, _height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void OpenGLTexture::bind(unsigned int slot) const
    {
        glActiveTexture(slot);
        glBindTexture(GL_TEXTURE_2D, _id);
    }
} // namespace inx