#include "buffer.h"

#include "../platform/opengl/opengl_buffer.h"

namespace inx
{
    constexpr uint32_t BufferElementDataTypeSize(BufferElementDataType type)
    {
        switch(type)
        {
            case BufferElementDataType::Float2:     return 4 * 2;
            case BufferElementDataType::Float3:     return 4 * 3;
        }

        return 0;
    }

    BufferElement::BufferElement(const std::string&name, BufferElementDataType data_type)
        : name(name), data_type(data_type), size(BufferElementDataTypeSize(data_type)), offset(0)
    {

    }

    uint32_t BufferElement::get_component_count() const
    {
        switch(data_type)
        {
            case BufferElementDataType::Float2:     return 2;
            case BufferElementDataType::Float3:     return 3;
        }

        return 0;
    }

    BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements)
        : _elements(elements)
    {
        _stride = 0;
        for (auto& element : _elements)
        {
            element.offset = _stride;
            _stride += element.size;
        }
    }

    std::shared_ptr<IVertexBuffer> IVertexBuffer::create(float* vertices, uint32_t size)
    {
        // NOTE(selina): In the future this will change what it returns based on current rendering API - 16/08
        auto result = std::make_shared<OpenGLVertexBuffer>(vertices, size);
        return result;
    }

    std::shared_ptr<IIndexBuffer> IIndexBuffer::create(uint32_t* indices, uint32_t count)
    {
        // NOTE(selina): In the future this will change what it returns based on current rendering API - 16/08
        auto result = std::make_shared<OpenGLIndexBuffer>(indices, count);
        return result;
    }
} // namespace inx