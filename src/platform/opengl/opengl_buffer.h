#ifndef __INX_OPENGL_BUFFER_H__
#define __INX_OPENGL_BUFFER_H__

#include "../../renderer/buffer.h"

namespace inx
{
    struct OpenGLVertexBuffer : public IVertexBuffer
    {
    public:
        OpenGLVertexBuffer(float* vertices, unsigned int size);
        virtual ~OpenGLVertexBuffer();

        virtual void bind() const override;
        virtual void unbind() const override;

        virtual const BufferLayout& get_layout() const override { return _layout; }
        virtual void set_layout(const BufferLayout& layout) override { _layout = layout; }

    private:
        uint32_t _id;
        BufferLayout _layout;
    };

    struct OpenGLIndexBuffer : public IIndexBuffer
    {
    public:
        OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
        virtual ~OpenGLIndexBuffer();

        virtual void bind() const override;
        virtual void unbind() const override;

        virtual uint32_t get_count() const override { return _count; };

    private:
        uint32_t _id;
        uint32_t _count;
    };
} // namespace inx

#endif // __INX_OPENGL_BUFFER_H__