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

    template <class Event>
    struct EventSink {
        virtual void handle(Event& event, InterfaceContainer& emitter) = 0;
    };

    typedef EventSink<KeyEvent> KeyEventSink;
    typedef EventSink<MouseButtonEvent> MouseButtonEventSink;
    typedef EventSink<MouseMotionEvent> MouseMotionEventSink;
} // namespace engine

#endif //CIVILWAR_INPUT_EVENTS_H
