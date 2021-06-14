//
// Created by Philip Smith on 6/4/21.
//

#ifndef ENGINE_MOUSEBUTTONHANDLER_H
#define ENGINE_MOUSEBUTTONHANDLER_H

#include <utils/macros.h>


namespace engine {
    class MouseButtonHandler {
    public:
        void handle(double x, double y, int button, bool value) {
            if(trigger(x, y, button, value) && m_next_button_handler != nullptr)
                m_next_button_handler->handle(x, y, button, value);
        }
    VAR(MouseButtonHandler*, next_button_handler, public, public){nullptr};
    protected:
        virtual bool trigger(double x, double y, int button, bool value) = 0;
    };
} // namespace engine


#endif //ENGINE_MOUSEBUTTONHANDLER_H
