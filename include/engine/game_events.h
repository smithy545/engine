//
// Created by Philip Smith on 6/21/21.
//

#ifndef ENGINE_GAME_EVENTS_H
#define ENGINE_GAME_EVENTS_H

#include "Camera.h"


namespace engine {
	struct NoopEvent {};

	typedef NoopEvent TransitionEvent; // for transitioning between game states

	struct CameraMoveEvent {
        const Camera::Ptr& camera;
    };
} // namespace engine

#endif //ENGINE_GAME_EVENTS_H
