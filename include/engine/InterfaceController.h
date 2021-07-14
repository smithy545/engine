//
// Created by Philip Smith on 5/25/21.
//

#ifndef ENGINE_INTERFACECONTROLLER_H
#define ENGINE_INTERFACECONTROLLER_H

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <utils/file_util.h>
#include <utils/macros.h>
#include <vector>

#include "InterfaceView.h"
#include "KeyHandler.h"
#include "MouseButtonHandler.h"
#include "MouseMotionHandler.h"
#include "RenderContext.h"
#include "UpdateEntity.h"


namespace engine {
    class InterfaceController :
            public UpdateEntity,
            public KeyHandler,
            public MouseButtonHandler,
            public MouseMotionHandler {
    public:
        explicit InterfaceController(entt::registry &registry, const RenderContext& context);

        void update() override;

        void set_state(InterfaceView::Ptr state);

    private:
        const RenderContext& context;
        InterfaceView::Ptr m_state, m_prev_state{nullptr};

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
