//
// Created by Philip Smith on 5/25/21.
//

#ifndef ENGINE_GAMEMANAGER_H
#define ENGINE_GAMEMANAGER_H

#include <entt/entt.hpp>
#include <functional>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <unordered_map>

#include "KeyHandler.h"
#include "MouseButtonHandler.h"
#include "MouseMotionHandler.h"


namespace engine {
    class GameManager {
    public:
        static constexpr entt::hashed_string MOUSE_X_KEY{"mouse_x"};
        static constexpr entt::hashed_string MOUSE_Y_KEY{"mouse_y"};
        static constexpr entt::hashed_string PREV_MOUSE_X_KEY{"prev_mouse_x"};
        static constexpr entt::hashed_string PREV_MOUSE_Y_KEY{"prev_mouse_y"};
        static constexpr entt::hashed_string MOUSE_SCROLL_KEY{"mouse_scroll"};
        static constexpr entt::hashed_string MOUSE_LEFT_KEY{"mouse_left"};
        static constexpr entt::hashed_string MOUSE_MIDDLE_KEY{"mouse_middle"};
        static constexpr entt::hashed_string MOUSE_RIGHT_KEY{"mouse_right"};
        static constexpr entt::hashed_string RESIZED_KEY{"resized"};
        static constexpr entt::hashed_string WIDTH_KEY{"width"};
        static constexpr entt::hashed_string HEIGHT_KEY{"height"};
        static constexpr entt::hashed_string PAUSE_KEY{"paused"};
        static constexpr entt::hashed_string STOP_KEY{"stopped"};
        static constexpr entt::hashed_string START_KEY{"started"};

        static void register_input_callbacks(GLFWwindow* window);

        static double get_mouse_x();

        static double get_mouse_y();

        static double get_prev_mouse_x();

        static double get_prev_mouse_y();

		static bool get_mouse_button(int button);

	    static void set_mouse_button(int button, bool value);

        static void set_mouse_position(double x, double y);

        static void set_mouse_scroll(double yoffset);

        static bool get_key(int key);

        static void set_key(int key, bool value);

        static int get_width();

        static int get_height();

        static bool has_resized();

        static void clear_resize();

        static void reset_prev_mouse_coords();

        static void start();

        static void pause();

        static void unpause();

        static void stop();

        static bool is_stopped();

        static bool is_paused();

        static bool has_started();

        static void register_key_handler(KeyHandler* handler);

        static void register_mouse_button_handler(MouseButtonHandler* handler);

        static void register_mouse_motion_handler(MouseMotionHandler* handler);

    private:
        static bool keys[GLFW_KEY_LAST];
        static KeyHandler* key_chain;
        static MouseButtonHandler* mouse_button_chain;
        static MouseMotionHandler* mouse_motion_chain;
    };
} // namespace engine


#endif //ENGINE_GAMEMANAGER_H
