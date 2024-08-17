#ifndef __INX_RENDERER_H__
#define __INX_RENDERER_H__

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "types.h"
#include "resources.h"

namespace inx
{
    namespace render_api
    {
        void init();

        void viewport(i32 width, i32 height);
        void clear_colour(const glm::vec3& colour);
        void clear();
    } // namespace render_api

    namespace render2d
    {
        void init(ResourceManager& manager);
        void shutdown();

        void begin_batch();
        void end_batch();
        void flush();

        void draw_quad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& colour);
    } // namespace render2d

    enum BufferElementDataType
    {
        None = 0, Float, Float2, Float3, Float4
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

        virtual void data(const void* data, u32 size) = 0;

        static Ref<VertexBuffer> create(float* vertices, u32 size);
        static Ref<VertexBuffer> create(u32 size);
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

    class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void projection(float left, float right, float bottom, float top);

		const glm::vec3& position() const { return _position; }
		void position(const glm::vec3& position) { _position = position; recalc_matrix(); }

		float rotation() const { return _rotation; }
		void rotation(float rotation) { _rotation = rotation; recalc_matrix(); }

		const glm::mat4& projection_matrix() const { return _projection_matrix; }
		const glm::mat4& view_matrix() const { return _view_matrix; }
		const glm::mat4& view_projection_matrix() const { return _view_projection_matrix; }

	private:
		glm::mat4 _projection_matrix;
		glm::mat4 _view_matrix;
		glm::mat4 _view_projection_matrix;

		glm::vec3 _position = { 0.0f, 0.0f, 0.0f };
		float _rotation = 0.0f;
	private:
		void recalc_matrix();
	};
} // namespace inx

#endif // __INX_RENDERER_H__