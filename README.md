# engine

My ongoing work on a small game engine with a focus of procedural generation and animation. Changes often, not for public consumption unless you hate yourself.

The following code will setup a blank window and exit on pressing escape:
```
#include <chrono>
#include <engine/execution.h>
#include <engine/interface/interface.h>
#include <engine/render/renderer.h>
#include <engine/state.h>
#include <iostream>


using namespace engine;

int main() {
	// render must be initialized before state
	if(!render::init()) {
		std::cerr << "Error during renderer initialization. Exiting." << std::endl;
		render::cleanup();
		return 1;
	}

	if(!state::init()) {
		std::cerr << "Error during state initialization. Exiting." << std::endl;
		return 2;
	}

	if(!interface::init()) {
		std::cerr << "Error during interface initialization. Exiting." << std::endl;
		return 3;
	}

	execution::append_ticker([](std::chrono::nanoseconds dt) {
	    std::cout << dt << " nanoseconds elapsed since last frame." << std::endl;
	});

	state::start();
	auto initial = std::chrono::system_clock::now();
	auto start = initial;
	do {
		auto dt = std::chrono::system_clock::now() - start;
		start = std::chrono::system_clock::now();
		execution::tick(dt);
		render::render();
		state::poll();
	} while (!state::is_stopped());

	interface::cleanup();
	render::cleanup();

	return 0;
}
```
