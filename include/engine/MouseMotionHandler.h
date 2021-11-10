//
// Created by Philip Smith on 6/4/21.
//

#ifndef ENGINE_MOUSEMOTIONHANDLER_H
#define ENGINE_MOUSEMOTIONHANDLER_H

#include "InputHandler.h"


namespace engine {
	class MouseMotionHandler : public InputHandler<MouseMotionHandler> {
    public:
        void handle(double x, double y) {
            if(trigger(x, y) && m_next != nullptr)
            	m_next->handle(x, y);
        }
        void handle(double yoffset) {
        	if(trigger(yoffset) && m_next != nullptr)
        		m_next->handle(yoffset);
        }
    protected:
        virtual bool trigger(double x, double y) = 0;
        virtual bool trigger(double scroll_delta) = 0;
    };
} // namespace engine


#endif //ENGINE_MOUSEMOTIONHANDLER_H
