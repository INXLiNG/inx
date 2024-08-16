#ifndef __INX_OPENGL_VERTEX_ARRAY_H__
#define __INX_OPENGL_VERTEX_ARRAY_H__

#include <cstdint>
#include <vector>                   // for std::vector

#include "../../renderer/vertex_array.h"

namespace inx
{
    struct OpenGLVertexArray : public IVertexArray
    {
    public:
        OpenGLVertexArray();
        virtual ~OpenGLVertexArray();

        virtual void bind() const override;
        virtual void unbind() const override;

        virtual void add_vertex_buffer(const std::shared_ptr<IVertexBuffer>& buffer) override;
        virtual void set_index_buffer(const std::shared_ptr<IIndexBuffer>& buffer) override;

    private:
        uint32_t _id;
        uint32_t _buffer_index;

        std::vector<std::shared_ptr<IVertexBuffer>> _vertex_buffers;
        std::shared_ptr<IIndexBuffer> _index_buffer;
    };
} // namespace inx

#endif // __INX_OPENGL_VERTEX_ARRAY_H__