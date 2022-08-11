# engine

My ongoing work on a small game engine with a focus of procedural generation and animation. Changes often, not for public consumption unless you hate yourself.

The following code will setup a blank window and exit on pressing escape:
```
#include <engine/execution.h>
#include <engine/interface/interface.h>
#include <engine/render/renderer.h>
#include <engine/state.h>
#include <iostream>


using namespace engine;

int main() {
	entt::registry registry{};

	// render must be initialized before state
	if(!render::init(registry)) {
		std::cerr << "Error during renderer initialization. Exiting." << std::endl;
		render::cleanup(registry);
		return 1;
	}

	if(!state::init()) {
		std::cerr << "Error during state initialization. Exiting." << std::endl;
		return 2;
	}

	if(!interface::init(registry)) {
		std::cerr << "Error during interface initialization. Exiting." << std::endl;
		return 3;
	}

	state::start();
	do {
		execution::tick(registry);
		render::render(registry);
		state::poll();
	} while (!state::is_stopped());

	interface::cleanup(registry);
	render::cleanup(registry);

	return 0;
}
```
