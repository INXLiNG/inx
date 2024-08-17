#ifndef __INX_RENDERER_H__
#define __INX_RENDERER_H__

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "types.h"

namespace inx
{
    namespace render_api
    {
        void init();

        void viewport(i32 width, i32 height);
        void clear_colour(const glm::vec3& colour);
        void clear();
    } // namespace render_api

    enum BufferElementDataType
    {
        None = 0, Float2, Float3,
    };

    static constexpr u32 BufferElementDataTypeSize(BufferElementDataType type);

    struct BufferElement
    {
        std::string name;
        BufferElementDataType data_type;
        u32 size;
        size_t offset;

        BufferElement(const std::string& name, BufferElementDataType data_type);
        u32 component_count() const;
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

        u32 stride() const { return _stride; }
    
    private:
        std::vector<BufferElement> _elements;
        u32 _stride;
    };

    struct VertexBuffer
    {
        virtual ~VertexBuffer() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual const BufferLayout& layout() const = 0;
        virtual void layout(const BufferLayout& layout) = 0;

        static Ref<VertexBuffer> create(float* vertices, u32 size);
    };

    struct IndexBuffer
    {
        virtual ~IndexBuffer() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual u32 count() const = 0;

        static Ref<IndexBuffer> create(u32* indices, u32 count);
    };

    struct VertexArray
    {
        virtual ~VertexArray() = default;
        
        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual void add_vertex_buffer(const Ref<VertexBuffer>& vbo) = 0;
        virtual void set_index_buffer(const Ref<IndexBuffer>& ebo) = 0;

        static Ref<VertexArray> create();
    };

    struct PerspectiveCamera
    {
    public:
        enum Direction
        {
            Forward, Backward, Left, Right, Up, Down,
        };

        PerspectiveCamera(const glm::vec3& position);

        glm::mat4 view_matrix();

        void position(Direction direction, float dt);
        void pov(float dx, float dy);
        void fov(float dy);

        const glm::vec3& position() const { return _position; }
        const glm::vec3& front() const { return _front; }
        float fov() const { return _fov; } 

    private:
        glm::vec3 _position;
        glm::vec3 _front;
        glm::vec3 _right;
        glm::vec3 _up;

        float _yaw      = -90.f;
        float _pitch    = 0.f;
        float _fov      = 45.f;
    
    private:
        void update_vectors();
    };
} // namespace inx

#endif // __INX_RENDERER_H__