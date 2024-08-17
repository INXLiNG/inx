using namespace inx;

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

#include "renderer.h"
#include "resources.h"

#define PATH(filepath) std::filesystem::path(RES_PATH) / filepath

typedef std::shared_ptr<VertexArray> VAO;

static std::vector<glm::vec3> cube_positions;

static glm::vec3 light_positions[] = {
    glm::vec3( 0.7f,  0.2f,  2.0f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f)
};

struct CubesData
{
    VAO cube_vao;
    VAO light_cube_vao;
};

static CubesData s_CubesData;

void init_cubes(ResourceManager &manager)
{
    for (int x = -25; x < 25; x += 5)
        for (int y = -25; y < 25; y += 5)
            for (int z = -25; z < 25; z += 5)
                cube_positions.push_back(glm::vec3((float)x, (float)y, (float)z));

    std::filesystem::path material_shader_vs = PATH("material.vs");
    std::filesystem::path material_shader_fs = PATH("material.fs");
    manager.load_resource<Shader>("material", material_shader_vs, material_shader_fs);

    std::filesystem::path cube_shader_vs = PATH("light_cube.vs");
    std::filesystem::path cube_shader_fs = PATH("light_cube.fs");
    manager.load_resource<Shader>("light_cube", cube_shader_vs, cube_shader_fs);
    
    std::filesystem::path container_path = PATH("container.png");
    manager.load_resource<Texture>("container", container_path);

    std::filesystem::path container_spec_path = PATH("container_spec.png");
    manager.load_resource<Texture>("container_spec", container_spec_path);

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    auto cube_vbo = VertexBuffer::create(vertices, sizeof(vertices));
    cube_vbo->layout({
        { "position",  BufferElementDataType::Float3 },
        { "normal",    BufferElementDataType::Float3 },
        { "tex_coord", BufferElementDataType::Float2 },
    });

    s_CubesData.cube_vao = VertexArray::create();
    s_CubesData.cube_vao->add_vertex_buffer(cube_vbo);

    s_CubesData.light_cube_vao = VertexArray::create();
    s_CubesData.light_cube_vao->add_vertex_buffer(cube_vbo);
    
    auto& shader = manager.get_resource<Shader>("material");
    shader.bind();
    shader.set_int("material.diffuse", 0);
    shader.set_int("material.specular", 1);
}

void draw_cubes(ResourceManager& manager, PerspectiveCamera& camera, int w, int h, bool rotate_cubes)
{
    auto& lighting_shader = manager.get_resource<Shader>("material");
    lighting_shader.bind();
    lighting_shader.set_vec3("viewPos", camera.position());
    lighting_shader.set_float("material.shininess", 32.f);

    // directional light
    lighting_shader.set_vec3("dirLight.direction", glm::vec3(-.2f, -1.f, -.3f));
    lighting_shader.set_vec3("dirLight.ambient", glm::vec3(.05f, .05f, .05f));
    lighting_shader.set_vec3("dirLight.diffuse", glm::vec3(.4f, .4f, .4f));
    lighting_shader.set_vec3("dirLight.specular", glm::vec3(.5f, .5f, .5f));

    // 1st point light
    lighting_shader.set_vec3("pointLights[0].position", light_positions[0]);
    lighting_shader.set_vec3("pointLights[0].ambient", glm::vec3(.05f, .05f, .05f));
    lighting_shader.set_vec3("pointLights[0].diffuse", glm::vec3(.8f, .8f, .8f));
    lighting_shader.set_vec3("pointLights[0].specular", glm::vec3(1.f, 1.f, 1.f));
    lighting_shader.set_float("pointLights[0].constant", 1.f);
    lighting_shader.set_float("pointLights[0].linear", .09f);
    lighting_shader.set_float("pointLights[0].quadratic", .032f);

    // 2nd point light
    lighting_shader.set_vec3("pointLights[1].position", light_positions[1]);
    lighting_shader.set_vec3("pointLights[1].ambient", glm::vec3(.05f, .05f, .05f));
    lighting_shader.set_vec3("pointLights[1].diffuse", glm::vec3(.8f, .8f, .8f));
    lighting_shader.set_vec3("pointLights[1].specular", glm::vec3(1.f, 1.f, 1.f));
    lighting_shader.set_float("pointLights[1].constant", 1.f);
    lighting_shader.set_float("pointLights[1].linear", .09f);
    lighting_shader.set_float("pointLights[1].quadratic", .032f);

    // 3rd point light
    lighting_shader.set_vec3("pointLights[2].position", light_positions[2]);
    lighting_shader.set_vec3("pointLights[2].ambient", glm::vec3(.05f, .05f, .05f));
    lighting_shader.set_vec3("pointLights[2].diffuse", glm::vec3(.8f, .8f, .8f));
    lighting_shader.set_vec3("pointLights[2].specular", glm::vec3(1.f, 1.f, 1.f));
    lighting_shader.set_float("pointLights[2].constant", 1.f);
    lighting_shader.set_float("pointLights[2].linear", .09f);
    lighting_shader.set_float("pointLights[2].quadratic", .032f);

    // 4th point light
    lighting_shader.set_vec3("pointLights[3].position", light_positions[3]);
    lighting_shader.set_vec3("pointLights[3].ambient", glm::vec3(.05f, .05f, .05f));
    lighting_shader.set_vec3("pointLights[3].diffuse", glm::vec3(.8f, .8f, .8f));
    lighting_shader.set_vec3("pointLights[3].specular", glm::vec3(1.f, 1.f, 1.f));
    lighting_shader.set_float("pointLights[3].constant", 1.f);
    lighting_shader.set_float("pointLights[3].linear", .09f);
    lighting_shader.set_float("pointLights[3].quadratic", .032f);

    // 4th point light
    lighting_shader.set_vec3("spotLight.position", camera.position());
    lighting_shader.set_vec3("spotLight.direction", camera.front());
    lighting_shader.set_vec3("spotLight.ambient", glm::vec3(0.f, 0.f, 0.f));
    lighting_shader.set_vec3("spotLight.diffuse", glm::vec3(1.f, 1.f, 1.f));
    lighting_shader.set_vec3("spotLight.specular", glm::vec3(1.f, 1.f, 1.f));
    lighting_shader.set_float("spotLight.constant", 1.f);
    lighting_shader.set_float("spotLight.linear", .09f);
    lighting_shader.set_float("spotLight.quadratic", .032f);
    lighting_shader.set_float("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    lighting_shader.set_float("spotLight.outerCutOff", glm::cos(glm::radians(15.f)));

    auto proj = glm::perspective(glm::radians(camera.fov()), (float)(w / h), .1f, 100.f);
    auto view = camera.view_matrix();
    lighting_shader.set_mat4("projection", proj);
    lighting_shader.set_mat4("view", view);

    glm::mat4 model = glm::mat4(1.f);
    lighting_shader.set_mat4("model", model);

    manager.get_resource<Texture>("container").bind(GL_TEXTURE0);
    manager.get_resource<Texture>("container_spec").bind(GL_TEXTURE1);

    s_CubesData.cube_vao->bind();
    for (unsigned int i = 0; i < cube_positions.size(); i++)
    {
        float angle = rotate_cubes ? (20.f * i) + (SDL_GetTicks() / 10.f) : 0;

        model = glm::mat4(1.f);
        model = glm::translate(model, cube_positions[i]);
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.f, .3f, .5f));
        lighting_shader.set_mat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    auto& cube_shader = manager.get_resource<Shader>("light_cube");
    cube_shader.bind();
    cube_shader.set_mat4("projection", proj);
    cube_shader.set_mat4("view", view);

    s_CubesData.light_cube_vao->bind();
    for (unsigned int i = 0; i < 4; i++)
    {
        model = glm::mat4(1.f);
        model = glm::translate(model, light_positions[i]);
        model = glm::scale(model, glm::vec3(.2f));
        cube_shader.set_mat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}