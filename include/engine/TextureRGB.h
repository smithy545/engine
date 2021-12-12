//
// Created by Philip Smith on 12/11/21.
//

#ifndef ENGINE_TEXTURERGBA_H
#define ENGINE_TEXTURERGBA_H


namespace engine {
	struct TextureRGB {
		std::string name;
		int width, height;
		const void *data;

		TextureRGB(std::string name, int width, int height, const void *data)
				: name(name), width(width), height(height), data(data) {}
	};
} // namespace engine

#endif //ENGINE_TEXTURERGBA_H
