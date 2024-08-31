#pragma once

#include "ray.hpp"
#include "shape.hpp"
#include <optional>

// optional是C++17引入的新特性，它是一个模板类，用于包装一个值，这个值可能存在也可能不存在

struct Sphere : public Shape
{
    Sphere(const glm::vec3 &center, float radius) : center(center), radius(radius) {}

    glm::vec3 center;
    float radius;

    std::optional<HitInfo> intersect(const Ray &ray, float t_min, float t_max) const override;
};