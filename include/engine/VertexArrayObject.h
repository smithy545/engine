//
// Created by Philip Smith on 5/18/21.
//

#ifndef CIVILWAR_VERTEXARRAYOBJECT_H
#define CIVILWAR_VERTEXARRAYOBJECT_H

#include <GL/glew.h>


struct VertexArrayObject {
    GLuint id;
    GLuint vbo;
    GLuint cbo;
    GLuint ebo;
    unsigned int num_indices;
};

#endif //CIVILWAR_VERTEXARRAYOBJECT_H
