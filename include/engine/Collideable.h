//
// Created by Philip Smith on 6/13/21.
//

#ifndef CIVILWAR_COLLIDEABLE_H
#define CIVILWAR_COLLIDEABLE_H


namespace engine {
    struct Collideable {
        virtual bool collides(double x, double y) {
            return false;
        }
    };
} // namespace engine


#endif //CIVILWAR_COLLIDEABLE_H
