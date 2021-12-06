//
// Created by Philip Smith on 6/7/21.
//

#ifndef ENGINE_INTERFACEVIEW_H
#define ENGINE_INTERFACEVIEW_H

#include <fmt/format.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <engine/Collidable.h>
#include <engine/IndependentEntity.h>
#include <engine/input_events.h>
#include <engine/RenderContext.h>
#include <engine/Tickable.h>
#include <utils/math_util.h>

#include "InterfaceElement.h"


using namespace utils::math;

namespace engine {
    class InterfaceController;

    class InterfaceView :
        public InterfaceElement,
        public IndependentEntity,
        public Collidable,
        public entt::emitter<InterfaceView>,
        public Tickable {
    public:
        PTR(InterfaceView);

        InterfaceView(InterfaceController& controller, entt::registry& registry);

        virtual void load(const RenderContext& context) = 0;

        virtual void unload();

	    void tick() override;

        glm::vec2 get_center() override;

        bool collides(double x, double y) override;

        InterfaceElement::Ptr get_nearest_element(double x, double y);

        void insert_element(InterfaceElement::Ptr element);

        void remove_element(const InterfaceElement::Ptr& element);

        void transition(Ptr next_state);

        entt::registry& get_registry();
    protected:
        InterfaceController& m_controller;
    private:
        std::unordered_map<Point_2, InterfaceElement::Ptr> m_elements;
        std::shared_ptr<PointFinder> m_element_finder{nullptr};
    };
} // namespace engine


#endif //ENGINE_INTERFACEVIEW_H
