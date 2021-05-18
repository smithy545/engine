//
// Created by Philip Smith on 5/18/21.
//

#ifndef ENGINE_VERTEXARRAYOBJECT_H
#define ENGINE_VERTEXARRAYOBJECT_H

#include <GL/glew.h>


struct VertexArrayObject {
    GLuint id;
    GLuint vbo;
    GLuint cbo;
    GLuint ebo;
    unsigned int num_indices;
};

#endif //ENGINE_VERTEXARRAYOBJECT_H
