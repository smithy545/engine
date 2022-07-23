# engine

My ongoing work on a small game engine with a focus of procedural generation and animation. Changes often, not for public consumption unless you hate yourself.

The following code will setup a blank window and exit on pressing escape:
```
#include <engine/state.h>
#include <iostream>

using namespace engine;

int main() {
	if(!state::init()) {
		std::cerr << "Error during initialization. Exiting." << std::endl;
		state::cleanup();
		return 1;
	}

	state::start();
	do {
		state::tick();
		state::render();
		state::poll();
	} while (!state::is_stopped());

	return 0;
}
```
