#ifndef __INX_TEXTURE_H__
#define __INX_TEXTURE_H__

#include <filesystem>   // used for std::filesystem::path

#include "resources.h"

namespace inx
{
    struct TextureParams
    {
        std::filesystem::path texture_filepath;
    };

    struct Texture : public IResource
    {
    public:
        Texture(const TextureParams& params);

        /// @brief Set the current shader to be used by OpenGL
        inline void use() const noexcept
        {
            glUseProgram(_id);
        }

    private:
        int _width;
        int _height;
        unsigned char* _pixels;
    };
} // namespace inx

#endif // __INX_TEXTURE_H__