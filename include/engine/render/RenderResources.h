/* Created by Philip Smith on 1/9/22.
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

#ifndef ENGINE_REQUESTEDRESOURCES_H
#define ENGINE_REQUESTEDRESOURCES_H

#include <engine/render/Glyph.h>
#include <GL/glew.h>
#include <map>
#include <string>


using std::string;
using std::map;

namespace engine {

struct FontResource {
	string path;
	int size;
};

struct ShaderResource {
	string vertex_path;
	string fragment_path;
};

typedef string TextureResource;

typedef map<unsigned long, Glyph> FontAsset;

typedef GLuint ShaderAsset;

typedef GLuint TextureAsset;

struct RenderResources {
	map<string, ShaderResource> shaders{};
	map<string, TextureResource> textures{};
	map<string, FontResource> fonts{};
};

struct RenderAssets {
	map<string, ShaderAsset> shaders{};
	map<string, TextureAsset> textures{};
	map<string, FontAsset> fonts{};
};

} // namespace engine

#endif //ENGINE_REQUESTEDRESOURCES_H
