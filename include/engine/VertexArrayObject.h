//
// Created by Philip Smith on 5/18/21.
//

#ifndef ENGINE_VERTEXARRAYOBJECT_H
#define ENGINE_VERTEXARRAYOBJECT_H

#include <GL/glew.h>

namespace engine {
    struct VertexArrayObject {
        GLuint id;
        GLuint vbo;
        GLuint ebo;
        unsigned int num_indices;
        GLuint cbo{0};
        GLuint tex_id{0};
        GLuint tex_uvs{0};
    };
} // namespace engine

#endif //ENGINE_VERTEXARRAYOBJECT_H
