#include "../renderer.h"

#include <array>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

namespace inx
{
    struct QuadVertex
    {
        glm::vec3 position;
        glm::vec4 colour;
        
        glm::vec2 tex_coord;
        float tex_index;
    };

    struct Render2DData
    {
        static constexpr u32 MAX_QUADS          = 20000;
        static constexpr u32 MAX_VERTICES       = MAX_QUADS * 4;
        static constexpr u32 MAX_INDICES        = MAX_QUADS * 6;
        static constexpr u32 MAX_TEXTURE_SLOTS  = 32;

        Ref<VertexArray> quad_vao;
        Ref<VertexBuffer> quad_vbo;

        std::array<u32, MAX_TEXTURE_SLOTS> texture_slots;
        u32 texture_slot_index = 1;

        QuadVertex* vertices = nullptr;
        QuadVertex* vertices_ptr = nullptr;
        
        u32 index_count = 0;

        glm::vec4 quad_positions[4];
    };
    
    static Render2DData RENDER_DATA;

    void render2d::init(ResourceManager& manager)
    {
        if (RENDER_DATA.quad_vao) return;

        RENDER_DATA.quad_vao = VertexArray::create();

        RENDER_DATA.quad_vbo = VertexBuffer::create(RENDER_DATA.MAX_VERTICES * sizeof(QuadVertex));
        RENDER_DATA.quad_vbo->layout({
            { "position",  BufferElementDataType::Float3 },
            { "colour",    BufferElementDataType::Float4 },
            { "tex_coord", BufferElementDataType::Float2 },
            { "tex_index", BufferElementDataType::Float },
        });

        RENDER_DATA.quad_vao->add_vertex_buffer(RENDER_DATA.quad_vbo);

        RENDER_DATA.vertices = new QuadVertex[RENDER_DATA.MAX_VERTICES];

        auto indices = new u32[RENDER_DATA.MAX_INDICES];
        u32 offset = 0;
        for (size_t i = 0; i < RENDER_DATA.MAX_INDICES; i += 6)
        {
            // triangle #1
            indices[i + 0] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;

            // triangle #2
            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 0;

            offset += 4;
        }

        auto ebo = IndexBuffer::create(indices, RENDER_DATA.MAX_INDICES);
        RENDER_DATA.quad_vao->set_index_buffer(ebo);
        delete[] indices;

        TextureSpec spec;
        manager.load_resource<Texture>("r2d_white", spec);
        u32 texture_data = 0xffffffff;
        manager.get_resource<Texture>("r2d_white").data(&texture_data, sizeof(u32));

        RENDER_DATA.quad_positions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		RENDER_DATA.quad_positions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		RENDER_DATA.quad_positions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		RENDER_DATA.quad_positions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
    }

    void render2d::shutdown()
    {
        delete[] RENDER_DATA.vertices;
    }

    void render2d::begin_batch()
    {
        RENDER_DATA.index_count = 0;
        RENDER_DATA.vertices_ptr = RENDER_DATA.vertices;
    }

    void render2d::end_batch()
    {
        u32 size = (u32)((u8*)RENDER_DATA.vertices_ptr - (u8*)RENDER_DATA.vertices);
        RENDER_DATA.quad_vbo->data(RENDER_DATA.vertices, size);
    }

    void render2d::flush()
    {
        RENDER_DATA.quad_vao->bind();
        glDrawElements(GL_TRIANGLES, RENDER_DATA.index_count, GL_UNSIGNED_INT, nullptr);
    }

    void render2d::draw_quad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& colour)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.f), position) * glm::scale(glm::mat4(1.f), { size.x, size.y, 1.f });

        if (RENDER_DATA.index_count >= RENDER_DATA.MAX_INDICES)
        {
            render2d::end_batch();
            render2d::flush();
            render2d::begin_batch();
        }

        constexpr size_t quad_count = 4;
        constexpr float texture_index = 0.0f;
        constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

        for (size_t i = 0; i < quad_count; i++)
        {
            RENDER_DATA.vertices_ptr->position = transform * RENDER_DATA.quad_positions[i];
            RENDER_DATA.vertices_ptr->colour = colour;
            RENDER_DATA.vertices_ptr->tex_coord = textureCoords[i];
            RENDER_DATA.vertices_ptr->tex_index = texture_index; 
            RENDER_DATA.vertices_ptr++;
        }

        RENDER_DATA.index_count += 6;
    }
} // namespace inx