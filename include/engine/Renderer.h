//
// Created by Philip Smith on 10/17/2020.
//

#ifndef CIVILWAR_RENDERER_H
#define CIVILWAR_RENDERER_H

#include <entt/entt.hpp>
#include <glm/ext.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <unordered_map>
#include <utils/macros.h>

#include "Mesh.h"


class Renderer {
public:
    struct Camera {
        glm::vec3 position{0, 0, 0};
        glm::vec3 forward{0, 0, 1};
        glm::vec3 up{0, 1, 0};
        float scale{1.0f};
    };

    struct VertexArrayObject {
        GLuint id;
        GLuint vbo;
        GLuint cbo;
        GLuint ebo;
        GLuint strategy; // GL_POINTS, GL_TRIANGLES, GL_LINES etc.
        unsigned int num_indices;
        unsigned int num_instances;
    };

    bool init(entt::registry &registry);

    void render_scene(entt::registry &registry);

    static void load_mesh(entt::registry &registry, entt::entity entity);

    static void update_mesh(entt::registry &registry, entt::entity entity);

    static void destroy_mesh(entt::registry &registry, entt::entity entity);

    void resize(float width, float height);

    void cleanup(entt::registry &registry);

    void pan_horizontal(float diff);

    void pan_vertical(float diff);

    void move_forward();

    void move_backward();

    void move_left();

    void move_right();

    void move_up();

    void move_down();

private:
    Camera camera;
    GLuint current_shader;

    void read_config(std::string filename);

    static bool init_glfw();

    static bool init_glew();

    bool init_window(int width, int height);

    bool init_shaders();

VAR_GET(float, width, public){0};
VAR_GET(float, height, public){0};
VAR_GET(GLFWwindow*, window, public){nullptr};
};


#endif //CIVILWAR_RENDERER_H
