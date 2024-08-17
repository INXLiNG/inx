#include "../opengl.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

u32 _load_shader(const std::filesystem::path& path, GLenum type)
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
    u32 shader = glCreateShader(type);
    glShaderSource(shader, 1, &code_str, NULL);
    glCompileShader(shader);

    // check for errors
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    { // error found; find info and throw exception
        i32 max_length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);
        
        std::vector<GLchar> log(max_length);
        glGetShaderInfoLog(shader, 1024, NULL, log.data());

        std::cerr << "Could not compile shader: " << log.data() << "\n";
        throw std::runtime_error(log.data());
    }

    // if no error found, we just return the shader id
    return shader;
}

namespace inx
{
    OpenGLShader::OpenGLShader(const std::filesystem::path& vertex_filepath, const std::filesystem::path& fragment_filepath)
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

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(_id);
    }

    void OpenGLShader::bind() const
    {
        glUseProgram(_id);
    }

    void OpenGLShader::set_int(const std::string& name, int i) const
    {
        auto location = glGetUniformLocation(_id, name.c_str());
        glUniform1iv(location, 1, &i);
    }

    void OpenGLShader::set_ints(const std::string& name, int* ints, u32 count) const
    {
        auto location = glGetUniformLocation(_id, name.c_str());
        glUniform1iv(location, count, ints);
    }

    void OpenGLShader::set_float(const std::string& name, float f) const
    {
        auto location = glGetUniformLocation(_id, name.c_str());
        glUniform1fv(location, 1, &f);
    }

    void OpenGLShader::set_vec3(const std::string& name, const glm::vec3& vec) const
    {
        auto location = glGetUniformLocation(_id, name.c_str());
        glUniform3fv(location, 1, &vec[0]);
    }

    void OpenGLShader::set_mat4(const std::string& name, const glm::mat4& mat) const
    {
        auto location = glGetUniformLocation(_id, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
    }
} // namespace inx