//
// Created by Philip Smith on 5/25/21.
//

#include <engine/InterfaceHandler.h>

#include <engine/InputManager.h>
#include <engine/InstanceList.h>
#include <filesystem>
#include <iostream>
#include <utils/math_util.h>


using namespace entt::literals;
namespace fs = std::filesystem;

namespace engine {
    InterfaceHandler::InterfaceHandler(entt::registry &registry) {
        // load interfaces
        auto interface_schema = utils::file::read_json_file("../res/schemas/interface.json");
        std::vector<std::string> directories{"../res/interfaces"};
        while(!directories.empty()) {
            auto directory = directories.back();
            directories.pop_back();
            for (const auto &entry: fs::directory_iterator(directory)) {
                const auto& path = entry.path();
                if (entry.is_directory())
                    directories.push_back(path);
                else {
                    try {
                        auto interface_json = utils::file::read_json_file(path, interface_schema);
                        load_interface(interface_json, registry);
                    } catch(std::runtime_error &exception) {
                        std::cerr << "Error when loading interface: " << exception.what() << std::endl;
                    }
                }
            }
        }
    }

    void InterfaceHandler::update(entt::registry &registry) {
        if (InputManager::get_key(GLFW_KEY_W))
            ;//camera.move_forward();
        if (InputManager::get_key(GLFW_KEY_A))
            ;//camera.move_left();
        if (InputManager::get_key(GLFW_KEY_S))
            ;//camera.move_backward();
        if (InputManager::get_key(GLFW_KEY_D))
            ;//camera.move_right();
        if (InputManager::get_key(GLFW_KEY_SPACE))
            ;//is_paused() ? unpause() : pause();
        if (InputManager::get_key(GLFW_KEY_LEFT_SHIFT))
            ;//camera.move_up();
        if (InputManager::get_key(GLFW_KEY_LEFT_CONTROL))
            ;//camera.move_down();
        if (InputManager::get_mouse_x() != InputManager::get_prev_mouse_x())
            ;//camera.pan_horizontal(InputManager::get_prev_mouse_x() - InputManager::get_mouse_x());
        if (InputManager::get_mouse_y() != InputManager::get_prev_mouse_y())
            ;//camera.pan_vertical(InputManager::get_mouse_y() - InputManager::get_prev_mouse_y());
    }

    void InterfaceHandler::load_interface(const json& interface, entt::registry &registry) {
        auto name = interface[std::string(NAME_KEY)];
        auto interface_entity = registry.create();
        //registry.emplace<std::string>(interface_entity, name);
        for(const auto& shape_json: interface[std::string(SHAPES_KEY)])
            load_shape_from_json(shape_json, registry);
        // TODO: setup transition triggers via entt signals/observers
        auto transitions = interface[std::string(TRANSITIONS_KEY)];
    }

    void InterfaceHandler::load_shape_from_json(const json &shape, entt::registry &registry) {
        std::vector<glm::vec2> vertices;
        std::vector<glm::vec3> colors;
        std::vector<unsigned int> indices;
        glm::vec3 color(1,1,1); // default color

        std::string shape_type = shape["type"];
        entt::hashed_string shape_enum{shape_type.c_str()};
        switch(shape_enum) {
            case "polygon"_hs:
                vertices = utils::math::triangulate(shape["vertices"]);
                if(shape.contains("color")) {
                    if(shape["color"].size() > 3) {
                        for (int i = 0; i < vertices.size(); i++) {
                            if(3*i + 2 < shape["color"].size())
                                color = glm::vec3(shape["color"][3*i], shape["color"][3*i+1], shape["color"][3*i+2]);
                            colors.push_back(color);
                            indices.push_back(i);
                        }
                        break;
                    } else
                        color = glm::vec3(shape["color"][0], shape["color"][1], shape["color"][2]);
                }
                for (int i = 0; i < vertices.size(); i++) {
                    colors.push_back(color);
                    indices.push_back(i);
                }
                break;
            case "ellipse"_hs:
                // vertices contain ellipse focii (if only one focus then circle)
                break;
            case "bezier"_hs:
                // vertices contain bezier curve control points
                break;
            case "text"_hs:
                // vertices represent text outline rect?
                break;
            default:
                std::cerr << "Unknown shape type " << shape_type << std::endl;
        }

        auto shape_entity = registry.create();
        registry.emplace<Sprite>(shape_entity, vertices, colors, indices);

        auto instances = shape["instances"];
        registry.patch<InstanceList>(shape_entity, [instances](auto &instance_list) {
            std::vector<glm::mat4> models;
            for(const auto& instance: instances) {
                models.emplace_back(1);
            }
            instance_list.set_models(models);
        });
    }

    bool InterfaceHandler::trigger(double x, double y) {
        std::cout << "Mouse at " << x << " " << y << std::endl;
        return true;
    }

    bool InterfaceHandler::trigger(double x, double y, int button, bool value) {
        std::cout << "Mouse " << button << (value ? " down at " : " up at ") << x << " " << y << std::endl;
        return true;
    }

    bool InterfaceHandler::trigger(int code, bool value) {
        std::cout << "Key " << code << (value ? " down" : " up") << std::endl;
        return true;
    }
} // namespace engine
