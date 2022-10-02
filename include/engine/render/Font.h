/*
MIT License
Copyright (c) 2022 Philip Arturo Smith
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

#ifndef ENGINE_FONT_H
#define ENGINE_FONT_H

#include <engine/render/Glyph.h>
#include <engine/render/Font.h>
#include <engine/render/renderer.h>
#include <fmt/format.h>
#include <GL/glew.h>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>


namespace engine::render {

class Font {
public:
	Font(const FT_Library& ft, const nlohmann::json& data) {
		// enable blending for text transparency
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		m_glyphs = load_font(ft, data["path"], data["size"]);
		if(m_glyphs.empty())
			throw fmt::format("Could not initialize font from '{}'", data["path"]);
	}
private:
	std::map<unsigned long, Glyph> m_glyphs;
};

} // namespace engine::render

#endif //ENGINE_FONT_H
