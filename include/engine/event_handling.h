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

#include <entt/entt.hpp>
#include <gsl/gsl>
#include <utility>
#include <utils/concepts.h>
#include <utils/macros.h>
#include <vector>

#include "events.h"


using std::shared_ptr;

namespace engine {
// entt event handlers (observer)
template <typename Event, typename Emitter>
struct EventSink : entt::type_list<void(Event)> {
	template <typename Base>
	struct type: Base {
		void handle(Event& event, Emitter& emitter) {
			entt::poly_call<0>(*this, event, emitter);
		}
	};

	template <typename Type>
	using impl = entt::value_list<&Type::handle>;
};

template <typename Emitter>
using KeyEventSink = EventSink<KeyEvent, Emitter>;

template <typename Emitter>
using MouseButtonEventSink = EventSink<MouseButtonEvent, Emitter>;

template <typename Emitter>
using MouseMotionEventSink = EventSink<MouseMotionEvent, Emitter>;

template <typename Emitter>
using MouseWheelEventSink = EventSink<MouseWheelEvent, Emitter>;

// GLFW/input event handlers (chain-of-responsibility)
namespace {
	// define handle in anonymous namespace to ensure Event is part of template signature, thereby distinguishing
	// between handlers of different event types in multiple inheritance
	template<typename Type, typename Event>
	bool handle(Type &self, Event event) {
		return self.handle(event);
	}
} // anonymous

template <typename Event>
struct Handler : entt::type_list<bool(Event)> {
	template <typename Base>
	struct type: Base {
		bool handle(Event event) {
			return entt::poly_call<0>(*this, event);
		}
	};

	template <typename Type>
	using impl = entt::value_list<&handle<Type, Event>>;
};

using KeyHandler = Handler<KeyEvent>;
using MouseButtonHandler = Handler<MouseButtonEvent>;
using MouseMotionHandler = Handler<MouseMotionEvent>;
using MouseWheelHandler = Handler<MouseWheelEvent>;

template <typename Event>
class HandlerChain {
public:
	using HandlerType = Handler<Event>;

	class HandlerNode {
	public:
		USING_PTR(HandlerNode);

		friend class HandlerChain<Event>;

		explicit HandlerNode(entt::poly<HandlerType> handler, Ptr next = nullptr)
		: m_handler(handler), m_next(next) {}
	private:
		entt::poly<HandlerType> m_handler{};
		Ptr m_next{nullptr};
	};

	void handle(Event event) {
		auto ptr = m_head;
		while(ptr != nullptr) {
			if(!ptr->m_handler->handle(event))
				return;
			ptr = ptr->m_next;
		}
	}

	typename HandlerNode::Ptr set_next(entt::poly<HandlerType> handler) {
		auto node = std::make_shared<HandlerNode>(handler);
		if(m_head == nullptr)
			m_head = node;
		else {
			auto ptr = m_head;
			while (ptr->m_next != nullptr)
				ptr = ptr->m_next;
			ptr->m_next = node;
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
		auto ptr = m_head;
		while(ptr != nullptr) {
			auto next = ptr->m_next;
			ptr->m_next = nullptr;
			ptr = next;
		}
		m_head = nullptr;
	}
private:
	typename HandlerNode::Ptr m_head{nullptr};
};

using KeyHandlerChain = HandlerChain<KeyEvent>;
using MouseButtonHandlerChain = HandlerChain<MouseButtonEvent>;
using MouseMotionHandlerChain = HandlerChain<MouseMotionEvent>;
using MouseWheelHandlerChain = HandlerChain<MouseWheelEvent>;
} // namespace engine

#endif //ENGINE_EVENT_HANDLING_H
