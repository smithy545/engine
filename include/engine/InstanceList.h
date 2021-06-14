//
// Created by Philip Smith on 5/18/21.
//

#ifndef ENGINE_INSTANCELIST_H
#define ENGINE_INSTANCELIST_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <utility>
#include <vector>


namespace engine {
    struct Instance {
        glm::mat4 model;

        explicit Instance(glm::mat4 model) : model(model) {}
    };

    struct InstanceList {
        GLuint id;
        GLuint render_strategy{GL_TRIANGLES};
        std::vector<Instance> instances;

        explicit InstanceList(GLuint id) : id(id) {}

        InstanceList(GLuint id, GLuint render_strategy) : id(id), render_strategy(render_strategy) {}

        void add_instance(const glm::mat4& model) {
            instances.emplace_back(model);
            glBindBuffer(GL_ARRAY_BUFFER, id);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Instance) * instances.size(), &(instances[0]), GL_STATIC_DRAW);
        }

        void set_instances(const std::vector<glm::mat4> &model_matrices) {
            instances.clear();
            for (auto model: model_matrices)
                instances.emplace_back(model);
            glBindBuffer(GL_ARRAY_BUFFER, id);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Instance) * instances.size(), &(instances[0]), GL_STATIC_DRAW);
        }
    };
} // namespace engine

#endif //ENGINE_INSTANCELIST_H
