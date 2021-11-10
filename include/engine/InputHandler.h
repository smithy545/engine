//
// Created by Philip Smith on 10/11/21.
//

#ifndef ENGINE_INPUTHANDLER_H
#define ENGINE_INPUTHANDLER_H


namespace engine {
	template <class Handler>
	struct InputHandler {
		void set_next_handler(Handler* handler) {
			auto ptr = this;
			while(ptr->m_next != nullptr)
				ptr = ptr->m_next;
			ptr->m_next = handler;
		}
	protected:
		Handler* m_next{nullptr};
	};
} // namespace engine

#endif //ENGINE_INPUTHANDLER_H
