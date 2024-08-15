#ifndef __INX_OPENGL_SHADER__
#define __INX_OPENGL_SHADER__

#include <glad/glad.h>

#include <fstream>
#include <sstream>

#include "../../resources/shader.h"

namespace inx
{
    struct OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const path& vertex_filepath, const path& fragment_filepath);

        void bind() const override;

    private:
        GLuint _id;
    };
} // namespace inx

#endif // __INX_OPENGL_SHADER__