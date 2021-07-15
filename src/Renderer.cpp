//
// Created by Philip Smith on 10/17/2020.
//

#include <engine/Renderer.h>

#include <engine/OrbitCam.h>
#include <engine/InstanceList.h>
#include <engine/Mesh.h>
#include <engine/sprites.h>
#include <engine/VertexArrayObject.h>
#include <fmt/format.h>
#include <glm/glm.hpp>
#include <iostream>
#include <utils/file_util.h>


namespace engine {
    Renderer::Renderer(entt::registry& registry) {
        m_context_entity = registry.create();
        registry.emplace<RenderContext>(m_context_entity);
    }

    Renderer::Renderer(const entt::entity& context_entity) : m_context_entity(context_entity) {}

    void Renderer::read_config(RenderContext& context, const std::string& filename) {
        // defaults
        context.screen_width = 800;
        context.screen_height = 600;
        context.fovy = 45.0f;
        context.z_near = 0.1f;
        context.z_far = 10000.0f;
        try {
            auto config_json = utils::file::read_json_file(filename);
            if (config_json.contains("width"))
                context.screen_width = config_json["width"];
            if (config_json.contains("height"))
                context.screen_height = config_json["height"];
            if(config_json.contains("fovy"))
                context.fovy = config_json["fovy"];
            if(config_json.contains("z_near"))
                context.z_near = config_json["z_near"];
            if(config_json.contains("z_far"))
                context.z_far = config_json["z_far"];
        } catch (nlohmann::detail::parse_error& e) {
            std::cout << "Error reading config file: " << e.what() << std::endl;
        }
    }

    bool Renderer::init(entt::registry &registry) {
        auto& context = registry.get<RenderContext>(m_context_entity);

        read_config(context, "../res/renderer.json");

        if(!init_glfw()) {
            std::cerr << "Failed to init GLFW" << std::endl;
            return false;
        }
        if(!init_window(context)) {
            std::cerr << "Failed to init window" << std::endl;
            return false;
        }
        if(!init_glew()) {
            std::cerr << "Failed to init glew" << std::endl;
            return false;
        }
        if(!init_shaders(context)) {
            std::cerr << "Failed to init shaders" << std::endl;
            return false;
        }

        context.camera = nullptr;

        // cull triangles facing away from camera
        glEnable(GL_CULL_FACE);

        // enable depth buffer
        glEnable(GL_DEPTH_TEST);

        // background
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        registry.on_construct<Mesh>().connect<&load_mesh>();
        registry.on_construct<ShapeSprite>().connect<&load_shape_sprite>();
        registry.on_construct<TextureSprite>().connect<&load_texture_sprite>();
        registry.on_update<Mesh>().connect<&update_mesh>();
        registry.on_update<ShapeSprite>().connect<&update_shape_sprite>();
        registry.on_update<TextureSprite>().connect<&update_texture_sprite>();
        registry.on_destroy<VertexArrayObject>().connect<&destroy_vao>();
        registry.on_destroy<InstanceList>().connect<&destroy_instances>();

        return true;
    }

    bool Renderer::init_glfw() {
        if (!glfwInit())
            return false;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        return true;
    }

    bool Renderer::init_glew() {
        glewExperimental = true; // Needed for core profile
        if (glewInit() != GLEW_OK) {
            glfwTerminate();
            return false;
        }

        return true;
    }

    bool Renderer::init_window(RenderContext& context) {
        // Open a window and create its OpenGL context
        context.window = glfwCreateWindow(context.screen_width, context.screen_height, "Civil War", nullptr, nullptr);
        if (context.window == nullptr) {
            std::cerr << "Failed to open GLFW window" << std::endl;
            glfwTerminate();
            return false;
        }
        glfwMakeContextCurrent(context.window);

        return true;
    }

    bool Renderer::init_shaders(RenderContext& context) {
        auto color_vshader_src = utils::file::read_file_to_string("../res/shaders/vert/color_vert.glsl");
        auto color_vshader_src2d = utils::file::read_file_to_string("../res/shaders/vert/color_vert2d.glsl");
        auto color_fshader_src = utils::file::read_file_to_string("../res/shaders/frag/color_frag.glsl");
        auto tex_vshader_src = utils::file::read_file_to_string("../res/shaders/vert/tex_vert.glsl");
        auto tex_vshader_src2d = utils::file::read_file_to_string("../res/shaders/vert/tex_vert2d.glsl");
        auto tex_fshader_src = utils::file::read_file_to_string("../res/shaders/frag/tex_frag.glsl");

        try {
            context.color_shader2d = load_shader(color_vshader_src2d.c_str(), color_fshader_src.c_str());
        } catch (std::runtime_error& e) {
            std::cout << "2d color shader failed to init: " << e.what() << std::endl;
            return false;
        }
        try {
            context.color_shader3d = load_shader(color_vshader_src.c_str(), color_fshader_src.c_str());
        } catch (std::runtime_error& e) {
            std::cout << "3d color shader failed to init: " << e.what() << std::endl;
            return false;
        }
        try {
            context.tex_shader2d = load_shader(tex_vshader_src2d.c_str(), tex_fshader_src.c_str());
        } catch (std::runtime_error& e) {
            std::cout << "2d texture shader failed to init: " << e.what() << std::endl;
            return false;
        }
        try {
            context.tex_shader3d = load_shader(tex_vshader_src.c_str(), tex_fshader_src.c_str());
        } catch (std::runtime_error& e) {
            std::cout << "3d texture shader failed to init: " << e.what() << std::endl;
            return false;
        }

        return true;
    }

    void Renderer::load_mesh(entt::registry &registry, entt::entity entity) {
        auto sizeof_vec4 = sizeof(glm::vec4);
        auto sizeof_vec3 = sizeof(glm::vec3);
        auto mesh = registry.get<Mesh>(entity);
        // setup vertex array object
        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // verts
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof_vec3 * mesh.vertices.size(), &mesh.vertices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        // colors
        GLuint cbo;
        glGenBuffers(1, &cbo);
        glBindBuffer(GL_ARRAY_BUFFER, cbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof_vec3 * mesh.colors.size(), &mesh.colors[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        // indices
        GLuint ebo;
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh.indices.size(), &mesh.indices[0],
                     GL_STATIC_DRAW);

        // instance buffer (initially empty)
        GLuint ibo;
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ARRAY_BUFFER, ibo);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof_vec4, (void *) 0);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof_vec4, (void *) (1 * sizeof_vec4));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof_vec4, (void *) (2 * sizeof_vec4));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof_vec4, (void *) (3 * sizeof_vec4));
        glVertexAttribDivisor(2, 1);
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);

        registry.emplace<VertexArrayObject>(
                entity, // id
                vao, // Vertex Array Object
                vbo, // Vertex Buffer Object
                ebo, // Element Buffer Object
                static_cast<unsigned int>(mesh.indices.size()), // Number of indices
                cbo // Color Buffer Object
        );
        registry.emplace<InstanceList>(entity, ibo);
        glBindVertexArray(0);
    }

    void Renderer::load_shape_sprite(entt::registry &registry, entt::entity entity) {
        auto sizeof_vec4 = sizeof(glm::vec4);
        auto sprite = registry.get<ShapeSprite>(entity);
        // setup vertex array object
        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // verts
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * sprite.vertices.size(), &sprite.vertices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        // colors
        GLuint cbo;
        glGenBuffers(1, &cbo);
        glBindBuffer(GL_ARRAY_BUFFER, cbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * sprite.colors.size(), &sprite.colors[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        // indices
        GLuint ebo;
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * sprite.indices.size(), &sprite.indices[0],
                     GL_STATIC_DRAW);

        // instance buffer (initially empty)
        GLuint ibo;
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ARRAY_BUFFER, ibo);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof_vec4, (void *) 0);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof_vec4, (void *) (1 * sizeof_vec4));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof_vec4, (void *) (2 * sizeof_vec4));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof_vec4, (void *) (3 * sizeof_vec4));
        glVertexAttribDivisor(2, 1);
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);

        registry.emplace<VertexArrayObject>(
                entity, // id
                vao, // Vertex Array Object
                vbo, // Vertex Buffer Object
                ebo, // Element Buffer Object
                static_cast<unsigned int>(sprite.indices.size()), // Number of indices
                cbo // Color Buffer Object
        );
        registry.emplace<InstanceList>(entity, ibo);
        glBindVertexArray(0);
    }

    void Renderer::load_texture_sprite(entt::registry &registry, entt::entity entity) {
        auto sizeof_vec2 = sizeof(glm::vec2);
        auto sizeof_vec4 = sizeof(glm::vec4);
        auto sprite = registry.get<TextureSprite>(entity);
        // setup vertex array object
        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // verts
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof_vec2 * sprite.vertices.size(), &sprite.vertices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        // uvs
        GLuint uvs;
        glGenBuffers(1, &uvs);
        glBindBuffer(GL_ARRAY_BUFFER, uvs);
        glBufferData(GL_ARRAY_BUFFER, sizeof_vec2 * sprite.uvs.size(), &sprite.uvs[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        // indices
        GLuint ebo;
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * sprite.indices.size(), &sprite.indices[0],
                     GL_STATIC_DRAW);

        // texture sampler
        GLuint tex_id = utils::file::read_png_file_to_texture(sprite.name);

        // instance buffer (initially empty)
        GLuint ibo;
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ARRAY_BUFFER, ibo);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof_vec4, (void *) 0);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof_vec4, (void *) (1 * sizeof_vec4));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof_vec4, (void *) (2 * sizeof_vec4));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof_vec4, (void *) (3 * sizeof_vec4));
        glVertexAttribDivisor(2, 1);
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);

        registry.emplace<VertexArrayObject>(
                entity, // id
                vao, // Vertex Array Object
                vbo, // Vertex Buffer Object
                ebo, // Element Buffer Object
                static_cast<unsigned int>(sprite.indices.size()), // Number of indices
                static_cast<unsigned int>(0), // Color Buffer Object
                tex_id, // texture object
                uvs // texture uv buffer
        );
        registry.emplace<InstanceList>(entity, ibo);
        glBindVertexArray(0);
    }

    void Renderer::update_mesh(entt::registry &registry, entt::entity entity) {
        auto sizeof_vec3 = sizeof(glm::vec3);
        auto& mesh = registry.get<Mesh>(entity);
        auto& vao = registry.get<VertexArrayObject>(entity);
        glBindVertexArray(vao.id);

        // verts
        glBindBuffer(GL_ARRAY_BUFFER, vao.vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof_vec3 * mesh.vertices.size(), &mesh.vertices[0], GL_STATIC_DRAW);

        // colors
        glBindBuffer(GL_ARRAY_BUFFER, vao.cbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof_vec3 * mesh.colors.size(), &mesh.colors[0], GL_STATIC_DRAW);

        // indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh.indices.size(), &mesh.indices[0],
                     GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    void Renderer::update_shape_sprite(entt::registry &registry, entt::entity entity) {
        auto sizeof_vec3 = sizeof(glm::vec3);
        auto sizeof_vec2 = sizeof(glm::vec2);
        auto& sprite = registry.get<ShapeSprite>(entity);
        auto& vao = registry.get<VertexArrayObject>(entity);
        glBindVertexArray(vao.id);

        // verts
        glBindBuffer(GL_ARRAY_BUFFER, vao.vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof_vec2 * sprite.vertices.size(), &sprite.vertices[0], GL_STATIC_DRAW);

        // colors
        glBindBuffer(GL_ARRAY_BUFFER, vao.cbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof_vec3 * sprite.colors.size(), &sprite.colors[0], GL_STATIC_DRAW);

        // indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * sprite.indices.size(), &sprite.indices[0],
                     GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    void Renderer::update_texture_sprite(entt::registry &registry, entt::entity entity) {
        auto sizeof_vec2 = sizeof(glm::vec2);
        auto& sprite = registry.get<TextureSprite>(entity);
        auto& vao = registry.get<VertexArrayObject>(entity);
        glBindVertexArray(vao.id);

        // verts
        glBindBuffer(GL_ARRAY_BUFFER, vao.vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof_vec2 * sprite.vertices.size(), &sprite.vertices[0], GL_STATIC_DRAW);

        // uvs
        glBindBuffer(GL_ARRAY_BUFFER, vao.tex_uvs);
        glBufferData(GL_ARRAY_BUFFER, sizeof_vec2 * sprite.uvs.size(), &sprite.uvs[0], GL_STATIC_DRAW);

        // indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * sprite.indices.size(), &sprite.indices[0],
                     GL_STATIC_DRAW);

        // texture sampler
        vao.tex_id = utils::file::read_png_file_to_texture(sprite.name);

        glBindVertexArray(0);
    }

    void Renderer::destroy_vao(entt::registry &registry, entt::entity entity) {
        auto vao = registry.get<VertexArrayObject>(entity);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glDeleteVertexArrays(1, &vao.id);
        glDeleteBuffers(1, &vao.vbo);
        glDeleteBuffers(1, &vao.ebo);
        if(vao.cbo)
            glDeleteBuffers(1, &vao.cbo);
        if(vao.tex_id)
            glDeleteTextures(1, &vao.tex_id);
        if(vao.tex_uvs)
            glDeleteBuffers(1, &vao.tex_uvs);
    }

    void Renderer::destroy_instances(entt::registry &registry, entt::entity entity) {
        auto instances = registry.get<InstanceList>(entity);
        glDeleteBuffers(1, &instances.id);
    }

    void Renderer::render(entt::registry &registry) {
        const auto& ctx = registry.get<RenderContext>(m_context_entity);
        GLuint vp_uniform;
        glm::mat4 vp;
        if(ctx.camera != nullptr) {
            // 3D rendering
            vp_uniform = glGetUniformLocation(ctx.color_shader3d, "VP");
            vp = glm::perspective(ctx.fovy, ctx.screen_width / ctx.screen_height, ctx.z_near, ctx.z_far)
                      * ctx.camera->get_view();
            glUseProgram(ctx.color_shader3d);
            glUniformMatrix4fv(vp_uniform, 1, GL_FALSE, &vp[0][0]);
            auto view3d = registry.view<VertexArrayObject, InstanceList, Mesh>();
            for (const auto &entity: view3d) {
                auto[vao, ilist, mesh] = view3d.get(entity);
                if (mesh.visible) {
                    glBindVertexArray(vao.id);
                    glDrawElementsInstanced(ilist.render_strategy, vao.num_indices, GL_UNSIGNED_INT, 0,
                                            ilist.instances.size());
                }
            }
        }

        // 2D rendering (TODO: Add occlusion culling to prevent drawing 3D entities that are covered by 2D elements)
        vp_uniform = glGetUniformLocation(ctx.color_shader2d, "VP");
        vp = glm::ortho(0.f, ctx.screen_width, ctx.screen_height, 0.f);
        glUseProgram(ctx.color_shader2d);
        glUniformMatrix4fv(vp_uniform, 1, GL_FALSE, &vp[0][0]);
        auto color_view = registry.view<VertexArrayObject, InstanceList, ShapeSprite>();
        for(const auto &entity: color_view) {
            auto [vao, ilist, sprite] = color_view.get(entity);
            if(sprite.visible) {
                glBindVertexArray(vao.id);
                glDrawElementsInstanced(ilist.render_strategy, vao.num_indices, GL_UNSIGNED_INT, 0,
                                        ilist.instances.size());
            }
        }
        vp_uniform = glGetUniformLocation(ctx.tex_shader2d, "VP");
        auto tex_uniform  = glGetUniformLocation(ctx.tex_shader2d, "texSampler");
        glUseProgram(ctx.tex_shader2d);
        glUniformMatrix4fv(vp_uniform, 1, GL_FALSE, &vp[0][0]);
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(tex_uniform, 0);
        auto tex_view = registry.view<VertexArrayObject, InstanceList, TextureSprite>();
        for(const auto &entity: tex_view) {
            auto [vao, ilist, sprite] = tex_view.get(entity);
            if(sprite.visible) {
                glBindVertexArray(vao.id);
                glBindTexture(GL_TEXTURE_2D, vao.tex_id);
                glDrawElementsInstanced(ilist.render_strategy, vao.num_indices, GL_UNSIGNED_INT, 0,
                                        ilist.instances.size());
            }
        }
    }

    void Renderer::cleanup(entt::registry &registry) {
        const auto& ctx = registry.get<RenderContext>(m_context_entity);
        glDeleteProgram(ctx.color_shader2d);
        glDeleteProgram(ctx.color_shader3d);
        glDeleteProgram(ctx.tex_shader2d);
        glDeleteProgram(ctx.tex_shader3d);
        glfwTerminate();
    }

    const RenderContext& Renderer::get_context(entt::registry& registry) const {
        return registry.get<RenderContext>(m_context_entity);
    }

    void Renderer::set_camera(entt::registry &registry, Camera::Ptr camera) {
        auto& context = registry.get<RenderContext>(m_context_entity);
        context.camera = std::move(camera);
    }

    GLuint Renderer::load_shader(const char* vertex_source, const char* frag_source) {
        int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vertex_source, nullptr);
        glCompileShader(vertex_shader);
        int success;
        char infoLog[512];
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex_shader, 512, nullptr, infoLog);
            std::string message = fmt::format("Error on vertex compilation ", infoLog);
            throw std::runtime_error(message.c_str());
        }
        int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &frag_source, nullptr);
        glCompileShader(fragment_shader);
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragment_shader, 512, nullptr, infoLog);
            std::string message = fmt::format("Error on fragment compilation ", infoLog);
            throw std::runtime_error(message.c_str());
        }
        auto shader = glCreateProgram();
        glAttachShader(shader, vertex_shader);
        glAttachShader(shader, fragment_shader);
        glLinkProgram(shader);
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 512, nullptr, infoLog);
            std::string message = fmt::format("Error linking program ", infoLog);
            throw std::runtime_error(message.c_str());
        }
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        return shader;
    }
} // namespace engine
