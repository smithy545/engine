/* Created by Philip Smith on 1/22/22.
MIT License

Copyright (c) 2021 Philip Arturo Smith

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef ENGINE_MIXINS_H
#define ENGINE_MIXINS_H

#include <entt/entt.hpp>
#include <utils/concepts.h>


namespace engine::mixins {
struct Tickable : entt::type_list<void(entt::registry&)> {
	template <typename Base>
	struct type: Base {
		void tick(entt::registry &registry) {
			entt::poly_call<0>(*this, registry);
		}
	};

	template <typename Type>
	using impl = entt::value_list<&Type::tick>;
};

template <typename Tickable>
concept TickableConcept = requires(Tickable ticker, entt::registry& registry) {
	{ ticker.tick(registry) } -> std::convertible_to<void>;
};

struct Drawable : entt::type_list<void(entt::registry&)> {
	template <typename Base>
	struct type: Base {
		void render(entt::registry &registry) {
			entt::poly_call<0>(*this, registry);
		}
	};

	template <typename Type>
	using impl = entt::value_list<&Type::render>;
};

template <typename Drawable>
concept DrawableConcept = requires(Drawable drawable, entt::registry& registry) {
	{ drawable.render(registry) } -> std::convertible_to<void>;
};

// convenience method for getting type_list from Type... (no need for definition, only used in decltype)
template<typename... Type>
entt::type_list<Type...> as_type_list(const entt::type_list<Type...> &);
} // namespace engine::mixins

#endif //ENGINE_MIXINS_H
