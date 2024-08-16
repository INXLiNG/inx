#ifndef __INX_SHADER_H__
#define __INX_SHADER_H__

#include <filesystem>   // used for std::filesystem::path

#include "resources.h"
#include "../math/math.h"

namespace inx
{
    using path = std::filesystem::path;

    struct Shader : public IResource
    {
    public:
        virtual ~Shader() = default;

        virtual void bind() const = 0;

        virtual void set_mat4(const std::string& name, const glm::mat4& mat) const = 0;

        static std::unique_ptr<Shader> load(const path& vertex_filepath, const path& fragment_filepath);
    };
} // namespace inx

#endif // __INX_SHADER_H__