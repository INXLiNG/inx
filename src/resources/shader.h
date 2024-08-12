#ifndef __INX_SHADER_H__
#define __INX_SHADER_H__

#include <filesystem>   // used for std::filesystem::path
#include <glad/glad.h>  // used for OpenGL

#include "resources.h"

namespace inx
{
    struct ShaderParams
    {
        std::filesystem::path vert_filepath;
        std::filesystem::path frag_filepath;
    };

    struct Shader : public Resource
    {
    public:
        Shader(const ShaderParams& params);

        /// @brief Set the current shader to be used by OpenGL
        inline void use() const noexcept
        {
            glUseProgram(_id);
        }

    private:
        /// @brief The program id of the shader; used for enabling program
        GLuint _id;

        /// @brief Helper function to load shaders from disk.
        /// @param path The filepath of the shader to load (e.g. ~/my/shader/file.vs)
        /// @param type The kind of shader (vertex, fragment, geometry) we're loading
        /// @return The id of the shader loaded; throws runtime_error if compilation or file reading failed
        GLuint _load_shader(const std::filesystem::path& path, GLenum type) const;
    };
} // namespace inx

#endif // __INX_SHADER_H__