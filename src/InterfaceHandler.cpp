//
// Created by Philip Smith on 5/25/21.
//

#include <engine/InterfaceHandler.h>

#include <engine/RenderContext.h>
#include <engine/GameManager.h>

#include <utility>


using namespace entt::literals;
namespace fs = std::filesystem;

namespace engine {
    InterfaceHandler::InterfaceHandler(entt::registry &registry, const RenderContext& context)
    : IndependentEntity(registry), context(context) {}

    void InterfaceHandler::update() {
        if(m_state != nullptr)
            m_state->update(context);
        if(m_prev_state != nullptr)
            m_prev_state = nullptr;
    }

    void InterfaceHandler::set_state(InterfaceContainer::Ptr state) {
        // store current state and cleanly dispose of it during update step
        m_prev_state = std::move(m_state);
        m_state = std::move(state);
        m_state->load(context);
    }

    bool InterfaceHandler::trigger(double scroll_delta) {
        m_state->publish<MouseScrollEvent>(scroll_delta);
        return true;
    }

    bool InterfaceHandler::trigger(double x, double y) {
        m_state->publish<MouseMotionEvent>(x, y);
        return true;
    }

    bool InterfaceHandler::trigger(double x, double y, int button, bool value) {
        m_state->publish<MouseButtonEvent>(x, y, button, value);
        return true;
    }

    bool InterfaceHandler::trigger(int code, bool value) {
        m_state->publish<KeyEvent>(code, value);
        return true;
    }
} // namespace engine
