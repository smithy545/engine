//
// Created by Philip Smith on 6/14/21.
//

#ifndef ENGINE_TICKABLE_H
#define ENGINE_TICKABLE_H


namespace engine {
    struct Tickable {
        virtual void tick() = 0;
    };
} // namespace engine


#endif //ENGINE_TICKABLE_H
