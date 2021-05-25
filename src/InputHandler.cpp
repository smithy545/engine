//
// Created by Philip Smith on 5/25/21.
//

#include <engine/InputHandler.h>

#include <utility>


void key_cb(GLFWwindow *window, int key, int scancode, int action, int mods) {
    switch (action) {
        case GLFW_PRESS:
            engine::InputHandler::set_key(key, true);
            break;
        case GLFW_RELEASE:
            engine::InputHandler::set_key(key, false);
            break;
        case GLFW_REPEAT:
            break;
        default:
            ;//std::cerr << "Key action \"" << action << "\" not handled" << std::endl;
    }
}

void cursor_pos_cb(GLFWwindow *window, double xpos, double ypos) {
    entt::monostate<engine::InputHandler::PREV_MOUSE_X_KEY>{} = ((double) entt::monostate<engine::InputHandler::MOUSE_X_KEY>{});
    entt::monostate<engine::InputHandler::PREV_MOUSE_Y_KEY>{} = ((double) entt::monostate<engine::InputHandler::MOUSE_Y_KEY>{});
    entt::monostate<engine::InputHandler::MOUSE_X_KEY>{} = xpos;
    entt::monostate<engine::InputHandler::MOUSE_Y_KEY>{} = ypos;
}

void mouse_scroll_cb(GLFWwindow *window, double xoffset, double yoffset) {
    entt::monostate<engine::InputHandler::MOUSE_SCROLL_KEY>{} = yoffset;
}

void mouse_button_cb(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT)
        entt::monostate<engine::InputHandler::MOUSE_LEFT_KEY>{} = action == GLFW_PRESS;
    else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        entt::monostate<engine::InputHandler::MOUSE_RIGHT_KEY>{} = action == GLFW_PRESS;
    else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
        entt::monostate<engine::InputHandler::MOUSE_MIDDLE_KEY>{} = action == GLFW_PRESS;
}

void resize_cb(GLFWwindow *window, int width, int height) {
    entt::monostate<engine::InputHandler::WIDTH_KEY>{} = width;
    entt::monostate<engine::InputHandler::HEIGHT_KEY>{} = height;
    entt::monostate<engine::InputHandler::RESIZED_KEY>{} = true;
}

namespace engine {
    bool InputHandler::keys[];

    InputHandler::InputHandler(GLFWwindow *window) {
        for (auto &k: keys)
            k = false;

        // Ensure we can capture the escape key being pressed below
        glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
        // Set invisible, automatically recentered cursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetKeyCallback(window, key_cb);
        glfwSetCursorPosCallback(window, cursor_pos_cb);
        glfwSetScrollCallback(window, mouse_scroll_cb);
        glfwSetMouseButtonCallback(window, mouse_button_cb);
        glfwSetFramebufferSizeCallback(window, resize_cb);
    }

    void InputHandler::update(entt::registry &registry) {
        update_prev_mouse_coords();
        for(auto [code, callback]: callbacks) {
            if(keys[code])
                callback(code, registry);
        }
        for(auto &callback: free_callbacks)
            callback();
    }

    void InputHandler::register_callback(int key, InputHandler::InputCallback callback) {
        callbacks[key] = std::move(callback);
    }

    void InputHandler::register_free_callback(const InputHandler::FreeCallback& callback) {
        free_callbacks.push_back(callback);
    }

    double InputHandler::get_mouse_x() {
        return entt::monostate<MOUSE_X_KEY>{};
    }

    double InputHandler::get_mouse_y() {
        return entt::monostate<MOUSE_Y_KEY>{};
    }

    double InputHandler::get_prev_mouse_x() {
        return entt::monostate<PREV_MOUSE_X_KEY>{};
    }

    double InputHandler::get_prev_mouse_y() {
        return entt::monostate<PREV_MOUSE_Y_KEY>{};
    }

    void InputHandler::update_prev_mouse_coords() {
        entt::monostate<PREV_MOUSE_X_KEY>{} = get_mouse_x();
        entt::monostate<PREV_MOUSE_Y_KEY>{} = get_mouse_y();
    }

    bool InputHandler::get_key(int key) {
        return keys[key];
    }

    void InputHandler::set_key(int key, bool value) {
        if (key < GLFW_KEY_LAST && key >= 0)
            keys[key] = value;
    }

    int InputHandler::get_width() {
        return entt::monostate<InputHandler::WIDTH_KEY>{};
    }

    int InputHandler::get_height() {
        return entt::monostate<InputHandler::HEIGHT_KEY>{};
    }

    bool InputHandler::has_resized() {
        return entt::monostate<RESIZED_KEY>{};
    }

    void InputHandler::clear_resize() {
        entt::monostate<engine::InputHandler::RESIZED_KEY>{} = false;
    }
} // namespace engine
