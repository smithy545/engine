//
// Created by Philip Smith on 6/14/21.
//

#ifndef CIVILWAR_UPDATEABLEENTITY_H
#define CIVILWAR_UPDATEABLEENTITY_H

#include "IndependentEntity.h"


namespace engine {
    class UpdateEntity : virtual public IndependentEntity {
    public:
        virtual void update() = 0;
    };
} // namespace engine


#endif //CIVILWAR_UPDATEABLEENTITY_H
