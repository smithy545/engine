//
// Created by Philip Smith on 6/7/21.
//

#ifndef ENGINE_INTERFACEVIEW_H
#define ENGINE_INTERFACEVIEW_H

#include <fmt/format.h>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>

#include <engine/entity.h>
#include <engine/physics.h>
#include <engine/input_events.h>
#include <engine/lifecycle.h>
#include <engine/RenderContext.h>
#include <utils/macros.h>
#include <utils/math_util.h>

#include "InterfaceEntity.h"


using namespace utils::math;

namespace engine {
    class InterfaceController;

	class InterfaceView : public IndependentEntity, public entt::emitter<InterfaceView> {
    public:
        PTR(InterfaceView);

        InterfaceView(InterfaceController& controller, entt::registry& registry);

        virtual void load(const RenderContext& context);

        virtual void unload();

	    virtual void tick();

		InterfaceEntity::Ptr get_nearest(double x, double y);

	    entt::entity insert_element(const InterfaceEntity::Ptr& element);

        void remove_element(const InterfaceEntity::Ptr& element);

        void transition(Ptr next_state);

        entt::registry& get_registry();
    protected:
        InterfaceController& m_controller;

	    virtual void mouse_button_callback(MouseButtonEvent& event, InterfaceView& emitter);

		virtual void mouse_motion_callback(MouseMotionEvent& event, InterfaceView& emitter);
    private:
		std::unordered_map<Point_2, InterfaceEntity::Ptr> m_entity_by_position;
        std::shared_ptr<PointFinder> m_nearest_entity_finder{nullptr};
    };
} // namespace engine


#endif //ENGINE_INTERFACEVIEW_H
