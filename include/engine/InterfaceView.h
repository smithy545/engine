//
// Created by Philip Smith on 6/7/21.
//

#ifndef ENGINE_INTERFACEVIEW_H
#define ENGINE_INTERFACEVIEW_H

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Quadtree.h>
#include <fmt/format.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Collidable.h"
#include "IndependentEntity.h"
#include "input_events.h"
#include "InterfaceElement.h"
#include "RenderContext.h"


namespace engine {
    typedef CGAL::Simple_cartesian<double> Kernel;
    typedef Kernel::Point_2 Point_2;
    typedef CGAL::Quadtree<Kernel, std::vector<Point_2>> Quadtree;

    class InterfaceController;

    class InterfaceView :
        public InterfaceElement,
        public IndependentEntity,
        public Collidable,
        public entt::emitter<InterfaceView> {
    public:
        PTR(InterfaceView);

        InterfaceView(InterfaceController& controller, entt::registry& registry);

        virtual void update(const RenderContext& context);

        virtual void load(const RenderContext& context) = 0;

        virtual void unload();

        glm::vec2 get_center() override;

        bool collides(double x, double y) override;

        InterfaceElement::Ptr get_nearest_element(double x, double y);

        void insert_element(InterfaceElement::Ptr element);

        void remove_element(const InterfaceElement::Ptr& element);

        void transition(Ptr next_state);

    protected:
        void set_camera(Camera::Ptr camera);
    private:
        static const int QUADTREE_MAX_DEPTH{4};
        static const int QUADTREE_BUCKET_SIZE{10};
        std::vector<Point_2> m_element_positions;
        std::unordered_map<std::string, InterfaceElement::Ptr> m_elements;
        std::shared_ptr<Quadtree> m_collision_tree{nullptr};
        InterfaceController& m_controller;

        static std::string point_key(double x, double y) {
            return fmt::format("{}:{}", x, y);
        }
    };
} // namespace engine


#endif //ENGINE_INTERFACEVIEW_H
