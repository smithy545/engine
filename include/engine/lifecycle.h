//
// Created by Philip Smith on 12/22/21.
//

#ifndef ENGINE_LIFECYCLE_H
#define ENGINE_LIFECYCLE_H

namespace engine {
	struct Tickable {
		virtual void tick() = 0;
	};
} // namespace engine

#endif //ENGINE_LIFECYCLE_H
