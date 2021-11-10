//
// Created by Philip Smith on 5/25/21.
//

#include <engine/interface/InterfaceController.h>

#include <utility>


using namespace entt::literals;
namespace fs = std::filesystem;

namespace engine {
    InterfaceController::InterfaceController(entt::registry &registry, const RenderContext& context)
    : IndependentEntity(registry), m_context(context) {}

    void InterfaceController::tick() {
        if(m_state != nullptr)
            m_state->tick();
    }

    void InterfaceController::set_state(InterfaceView::Ptr state) {
        m_prev_state = std::move(m_state);
        m_state = std::move(state);
        m_state->load(m_context);
    }

    void InterfaceController::set_camera(Camera::Ptr camera) {
        m_camera = std::move(camera);
    }

    Camera::Ptr InterfaceController::get_camera() {
		return m_camera;
    }

    bool InterfaceController::trigger(double scroll_delta) {
        m_state->publish<MouseScrollEvent>(scroll_delta);
        return true;
    }

    bool InterfaceController::trigger(double x, double y) {
        m_state->publish<MouseMotionEvent>(x, y);
        return true;
    }

    bool InterfaceController::trigger(double x, double y, int button, bool value) {
        m_state->publish<MouseButtonEvent>(x, y, button, value);
        return true;
    } 

    bool InterfaceController::trigger(int code, bool value) {
        m_state->publish<KeyEvent>(code, value);
        return true;
    }
} // namespace engine
