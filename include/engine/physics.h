//
// Created by Philip Smith on 12/22/21.
//

#ifndef ENGINE_PHYSICS_H
#define ENGINE_PHYSICS_H

namespace engine {
	struct Collidable {
		virtual bool collides(double x, double y) {
			return true;
		}
	};
} // namespace engine

#endif //ENGINE_PHYSICS_H
