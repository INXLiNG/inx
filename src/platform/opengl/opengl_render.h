#ifndef __INX_OPENGL_RENDER_H__
#define __INX_OPENGL_RENDER_H__

#include "../../renderer/render.h"

namespace inx
{
    struct OpenGLRenderAPI : public RenderAPI
    {
    public:
        virtual void init() const override;
        
        virtual void set_viewport(int width, int height) const override;

        virtual void clear_colour(glm::vec3 colour) const override;
        virtual void clear() const override;
    };
} // namespace inx

#endif // __INX_OPENGL_RENDER_H__