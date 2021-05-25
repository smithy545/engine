//
// Created by Philip Smith on 5/25/21.
//

#ifndef ENGINE_INPUTHANDLER_H
#define ENGINE_INPUTHANDLER_H

#include <entt/entt.hpp>
#include <functional>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <unordered_map>

#include "StateHandler.h"


namespace engine {
    class InputHandler : public StateHandler {
    public:
        typedef std::function<void(int, entt::registry&)> InputCallback;
        typedef std::function<void()> FreeCallback;

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

        explicit InputHandler(GLFWwindow* window);

        void update(entt::registry &registry) override;

        void register_callback(int key, InputCallback callback);

        void register_free_callback(const FreeCallback& callback);

        static double get_mouse_x();

        static double get_mouse_y();

        static double get_prev_mouse_x();

        static double get_prev_mouse_y();

        static bool get_key(int key);

        static void set_key(int key, bool value);

        static int get_width();

        static int get_height();

        static bool has_resized();

        static void clear_resize();
    private:
        std::unordered_map<int, InputCallback> callbacks;
        std::vector<FreeCallback> free_callbacks;
        static bool keys[GLFW_KEY_LAST];

        static void update_prev_mouse_coords();
    };
} // namespace engine

#endif //ENGINE_INPUTHANDLER_H
