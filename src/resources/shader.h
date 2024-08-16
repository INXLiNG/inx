#ifndef __INX_SHADER_H__
#define __INX_SHADER_H__

#include <filesystem>               // for std::filesystem::path

#include <glm/glm.hpp>

#include "resources.h"

namespace inx
{
    using path = std::filesystem::path;

    struct Shader : public IResource
    {
    public:
        virtual ~Shader() = default;

        virtual void bind() const = 0;

        virtual void set_int(const std::string& name, int i) const = 0;
        virtual void set_float(const std::string& name, float f) const = 0;
        virtual void set_vec3(const std::string& name, const glm::vec3& vec) const = 0;
        virtual void set_mat4(const std::string& name, const glm::mat4& mat) const = 0;

        static std::unique_ptr<Shader> load(const path& vertex_filepath, const path& fragment_filepath);
    };
} // namespace inx

#endif // __INX_SHADER_H__