//
// Created by Philip Smith on 6/4/21.
//

#ifndef ENGINE_KEYHANDLER_H
#define ENGINE_KEYHANDLER_H

#include "InputHandler.h"


namespace engine {
    class KeyHandler : public InputHandler<KeyHandler> {
    public:
        void handle(int code, bool value) {
        	if(trigger(code, value) && m_next != nullptr)
        		m_next->handle(code, value);
        }
    protected:
        virtual bool trigger(int code, bool value) = 0;
    };
} // namespace engine


#endif //ENGINE_KEYHANDLER_H
