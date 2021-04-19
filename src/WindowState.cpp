//
// Created by Philip Smith on 10/18/2020.
//

#include <engine/WindowState.h>

WindowState::WindowState() : m_mouse_x(0),
                             m_mouse_y(0),
                             m_last_mouse_x(0),
                             m_last_mouse_y(0) {
    for (bool &key : keys)
        key = false;
}

void WindowState::pause() {
    m_paused = true;
}

void WindowState::unpause() {
    m_paused = false;
}

void WindowState::start() {
    m_paused = false;
    m_stopped = false;
    m_last_frame_start = std::chrono::system_clock::now();
}

void WindowState::stop() {
    m_stopped = true;
    m_last_frame_start = std::chrono::system_clock::now();
}

double WindowState::enter_frame() {
    auto frame_start = std::chrono::system_clock::now();
    auto diff = std::chrono::duration<double>(frame_start - m_last_frame_start).count();
    m_fps = 1.0 / diff;
    m_last_frame_start = frame_start;
    return diff;
}

bool WindowState::get_key(int key) const {
    return keys[key];
}

void WindowState::set_key(int key, bool value) {
    keys[key] = value;
}

void WindowState::set_mouse_x(double x) {
    m_last_mouse_x = m_mouse_x;
    m_mouse_x = x;
}

void WindowState::set_mouse_y(double y) {
    m_last_mouse_y = m_mouse_y;
    m_mouse_y = y;
}

void WindowState::set_mouse_action(int button, int action) {
    if(button == GLFW_MOUSE_BUTTON_LEFT) {
        if(action == GLFW_PRESS)
            buttons[0] = true;
        else if(action == GLFW_RELEASE)
            buttons[0] = false;
    } else if(button == GLFW_MOUSE_BUTTON_RIGHT) {
        if(action == GLFW_PRESS)
            buttons[1] = true;
        else if(action == GLFW_RELEASE)
            buttons[1] = false;
    } else if(button == GLFW_MOUSE_BUTTON_MIDDLE) {
        if (action == GLFW_PRESS)
            buttons[2] = true;
        else if (action == GLFW_RELEASE)
            buttons[2] = false;
    }
}

bool WindowState::get_left_mouse_button() const {
    return buttons[0];
}

bool WindowState::get_middle_mouse_button() const {
    return buttons[1];
}

bool WindowState::get_right_mouse_button() const {
    return buttons[2];
}

void WindowState::resize(int width, int height) {
    resized = true;
    m_width = width;
    m_height = height;
}

bool WindowState::check_resize_flag() const {
    return resized;
}

void WindowState::clear_flags() {
    resized = false;
}
