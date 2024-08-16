#ifndef __INX_VERTEX_ARRAY_H__
#define __INX_VERTEX_ARRAY_H__

#include <memory>                   // for std::shared_ptr

#include "buffer.h"

namespace inx
{
    struct IVertexArray
    {
    public:
        virtual ~IVertexArray() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual void add_vertex_buffer(const std::shared_ptr<IVertexBuffer>& buffer) = 0;
        virtual void set_index_buffer(const std::shared_ptr<IIndexBuffer>& buffer) = 0;

        static std::shared_ptr<IVertexArray> create();
    };
} // namespace inx

#endif // __INX_VERTEX_ARRAY_H__