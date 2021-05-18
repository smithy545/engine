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
    GLuint strategy; // GL_POINTS, GL_TRIANGLES, GL_LINES etc.
    unsigned int num_indices;
    unsigned int num_instances;
};

#endif //CIVILWAR_VERTEXARRAYOBJECT_H
