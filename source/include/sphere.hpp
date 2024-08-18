#pragma once

#include <optional> 
#include "ray.hpp"

// optional是C++17引入的新特性，它是一个模板类，用于包装一个值，这个值可能存在也可能不存在

struct Sphere
{
    glm::vec3 center;
    float radius;

    std::optional<float> intersect(const Ray &ray) const;
};