#ifndef __INX_RENDERER_INTERNAL_H__
#define __INX_RENDERER_INTERNAL_H__

#include "../renderer.h"

namespace inx
{
    struct RenderAPI
    {
        virtual ~RenderAPI() = default;
        virtual void init() const = 0;

        virtual void viewport(i32 width, i32 height) const = 0;
        virtual void clear_colour(const glm::vec3&  colour) const = 0;
        virtual void clear() const = 0;
    };
} // namespace inx

#endif // __INX_RENDERER_INTERNAL_H__