#include "../opengl.h"

#include <glad/glad.h>
#include <stb/stb_image.h>
#include <iostream>

namespace inx
{
    static u32 convert(ImageFormat format)
    {
        switch(format)
        {
            case ImageFormat::RGB8:     return GL_RGB8;
            case ImageFormat::RGBA8:    return GL_RGBA8;
        }

        return 0;
    }

    OpenGLTexture::OpenGLTexture(const std::filesystem::path& texture_filepath)
    {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(texture_filepath.string().c_str(), &width, &height, &channels, 0);

        _width = width;
        _height = height;
        
        glGenTextures(1, &_id);
        glBindTexture(GL_TEXTURE_2D, _id);

        switch (channels)
        {
            case 1: _format = GL_RED; break;
            case 3: _format = GL_RGB; break;
            case 4: _format = GL_RGBA; break;
        }

        if (data && _format != 0)
        {
            std::cout << "Loaded: " << texture_filepath.string() << "\n";
            glTexImage2D(GL_TEXTURE_2D, 0, _format, _width, _height, 0, _format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    OpenGLTexture::OpenGLTexture(const TextureSpec& spec)
        : _spec(spec), _width(spec.width), _height(spec.height)
    {
        _format = convert(spec.format);

        glGenTextures(1, &_id);
        glBindTexture(GL_TEXTURE_2D, _id);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void OpenGLTexture::bind(unsigned int slot) const
    {
        glActiveTexture(slot);
        glBindTexture(GL_TEXTURE_2D, _id);
    }

    void OpenGLTexture::data(const void* data, u32 size)
    {
        glBindTexture(GL_TEXTURE_2D, _id);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height, _format, GL_UNSIGNED_BYTE, data);
    }
} // namespace inx