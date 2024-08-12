#include "shader.h"

#include <fstream>  // used for std::ifstream
#include <sstream>  // used for std::stringstream

inx::Shader::Shader(const ShaderParams& params)
{
    // load/compile our base shaders
    auto vert_id = _load_shader(params.vert_filepath, GL_VERTEX_SHADER);
    auto frag_id = _load_shader(params.frag_filepath, GL_FRAGMENT_SHADER);
    
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
        char log[1024];
        glGetProgramInfoLog(_id, 1024, NULL, log);
        
        throw std::runtime_error(log);
    }

    // cleanup shaders
    glDetachShader(_id, vert_id);
    glDetachShader(_id, frag_id);
}

GLuint inx::Shader::_load_shader(const std::filesystem::path& path, GLenum type) const
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
        char log[1024];
        glGetShaderInfoLog(shader, 1024, NULL, log);

        throw std::runtime_error(log);
    }

    // if no error found, we just return the shader id
    return shader;
}