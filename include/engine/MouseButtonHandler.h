//
// Created by Philip Smith on 6/4/21.
//

#ifndef ENGINE_MOUSEBUTTONHANDLER_H
#define ENGINE_MOUSEBUTTONHANDLER_H

#include "InputHandler.h"


namespace engine {
	class MouseButtonHandler : public InputHandler<MouseButtonHandler> {
    public:
        void handle(double x, double y, int button, bool value) {
            if(trigger(x, y, button, value) && m_next != nullptr)
            	m_next->handle(x, y, button, value);
        }
    protected:
        virtual bool trigger(double x, double y, int button, bool value) = 0;
    };
} // namespace engine


#endif //ENGINE_MOUSEBUTTONHANDLER_H
