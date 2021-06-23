//
// Created by Philip Smith on 6/13/21.
//

#ifndef ENGINE_COLLIDABLE_H
#define ENGINE_COLLIDABLE_H


namespace engine {
    struct Collidable {
        virtual bool collides(double x, double y) {
            return false;
        }
    };
} // namespace engine


#endif //ENGINE_COLLIDABLE_H
