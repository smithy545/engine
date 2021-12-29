//
// Created by Philip Smith on 5/25/21.
//

#ifndef ENGINE_INTERFACECONTROLLER_H
#define ENGINE_INTERFACECONTROLLER_H

#include <engine/Camera.h>
#include <engine/entity.h>
#include <engine/KeyHandler.h>
#include <engine/MouseButtonHandler.h>
#include <engine/MouseMotionHandler.h>
#include <engine/lifecycle.h>

#include "InterfaceView.h"


namespace engine {
    class InterfaceController :
    		public IndependentEntity,
            public KeyHandler,
            public MouseButtonHandler,
            public MouseMotionHandler,
            public Tickable {
    public:
    	explicit InterfaceController(entt::registry &registry, const RenderContext& context);

        void tick() override;

        void set_state(InterfaceView::Ptr state);

        void set_camera(Camera::Ptr camera);

        Camera::Ptr get_camera();

    private:
        const RenderContext& m_context;
        Camera::Ptr m_camera{nullptr};
        InterfaceView::Ptr m_state{nullptr}, m_prev_state{nullptr};

        // mouse motion
        bool trigger(double x, double y) override;

        // mouse scroll
        bool trigger(double scroll_delta) override;

        // mouse press
        bool trigger(double x, double y, int button, bool value) override;

        // key press
        bool trigger(int code, bool value) override;
    };
} // namespace engine


#endif //ENGINE_INTERFACECONTROLLER_H
