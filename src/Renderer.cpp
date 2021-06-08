//
// Created by Philip Smith on 10/17/2020.
//

#include <engine/Renderer.h>

#include <engine/Camera.h>
#include <engine/InputManager.h>
#include <engine/InstanceList.h>
#include <engine/Sprite.h>
#include <engine/Mesh.h>
#include <engine/VertexArrayObject.h>
#include <glm/glm.hpp>
#include <iostream>
#include <utils/file_util.h>


namespace engine {
    bool Renderer::init(entt::registry &registry) {
        read_config("../res/renderer.json");

        assert(init_glfw());
        assert(init_window(m_width, m_height));
        assert(init_glew());
        assert(init_shaders());

        // cull triangles facing away from camera
        glEnable(GL_CULL_FACE);

        // enable depth buffer
        glEnable(GL_DEPTH_TEST);

        // background
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        registry.on_construct<Mesh>().connect<&load_mesh>();
        registry.on_construct<Sprite>().connect<&load_sprite>();
        registry.on_destroy<VertexArrayObject>().connect<&destroy_vao>();
        registry.on_destroy<InstanceList>().connect<&destroy_instances>();

        // create camera
        m_camera_entity = registry.create();
        registry.emplace<Camera>(m_camera_entity);
        auto &camera = registry.get<Camera>(m_camera_entity);
        camera.filming = true;
        camera.position = glm::vec3(10, 10, 10);

        return m_window;
    }

    bool Renderer::init_window(int width, int height) {
        // Open a window and create its OpenGL context
        m_window = glfwCreateWindow(width, height, "Civil War", nullptr, nullptr);
        if (m_window == nullptr) {
            std::cerr << "Failed to open GLFW window" << std::endl;
            glfwTerminate();
            return false;
        }
        glfwMakeContextCurrent(m_window);

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

    bool Renderer::init_shaders() {
        const char *vshader_src = "#version 400 core\n"
                                  "\n"
                                  "layout (location = 0) in vec3 inPos;\n"
                                  "layout (location = 1) in vec3 inColor;\n"
                                  "layout (location = 2) in mat4 instanceModel;\n"
                                  "\n"
                                  "uniform mat4 VP;\n"
                                  "\n"
                                  "out vec4 color;\n"
                                  "\n"
                                  "void main() {\n"
                                  "    gl_Position = VP * instanceModel * vec4(inPos, 1);\n"
                                  "    color = vec4(inColor, 1);\n"
                                  "}";
        int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vshader_src, nullptr);
        glCompileShader(vertex_shader);
        int success;
        char infoLog[512];
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex_shader, 512, nullptr, infoLog);
            std::cout << "Error on vertex compilation: " << infoLog << std::endl;
            return false;
        }
        const char *fshader_src = "#version 400 core\n"
                                  "\n"
                                  "\n"
                                  "in vec4 color;\n"
                                  "out vec4 fragColor;\n"
                                  "\n"
                                  "\n"
                                  "void main() {\n"
                                  "    fragColor = color;\n"
                                  "}";
        int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &fshader_src, nullptr);
        glCompileShader(fragment_shader);
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragment_shader, 512, nullptr, infoLog);
            std::cout << "Error on fragment compilation: " << infoLog << std::endl;
            return false;
        }
        shader3d = glCreateProgram();
        glAttachShader(shader3d, vertex_shader);
        glAttachShader(shader3d, fragment_shader);
        glLinkProgram(shader3d);
        glGetProgramiv(shader3d, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader3d, 512, nullptr, infoLog);
            std::cout << "Error linking program: " << infoLog << std::endl;
            return false;
        }
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        const char *vshader_src2d = "#version 400 core\n"
                                  "\n"
                                  "layout (location = 0) in vec2 inPos;\n"
                                  "layout (location = 1) in vec3 inColor;\n"
                                  "layout (location = 2) in mat4 instanceModel;\n"
                                  "\n"
                                  "uniform mat4 VP;\n"
                                  "\n"
                                  "out vec4 color;\n"
                                  "\n"
                                  "void main() {\n"
                                  "    gl_Position = VP * instanceModel * vec4(inPos, 0, 1);\n"
                                  "    color = vec4(inColor, 1);\n"
                                  "}";
        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vshader_src2d, nullptr);
        glCompileShader(vertex_shader);
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex_shader, 512, nullptr, infoLog);
            std::cout << "Error on vertex compilation: " << infoLog << std::endl;
            return false;
        }
        const char *fshader_src2d = "#version 400 core\n"
                                  "\n"
                                  "\n"
                                  "in vec4 color;\n"
                                  "out vec4 fragColor;\n"
                                  "\n"
                                  "\n"
                                  "void main() {\n"
                                  "    fragColor = color;\n"
                                  "}";
        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &fshader_src2d, nullptr);
        glCompileShader(fragment_shader);
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragment_shader, 512, nullptr, infoLog);
            std::cout << "Error on fragment compilation: " << infoLog << std::endl;
            return false;
        }
        shader2d = glCreateProgram();
        glAttachShader(shader2d, vertex_shader);
        glAttachShader(shader2d, fragment_shader);
        glLinkProgram(shader2d);
        glGetProgramiv(shader2d, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader2d, 512, nullptr, infoLog);
            std::cout << "Error linking program: " << infoLog << std::endl;
            return false;
        }
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

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
                cbo, // Color Buffer Object
                ebo, // Element Buffer Object
                static_cast<unsigned int>(mesh.indices.size()) // Number of indices
        );
        registry.emplace<InstanceList>(entity, ibo);
        glBindVertexArray(0);
    }

    void Renderer::load_sprite(entt::registry &registry, entt::entity entity) {
        auto sizeof_vec4 = sizeof(glm::vec4);
        auto sprite = registry.get<Sprite>(entity);
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
                cbo, // Color Buffer Object
                ebo, // Element Buffer Object
                static_cast<unsigned int>(sprite.indices.size()) // Number of indices
        );
        registry.emplace<InstanceList>(entity, ibo);
        glBindVertexArray(0);
    }

    void Renderer::destroy_vao(entt::registry &registry, entt::entity entity) {
        auto vao = registry.get<VertexArrayObject>(entity);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glDeleteVertexArrays(1, &vao.id);
        glDeleteBuffers(1, &vao.vbo);
        glDeleteBuffers(1, &vao.cbo);
        glDeleteBuffers(1, &vao.ebo);
    }

    void Renderer::destroy_instances(entt::registry &registry, entt::entity entity) {
        auto instances = registry.get<InstanceList>(entity);
        glDeleteBuffers(1, &instances.id);
    }

    void Renderer::render(entt::registry &registry) {
        if(InputManager::has_resized()) {
            resize(InputManager::get_width(), InputManager::get_height());
            InputManager::clear_resize();
        }
        auto const &camera = registry.get<Camera>(m_camera_entity);
        auto vp_uniform = glGetUniformLocation(shader3d, "VP");
        glm::mat4 view_matrix = camera.get_view();
        glm::mat4 vp = glm::perspective(45.0f, m_width / m_height, 0.1f, 1000.0f) * view_matrix;
        glUseProgram(shader3d);
        glUniformMatrix4fv(vp_uniform, 1, GL_FALSE, &vp[0][0]);
        auto view3d = registry.view<VertexArrayObject, InstanceList, Mesh>();
        for (const auto &entity: view3d) {
            auto [vao, ilist, mesh] = view3d.get(entity);
            glBindVertexArray(vao.id);
            glDrawElementsInstanced(ilist.render_strategy, vao.num_indices, GL_UNSIGNED_INT, 0, ilist.instances.size());
        }
        vp_uniform = glGetUniformLocation(shader2d, "VP");
        vp = glm::ortho(-0.f*m_width, 1.f*m_width, 1.f*m_height, -0.f*m_height);
        glUseProgram(shader2d);
        glUniformMatrix4fv(vp_uniform, 1, GL_FALSE, &vp[0][0]);
        auto view2d = registry.view<VertexArrayObject, InstanceList, Sprite>();
        for(const auto &entity: view2d) {
            auto [vao, ilist, sprite] = view2d.get(entity);
            glBindVertexArray(vao.id);
            glDrawElementsInstanced(ilist.render_strategy, vao.num_indices, GL_UNSIGNED_INT, 0, ilist.instances.size());
        }
    }

    void Renderer::cleanup(entt::registry &registry) {
        glDeleteProgram(shader3d);
        glfwTerminate();
    }

    void Renderer::resize(float width, float height) {
        glViewport(0, 0, width, height);
        m_width = width;
        m_height = height;
    }

    void Renderer::read_config(const std::string& filename) {
        try {
            auto config_json = utils::file::read_json_file(filename);
            if (config_json.contains("width"))
                m_width = config_json["width"];
            if (config_json.contains("height"))
                m_height = config_json["height"];
        } catch (nlohmann::detail::parse_error e) {
            std::cout << "Error reading config file: " << e.what() << std::endl;
            m_width = 800;
            m_height = 600;
        }
    }
} // namespace engine