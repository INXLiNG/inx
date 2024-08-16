#ifndef __INX_BUFFER_H__
#define __INX_BUFFER_H__

#include <vector>                   // for std::vector, std::initializer_list
#include <memory>                   // for std::shared_ptr
#include <cstdint>                  // for uint32_t
#include <string>                   // for std::string

namespace inx
{
    enum class BufferElementDataType
    {
        None = 0,
        Float2,
        Float3,
    };

    static constexpr uint32_t BufferElementDataTypeSize(BufferElementDataType type);

    struct BufferElement
    {
        std::string name;
        BufferElementDataType data_type;
        uint32_t size;
        size_t offset;

        BufferElement(const std::string& name, BufferElementDataType data_type);

        uint32_t get_component_count() const;
    };

    struct BufferLayout
    {
    public:
        BufferLayout() = default;
        BufferLayout(std::initializer_list<BufferElement> elements);

        std::vector<BufferElement>::iterator begin() { return _elements.begin(); }
        std::vector<BufferElement>::iterator end() { return _elements.end(); }

        std::vector<BufferElement>::const_iterator begin() const { return _elements.begin(); }
        std::vector<BufferElement>::const_iterator end() const { return _elements.end(); }

        uint32_t get_stride() const { return _stride; }
    
    private:
        std::vector<BufferElement> _elements;
        uint32_t _stride;
    };

    struct IVertexBuffer
    {
    public:
        virtual ~IVertexBuffer() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual const BufferLayout& get_layout() const = 0;
        virtual void set_layout(const BufferLayout& layout) = 0;
        
        static std::shared_ptr<IVertexBuffer> create(float* vertices, uint32_t size);
    };

    struct IIndexBuffer
    {
    public:
        virtual ~IIndexBuffer() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual uint32_t get_count() const = 0;
        
        static std::shared_ptr<IIndexBuffer> create(uint32_t* indices, uint32_t count);
    };
} // namespace inx

#endif // __INX_BUFFER_H__