#include "../opengl.h"

#include <glad/glad.h>

namespace inx
{
    OpenGLVertexArray::OpenGLVertexArray()
        : _buffer_index(0)
    {
        glGenVertexArrays(1, &_id);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        glDeleteVertexArrays(1, &_id);
    }

    void OpenGLVertexArray::bind() const
    {
        glBindVertexArray(_id);
    }

    void OpenGLVertexArray::unbind() const
    {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::add_vertex_buffer(const Ref<VertexBuffer>& buffer)
    {
        glBindVertexArray(_id);
        buffer->bind();

        const auto& layout = buffer->layout();
        for (const auto& element : layout)
        {
            switch(element.data_type)
            {
                case BufferElementDataType::Float2:
                case BufferElementDataType::Float3:
                {
                    glEnableVertexAttribArray(_buffer_index);
                    glVertexAttribPointer(
                        _buffer_index,
                        element.component_count(),
                        GL_FLOAT,
                        GL_FALSE,
                        layout.stride(),
                        (const void*)element.offset
                    );
                    _buffer_index += 1;
                } break;
            }
        }

        _vertex_buffers.push_back(buffer);
    }

    void OpenGLVertexArray::set_index_buffer(const Ref<IndexBuffer>& buffer)
    {
        glBindVertexArray(_id);
        buffer->bind();

        _index_buffer = buffer;
    }
} // namespace inx