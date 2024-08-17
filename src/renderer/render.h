#ifndef __INX_RENDER_H__
#define __INX_RENDER_H__

#include <glm/glm.hpp>

namespace inx
{
    struct RenderAPI
    {
    public:
        virtual ~RenderAPI() = default;

        virtual void init() const = 0;

        virtual void set_viewport(int width, int height) const = 0;

        virtual void clear_colour(glm::vec3 colour) const = 0;
        virtual void clear() const = 0;
    };

    namespace render_api
    {
        void init();

        void set_viewport(int width, int height);
        void clear_colour(glm::vec3 colour);
        void clear();
    } // namespace render_api
} // namespace inx

#endif // __INX_RENDER_H__