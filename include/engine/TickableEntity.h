//
// Created by Philip Smith on 6/14/21.
//

#ifndef ENGINE_TICKABLEENTITY_H
#define ENGINE_TICKABLEENTITY_H


namespace engine {
    class TickableEntity {
    public:
        virtual void tick() = 0;
    };
} // namespace engine


#endif //ENGINE_TICKABLEENTITY_H
