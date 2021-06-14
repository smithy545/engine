//
// Created by Philip Smith on 5/25/21.
//

#include <engine/InputManager.h>

#include <iostream>


void key_cb(GLFWwindow *window, int key, int scancode, int action, int mods) {
    switch (action) {
        case GLFW_PRESS:
            engine::InputManager::set_key(key, true);
            break;
        case GLFW_RELEASE:
            engine::InputManager::set_key(key, false);
            break;
        case GLFW_REPEAT:
            break;
        default:
            std::cerr << "Key action \"" << action << "\" not handled" << std::endl;
    }
}

void cursor_pos_cb(GLFWwindow *window, double xpos, double ypos) {
    engine::InputManager::set_mouse_position(xpos, ypos);
}

void mouse_scroll_cb(GLFWwindow *window, double xoffset, double yoffset) {
    entt::monostate<engine::InputManager::MOUSE_SCROLL_KEY>{} = yoffset;
}

void mouse_button_cb(GLFWwindow *window, int button, int action, int mods) {
    switch (action) {
        case GLFW_PRESS:
            engine::InputManager::set_mouse_button(button, true);
            break;
        case GLFW_RELEASE:
            engine::InputManager::set_mouse_button(button, false);
            break;
        default:
            std::cerr << "Mouse action \"" << action << "\" not handled" << std::endl;
    }
}

void resize_cb(GLFWwindow *window, int width, int height) {
    entt::monostate<engine::InputManager::WIDTH_KEY>{} = width;
    entt::monostate<engine::InputManager::HEIGHT_KEY>{} = height;
    entt::monostate<engine::InputManager::RESIZED_KEY>{} = true;
}

namespace engine {
    bool InputManager::keys[];
    KeyHandler* InputManager::key_chain = nullptr;
    MouseButtonHandler* InputManager::mouse_button_chain = nullptr;
    MouseMotionHandler* InputManager::mouse_motion_chain = nullptr;

    void InputManager::register_input_callbacks(GLFWwindow *window) {
        for (auto &k: keys)
            k = false;

        //glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
        //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetKeyCallback(window, key_cb);
        glfwSetCursorPosCallback(window, cursor_pos_cb);
        glfwSetScrollCallback(window, mouse_scroll_cb);
        glfwSetMouseButtonCallback(window, mouse_button_cb);
        glfwSetFramebufferSizeCallback(window, resize_cb);
    }

    double InputManager::get_mouse_x() {
        return entt::monostate<MOUSE_X_KEY>{};
    }

    double InputManager::get_mouse_y() {
        return entt::monostate<MOUSE_Y_KEY>{};
    }

    double InputManager::get_prev_mouse_x() {
        return entt::monostate<PREV_MOUSE_X_KEY>{};
    }

    double InputManager::get_prev_mouse_y() {
        return entt::monostate<PREV_MOUSE_Y_KEY>{};
    }

    void InputManager::reset_prev_mouse_coords() {
        entt::monostate<PREV_MOUSE_X_KEY>{} = get_mouse_x();
        entt::monostate<PREV_MOUSE_Y_KEY>{} = get_mouse_y();
    }

    void InputManager::start() {
        entt::monostate<START_KEY>{} = true;
        entt::monostate<STOP_KEY>{} = false;
        entt::monostate<PAUSE_KEY>{} = false;
    }

    void InputManager::pause() {
        entt::monostate<PAUSE_KEY>{} = true;
    }

    void InputManager::unpause() {
        entt::monostate<PAUSE_KEY>{} = false;
    }

    void InputManager::stop() {
        entt::monostate<STOP_KEY>{} = true;
    }

    bool InputManager::is_stopped() {
        return entt::monostate<STOP_KEY>{};
    }

    bool InputManager::is_paused() {
        return entt::monostate<PAUSE_KEY>{};
    }

    bool InputManager::has_started() {
        return entt::monostate<START_KEY>{};
    }

    int InputManager::get_width() {
        return entt::monostate<InputManager::WIDTH_KEY>{};
    }

    int InputManager::get_height() {
        return entt::monostate<InputManager::HEIGHT_KEY>{};
    }

    bool InputManager::has_resized() {
        return entt::monostate<RESIZED_KEY>{};
    }

    void InputManager::clear_resize() {
        entt::monostate<engine::InputManager::RESIZED_KEY>{} = false;
    }

    bool InputManager::get_key(int key) {
        return keys[key];
    }

    void InputManager::set_key(int key, bool value) {
        if (key < GLFW_KEY_LAST && key >= 0)
            keys[key] = value;
        if(key_chain != nullptr)
            key_chain->handle(key, value);
    }

    void InputManager::set_mouse_position(double x, double y) {
        entt::monostate<PREV_MOUSE_X_KEY>{} = ((double) entt::monostate<MOUSE_X_KEY>{});
        entt::monostate<PREV_MOUSE_Y_KEY>{} = ((double) entt::monostate<MOUSE_Y_KEY>{});
        entt::monostate<MOUSE_X_KEY>{} = x;
        entt::monostate<MOUSE_Y_KEY>{} = y;
        if(mouse_motion_chain != nullptr)
            mouse_motion_chain->handle(x, y);
    }

    void InputManager::set_mouse_button(int button, bool value) {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
            entt::monostate<MOUSE_LEFT_KEY>{} = value;
        else if (button == GLFW_MOUSE_BUTTON_RIGHT)
            entt::monostate<MOUSE_RIGHT_KEY>{} = value;
        else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
            entt::monostate<MOUSE_MIDDLE_KEY>{} = value;
        if(mouse_button_chain != nullptr)
            mouse_button_chain->handle(get_mouse_x(), get_mouse_y(), button, value);
    }

    void InputManager::register_key_handler(KeyHandler* handler) {
        if(key_chain == nullptr) {
            key_chain = handler;
            return;
        }
        auto ptr = key_chain;
        while(ptr->get_next_key_handler() != nullptr)
            ptr = ptr->get_next_key_handler();
        ptr->set_next_key_handler(handler);
    }

    void InputManager::register_mouse_button_handler(MouseButtonHandler* handler) {
        if(mouse_button_chain == nullptr) {
            mouse_button_chain = handler;
            return;
        }
        auto ptr = mouse_button_chain;
        while(ptr->get_next_button_handler() != nullptr)
            ptr = ptr->get_next_button_handler();
        ptr->set_next_button_handler(handler);
    }

    void InputManager::register_mouse_motion_handler(MouseMotionHandler* handler) {
        if(mouse_motion_chain == nullptr) {
            mouse_motion_chain = handler;
            return;
        }
        auto ptr = mouse_motion_chain;
        while(ptr->get_next_motion_handler() != nullptr)
            ptr = ptr->get_next_motion_handler();
        ptr->set_next_motion_handler(handler);
    }

    void InputManager::unregister_key_handler(KeyHandler *handler) {
        if(handler == key_chain) {
            key_chain = key_chain->get_next_key_handler();
            return;
        }
        auto ptr = key_chain;
        while(ptr != nullptr) {
            if(ptr->get_next_key_handler() == handler) {
                ptr->set_next_key_handler(handler->get_next_key_handler());
                return;
            }
            ptr = ptr->get_next_key_handler();
        }
    }

    void InputManager::unregister_mouse_button_handler(MouseButtonHandler *handler) {
        if(handler == mouse_button_chain) {
            mouse_button_chain = mouse_button_chain->get_next_button_handler();
            return;
        }
        auto ptr = mouse_button_chain;
        while(ptr != nullptr) {
            if(ptr->get_next_button_handler() == handler) {
                ptr->set_next_button_handler(handler->get_next_button_handler());
                return;
            }
            ptr = ptr->get_next_button_handler();
        }
    }

    void InputManager::unregister_mouse_motion_handler(MouseMotionHandler *handler) {
        if(handler == mouse_motion_chain) {
            mouse_motion_chain = mouse_motion_chain->get_next_motion_handler();
            return;
        }
        auto ptr = mouse_motion_chain;
        while(ptr != nullptr) {
            if(ptr->get_next_motion_handler() == handler) {
                ptr->set_next_motion_handler(handler->get_next_motion_handler());
                return;
            }
            ptr = ptr->get_next_motion_handler();
        }
    }
} // namespace engine
