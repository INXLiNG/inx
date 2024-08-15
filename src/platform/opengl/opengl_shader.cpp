#include "opengl_shader.h"

/// @brief Helper function to load a shader based on a filepath 
GLuint _load_shader(const std::filesystem::path& path, GLenum type)
{
    std::string code;
    { // obtaining the actual code we care about
        std::ifstream fstream;
        fstream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fstream.open(path.string());

        std::stringstream sstream;
        sstream << fstream.rdbuf();

        fstream.close();

        code = sstream.str();
    }

    // gl functions require a c string
    const char* code_str = code.c_str();

    // create the actual shader program
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &code_str, NULL);
    glCompileShader(shader);

    // check for errors
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    { // error found; find info and throw exception
        GLint max_length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);
        
        std::vector<GLchar> log(max_length);
        glGetShaderInfoLog(shader, 1024, NULL, log.data());

        throw std::runtime_error(log.data());
    }

    // if no error found, we just return the shader id
    return shader;
}

namespace inx
{
    OpenGLShader::OpenGLShader(const path& vertex_filepath, const path& fragment_filepath)
    {
        // load/compile our base shaders
        auto vert_id = _load_shader(vertex_filepath, GL_VERTEX_SHADER);
        auto frag_id = _load_shader(fragment_filepath, GL_FRAGMENT_SHADER);
        
        // create our shader program
        _id = glCreateProgram();
        glAttachShader(_id, vert_id);
        glAttachShader(_id, frag_id);
        glLinkProgram(_id);
        
        // check for errors
        int success;
        glGetProgramiv(_id, GL_LINK_STATUS, &success);
        if (!success)
        { // error found; find info and throw exception
            GLint max_length;
            glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &max_length);
            
            std::vector<GLchar> log(max_length);
            glGetProgramInfoLog(_id, 1024, NULL, log.data());

            glDeleteShader(vert_id);
            glDeleteShader(frag_id);
            
            throw std::runtime_error(log.data());
        }

        // cleanup shaders
        glDetachShader(_id, vert_id);
        glDeleteShader(vert_id);
        glDetachShader(_id, frag_id);
        glDeleteShader(frag_id);
    }

    void OpenGLShader::bind() const
    {
        glUseProgram(_id);
    }
} // namespace inx