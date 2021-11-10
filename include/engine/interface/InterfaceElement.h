//
// Created by Philip Smith on 6/7/21.
//

#ifndef ENGINE_INTERFACEELEMENT_H
#define ENGINE_INTERFACEELEMENT_H

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <utils/macros.h>
#include <vector>


namespace engine {
    class InterfaceElement {
    public:
        PTR(InterfaceElement);

        friend class InterfaceView;

        virtual glm::vec2 get_center() = 0;

    REFVAR(Ptr, next, public, protected){nullptr};
    };
} // namespace engine


#endif //ENGINE_INTERFACEELEMENT_H
