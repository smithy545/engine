//
// Created by Philip Smith on 5/18/21.
//

#ifndef ENGINE_VERTEXARRAYOBJECT_H
#define ENGINE_VERTEXARRAYOBJECT_H

#include <GL/glew.h>

namespace engine {
    struct VertexArrayObject {
        GLuint id{0};
        GLuint vbo{0};
        GLuint ebo{0};
        unsigned int num_indices{0};
        GLuint cbo{0};
        GLuint tex_id{0};
        GLuint tex_uvs{0};
    };
} // namespace engine

#endif //ENGINE_VERTEXARRAYOBJECT_H
