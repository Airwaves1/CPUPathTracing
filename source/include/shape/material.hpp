#pragma once

#include <glm/glm.hpp>

struct Material {

    glm::vec3 albedo = {1.0f, 1.0f, 1.0f};  // 反射率

    bool is_specular = false;  // 是否为镜面反射，为false为漫反射材质

    glm::vec3 emission = {0.0f, 0.0f, 0.0f};  // 自发光颜色
};