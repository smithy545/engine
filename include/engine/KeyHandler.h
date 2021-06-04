//
// Created by Philip Smith on 6/4/21.
//

#ifndef ENGINE_KEYUPHANDLER_H
#define ENGINE_KEYUPHANDLER_H

#include <utils/macros.h>


namespace engine {
    class KeyHandler {
    public:
        void handle(int code, bool value) {
            if(trigger(code, value) && m_next_key_handler != nullptr)
                m_next_key_handler->handle(code, value);
        }
    VAR(KeyHandler*, next_key_handler, public, public){nullptr};
    protected:
        virtual bool trigger(int code, bool value) = 0;
    private:
    };
} // namespace engine


#endif //ENGINE_KEYUPHANDLER_H
