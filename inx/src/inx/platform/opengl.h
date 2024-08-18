#ifndef __INX_OPENGL_INTERNAL_H__
#define __INX_OPENGL_INTERNAL_H__

#include "../renderer.h"
#include "../renderer/renderer_internal.h"
#include "../resources.h"

namespace inx
{
    struct OpenGLRenderAPI : public RenderAPI
    {
    public:
        virtual void init() const override;
        
        virtual void viewport(i32 width, i32 height) const override;

        virtual void clear_colour(const glm::vec3& colour) const override;
        virtual void clear() const override;
    };

    struct OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(float* vertices, u32 size);
        OpenGLVertexBuffer(u32 size);

        virtual ~OpenGLVertexBuffer();

        virtual void bind() const override;
        virtual void unbind() const override;

        virtual const BufferLayout& layout() const override { return _layout; }
        virtual void layout(const BufferLayout& layout) override { _layout = layout; }
        
        virtual void data(const void* data, u32 size) override;

    private:
        u32 _id;
        BufferLayout _layout;
    };

    struct OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(u32* indices, u32 count);
        virtual ~OpenGLIndexBuffer();

        virtual void bind() const override;
        virtual void unbind() const override;

        virtual uint32_t count() const override { return _count; };

    private:
        u32 _id;
        u32 _count;
    };

    struct OpenGLVertexArray : public VertexArray
    {
    public:
        OpenGLVertexArray();
        virtual ~OpenGLVertexArray();

        virtual void bind() const override;
        virtual void unbind() const override;

        virtual void add_vertex_buffer(const Ref<VertexBuffer>& buffer) override;
        virtual void set_index_buffer(const Ref<IndexBuffer>& buffer) override;

    private:
        u32 _id;
        u32 _buffer_index;

        std::vector<Ref<VertexBuffer>> _vertex_buffers;
        Ref<IndexBuffer> _index_buffer;
    };

    struct OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::filesystem::path& vertex_filepath, const std::filesystem::path& fragment_filepath);
        ~OpenGLShader();

        virtual void bind() const override;

        virtual void set_int(const std::string& name, int i) const override;
        virtual void set_ints(const std::string& name, int* ints, u32 count) const override;
        virtual void set_float(const std::string& name, float f) const override;
        virtual void set_vec3(const std::string& name, const glm::vec3& vec) const override;
        virtual void set_mat4(const std::string& name, const glm::mat4& mat) const override;

    private:
        u32 _id;
    };

    struct OpenGLTexture : public Texture
    {
    public:
        OpenGLTexture(const std::filesystem::path& texture_filepath);
        OpenGLTexture(const TextureSpec& spec);

        void bind(unsigned int slot = 0) const override;

        virtual const TextureSpec& spec() const override { return _spec; }

        virtual void data(const void* data, u32 size) override;

    private:
        u32 _id;
        int _width;
        int _height;

        TextureSpec _spec;

        u32 _format;
    };
} // namespace inx

#endif // __INX_OPENGL_INTERNAL_H__