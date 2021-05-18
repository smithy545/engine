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
        GLuint cbo;
        GLuint ebo;
        unsigned int num_indices;
    };
} // namespace engine

#endif //ENGINE_VERTEXARRAYOBJECT_H
