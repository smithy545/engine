//
// Created by Philip Smith on 5/25/21.
//

#include <engine/GameManager.h>

#include <iostream>


namespace engine {
    bool GameManager::keys[];
    KeyHandler* GameManager::key_chain = nullptr;
    MouseButtonHandler* GameManager::mouse_button_chain = nullptr;
    MouseMotionHandler* GameManager::mouse_motion_chain = nullptr;

    void key_cb(GLFWwindow *window, int key, int scancode, int action, int mods) {
        switch (action) {
            case GLFW_PRESS:
                GameManager::set_key(key, true);
                break;
            case GLFW_RELEASE:
                GameManager::set_key(key, false);
                break;
            case GLFW_REPEAT:
                break;
            default:
                std::cerr << "Key action \"" << action << "\" not handled" << std::endl;
        }
    }

    void cursor_pos_cb(GLFWwindow *window, double xpos, double ypos) {
        GameManager::set_mouse_position(xpos, ypos);
    }

    void mouse_scroll_cb(GLFWwindow *window, double xoffset, double yoffset) {
        GameManager::set_mouse_scroll(yoffset);
    }

    void mouse_button_cb(GLFWwindow *window, int button, int action, int mods) {
        switch (action) {
            case GLFW_PRESS:
                GameManager::set_mouse_button(button, true);
                break;
            case GLFW_RELEASE:
                GameManager::set_mouse_button(button, false);
                break;
            default:
                std::cerr << "Mouse action \"" << action << "\" not handled" << std::endl;
        }
    }

    void resize_cb(GLFWwindow *window, int width, int height) {
        entt::monostate<GameManager::WIDTH_KEY>{} = width;
        entt::monostate<GameManager::HEIGHT_KEY>{} = height;
        entt::monostate<GameManager::RESIZED_KEY>{} = true;
    }

    void GameManager::register_input_callbacks(GLFWwindow *window) {
        for (auto &k: keys)
            k = false;

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetKeyCallback(window, key_cb);
        glfwSetCursorPosCallback(window, cursor_pos_cb);
        glfwSetScrollCallback(window, mouse_scroll_cb);
        glfwSetMouseButtonCallback(window, mouse_button_cb);
        glfwSetFramebufferSizeCallback(window, resize_cb);
    }

    double GameManager::get_mouse_x() {
        return entt::monostate<MOUSE_X_KEY>{};
    }

    double GameManager::get_mouse_y() {
        return entt::monostate<MOUSE_Y_KEY>{};
    }

    double GameManager::get_prev_mouse_x() {
        return entt::monostate<PREV_MOUSE_X_KEY>{};
    }

    double GameManager::get_prev_mouse_y() {
        return entt::monostate<PREV_MOUSE_Y_KEY>{};
    }

    void GameManager::reset_prev_mouse_coords() {
        entt::monostate<PREV_MOUSE_X_KEY>{} = get_mouse_x();
        entt::monostate<PREV_MOUSE_Y_KEY>{} = get_mouse_y();
    }

    void GameManager::start() {
        entt::monostate<START_KEY>{} = true;
        entt::monostate<STOP_KEY>{} = false;
        entt::monostate<PAUSE_KEY>{} = false;
    }

    void GameManager::pause() {
        entt::monostate<PAUSE_KEY>{} = true;
    }

    void GameManager::unpause() {
        entt::monostate<PAUSE_KEY>{} = false;
    }

    void GameManager::stop() {
        entt::monostate<STOP_KEY>{} = true;
    }

    bool GameManager::is_stopped() {
        return entt::monostate<STOP_KEY>{};
    }

    bool GameManager::is_paused() {
        return entt::monostate<PAUSE_KEY>{};
    }

    bool GameManager::has_started() {
        return entt::monostate<START_KEY>{};
    }

    int GameManager::get_width() {
        return entt::monostate<GameManager::WIDTH_KEY>{};
    }

    int GameManager::get_height() {
        return entt::monostate<GameManager::HEIGHT_KEY>{};
    }

    bool GameManager::has_resized() {
        return entt::monostate<RESIZED_KEY>{};
    }

    void GameManager::clear_resize() {
        entt::monostate<engine::GameManager::RESIZED_KEY>{} = false;
    }

    bool GameManager::get_key(int key) {
        return keys[key];
    }

    void GameManager::set_key(int key, bool value) {
        if (key < GLFW_KEY_LAST && key >= 0)
            keys[key] = value;
        if(key_chain != nullptr)
            key_chain->handle(key, value);
    }

    void GameManager::set_mouse_position(double x, double y) {
        entt::monostate<PREV_MOUSE_X_KEY>{} = ((double) entt::monostate<MOUSE_X_KEY>{});
        entt::monostate<PREV_MOUSE_Y_KEY>{} = ((double) entt::monostate<MOUSE_Y_KEY>{});
        entt::monostate<MOUSE_X_KEY>{} = x;
        entt::monostate<MOUSE_Y_KEY>{} = y;
        if(mouse_motion_chain != nullptr)
            mouse_motion_chain->handle(x, y);
    }

	bool GameManager::get_mouse_button(int button) {
		if (button == GLFW_MOUSE_BUTTON_LEFT)
			return entt::monostate<MOUSE_LEFT_KEY>{};
		else if (button == GLFW_MOUSE_BUTTON_RIGHT)
			return entt::monostate<MOUSE_RIGHT_KEY>{};
		else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
			return entt::monostate<MOUSE_MIDDLE_KEY>{};
		return false;
	}

    void GameManager::set_mouse_button(int button, bool value) {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
            entt::monostate<MOUSE_LEFT_KEY>{} = value;
        else if (button == GLFW_MOUSE_BUTTON_RIGHT)
            entt::monostate<MOUSE_RIGHT_KEY>{} = value;
        else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
            entt::monostate<MOUSE_MIDDLE_KEY>{} = value;
        if(mouse_button_chain != nullptr)
            mouse_button_chain->handle(get_mouse_x(), get_mouse_y(), button, value);
    }

    void GameManager::set_mouse_scroll(double yoffset) {
        entt::monostate<engine::GameManager::MOUSE_SCROLL_KEY>{} = yoffset;
        if(mouse_motion_chain != nullptr)
            mouse_motion_chain->handle(yoffset);
    }

    void GameManager::register_key_handler(KeyHandler* handler) {
    	if(key_chain == nullptr)
    		key_chain = handler;
    	else
    		key_chain->set_next_handler(handler);
    }

    void GameManager::register_mouse_button_handler(MouseButtonHandler* handler) {
        if(mouse_button_chain == nullptr)
            mouse_button_chain = handler;
        else
        	mouse_button_chain->set_next_handler(handler);
    }

    void GameManager::register_mouse_motion_handler(MouseMotionHandler* handler) {
    	if(mouse_motion_chain == nullptr)
    		mouse_motion_chain = handler;
    	else
    		mouse_motion_chain->set_next_handler(handler);
    }

} // namespace engine
