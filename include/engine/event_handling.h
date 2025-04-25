/* Created by Philip Smith on 1/12/22.
MIT License

Copyright (c) 2021 Philip Arturo Smith

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef ENGINE_EVENT_HANDLING_H
#define ENGINE_EVENT_HANDLING_H

#include <functional>
#include <gsl/gsl>
#include <utils/macros.h>

#include "input_events.h"


namespace engine {

// return false when event handled
template <typename EventType>
using EventCallback = std::function<bool(EventType)>;

template <typename Event>
class HandlerChain {
public:
	using Handler = EventCallback<Event>;

	class HandlerNode {
	public:
		USEPTR(HandlerNode);

		friend class HandlerChain<Event>;

		explicit HandlerNode(Handler handler, Ptr next = nullptr)
		: m_handler(handler), m_next(next) {}
	private:
		Handler m_handler;
		Ptr m_next{nullptr};
	};

	void handle(Event event) {
		auto ptr = m_head;
		while(ptr != nullptr) {
			if(!ptr->m_handler(event))
				return;
			ptr = ptr->m_next;
		}
	}

	typename HandlerNode::Ptr set_next(Handler handler) {
		auto node = std::make_shared<HandlerNode>(handler);
		if(m_tail == nullptr) {
			m_head = node;
			m_tail = node;
		} else {
			m_tail->m_next = node;
			m_tail = node;
		}
		return node;
	}

	void remove(typename HandlerNode::Ptr node) {
		Expects(node != nullptr);
		if(node == m_head)
			m_head = m_head->m_next;
		else {
			auto ptr = m_head;
			while (ptr != nullptr && ptr->m_next != node)
				ptr = ptr->m_next;
			if(ptr != nullptr)
				ptr->m_next = node->m_next;
		}
	}

	virtual ~HandlerChain() {
		if(m_head != nullptr) {
			auto ptr = m_head;
			do {
				auto next = ptr->m_next;
				ptr->m_next = nullptr;
				ptr = next;
			} while (ptr != nullptr);
			m_head = nullptr;
		}
		m_tail = nullptr;
	}
private:
	typename HandlerNode::Ptr m_head{nullptr}, m_tail{nullptr};
};

using KeyHandlerChain = HandlerChain<KeyEvent>;
using MouseButtonHandlerChain = HandlerChain<MouseButtonEvent>;
using MouseMotionHandlerChain = HandlerChain<MouseMotionEvent>;
using MouseWheelHandlerChain = HandlerChain<MouseWheelEvent>;

using KeyCallback = EventCallback<KeyEvent>;
using MouseButtonCallback = EventCallback<MouseButtonEvent>;
using MouseMotionCallback = EventCallback<MouseMotionEvent>;
using MouseWheelCallback = EventCallback<MouseWheelEvent>;


} // namespace engine

#endif //ENGINE_EVENT_HANDLING_H
