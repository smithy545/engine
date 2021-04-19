//
// Created by Philip Smith on 10/18/2020.
//

#ifndef CIVILWAR_WINDOWSTATE_H
#define CIVILWAR_WINDOWSTATE_H


#include <chrono>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <utils/macros.h>


class WindowState {
public:
    PTR(WindowState);

    WindowState();

    bool get_key(int key) const;

    bool get_left_mouse_button() const;

    bool get_middle_mouse_button() const;

    bool get_right_mouse_button() const;

    void set_key(int key, bool value);

    void set_mouse_x(double x);

    void set_mouse_y(double y);

    void set_mouse_action(int button, int action);

    void pause();

    void unpause();

    void start();

    void stop();

    double enter_frame();

    void resize(int width, int height);

    bool check_resize_flag() const;

    void clear_flags();
private:
    bool keys[GLFW_KEY_LAST]{};
    bool buttons[3]{false, false, false}; // mouse button states in natural order: left, middle, right
    bool resized{false};

// game state
BVAR(paused, public, private){true};
BVAR(stopped, public, private){true};
VAR(double, fps, public, private){0};
VAR(std::chrono::time_point<std::chrono::system_clock>, last_frame_start, public, private);
// io state
VAR_GET(double, mouse_x, public){0};
VAR_GET(double, mouse_y, public){0};
VAR_GET(double, last_mouse_x, public){0};
VAR_GET(double, last_mouse_y, public){0};
VAR_GET(int, width, public){0};
VAR_GET(int, height, public){0};
VAR(double, mouse_scroll, public, public){0};
};


#endif //CIVILWAR_WINDOWSTATE_H
