#ifndef __INX_TEXTURE_H__
#define __INX_TEXTURE_H__

#include <filesystem>   // used for std::filesystem::path

#include "resources.h"

namespace inx
{
    struct Texture : public IResource
    {
    public:
        virtual ~Texture() = default;
        static std::unique_ptr<Texture> load(const std::filesystem::path& texture_filepath);

        virtual void bind(unsigned int slot = 0) const = 0;
    };
} // namespace inx

#endif // __INX_TEXTURE_H__