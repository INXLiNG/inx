#include "../renderer.h"

#include "../platform/opengl.h"

namespace inx
{
    constexpr u32 BufferElementDataTypeSize(BufferElementDataType type)
    {
        switch(type)
        {
            case BufferElementDataType::Float2:     return 4 * 2;
            case BufferElementDataType::Float3:     return 4 * 3;
        }

        return 0;
    }

    BufferElement::BufferElement(const std::string& name, BufferElementDataType data_type)
        : name(name), data_type(data_type), size(BufferElementDataTypeSize(data_type)), offset(0)
    {

    }

    u32 BufferElement::component_count() const
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

    Ref<VertexArray> VertexArray::create()
    {
        // NOTE(selina): In the future this will change what it returns based on current rendering API - 16/08
        auto result = std::make_shared<OpenGLVertexArray>();
        return result;
    }

    Ref<VertexBuffer> VertexBuffer::create(float* vertices, u32 size)
    {
        // NOTE(selina): In the future this will change what it returns based on current rendering API - 16/08
        auto result = std::make_shared<OpenGLVertexBuffer>(vertices, size);
        return result;
    }

    Ref<IndexBuffer> IndexBuffer::create(u32* indices, u32 count)
    {
        // NOTE(selina): In the future this will change what it returns based on current rendering API - 16/08
        auto result = std::make_shared<OpenGLIndexBuffer>(indices, count);
        return result;
    }
} // namespace inx