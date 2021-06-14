//
// Created by Philip Smith on 6/13/21.
//

#ifndef CIVILWAR_INPUT_EVENTS_H
#define CIVILWAR_INPUT_EVENTS_H

#include <entt/entt.hpp>
#include <functional>


namespace engine {
    class InterfaceContainer;

    struct KeyEvent {
        int key;
        bool pressed;
    };
    struct MouseButtonEvent {
        double x;
        double y;
        int button;
        bool pressed;
    };
    struct MouseMotionEvent {
        double x;
        double y;
    };

    struct KeyEventSink {
        virtual void handle(KeyEvent& event, InterfaceContainer& emitter) = 0;
    };

    struct MouseButtonEventSink {
        virtual void handle(MouseButtonEvent& event, InterfaceContainer& emitter) = 0;
    };

    struct MouseMotionEventSink {
        virtual void handle(MouseMotionEvent& event, InterfaceContainer& emitter) = 0;
    };

} // namespace engine

#endif //CIVILWAR_INPUT_EVENTS_H
