//
// Created by Philip Smith on 6/7/21.
//

#ifndef ENGINE_INTERFACECONTAINER_H
#define ENGINE_INTERFACECONTAINER_H

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Quadtree.h>
#include <fmt/format.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "InterfaceElement.h"


namespace engine {
    typedef CGAL::Simple_cartesian<double> Kernel;
    typedef Kernel::Point_2 Point_2;
    typedef CGAL::Quadtree<Kernel, std::vector<Point_2>> Quadtree;

    class InterfaceContainer : public InterfaceElement {
    public:
        PTR(InterfaceContainer);

        InterfaceContainer();

        explicit InterfaceContainer(std::vector<InterfaceElement::Ptr>& initial_elements);

        void register_with(entt::registry& registry) override;

        void deregister(entt::registry& registry) override;

        bool handle_mouse_move(double x, double y) override;

        bool handle_mouse_down(double x, double y, int button) override;

        bool handle_mouse_up(double x, double y, int button) override;

        bool handle_key_down(int code) override;

        bool handle_key_up(int code) override;

        glm::vec2 get_center() override;

        bool collides(double x, double y) override;

        InterfaceElement::Ptr get_nearest_element(double x, double y);

        void insert_element(InterfaceElement::Ptr element);

        void remove_element(const InterfaceElement::Ptr& element);

        void clear();

    private:
        std::vector<Point_2> m_element_positions;
        std::unordered_map<std::string, InterfaceElement::Ptr> m_elements;
        std::shared_ptr<Quadtree> m_collision_tree{nullptr};

        static std::string point_key(double x, double y) {
            return fmt::format("{}:{}", x, y);
        }
    };
} // namespace engine


#endif //ENGINE_INTERFACECONTAINER_H
