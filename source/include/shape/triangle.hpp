#pragma once

#include "shape/shape.hpp"

struct Triangle : public Shape
{
    Triangle(
        const glm::vec3 &vo, const glm::vec3 &v1, const glm::vec3 &v2,
        const glm::vec3 &n0, const glm::vec3 &n1, const glm::vec3 &n2) : v0(vo), v1(v1), v2(v2), n0(n0), n1(n1), n2(n2) {}

    Triangle(
        const glm::vec3 &vo, const glm::vec3 &v1, const glm::vec3 &v2) : v0(vo), v1(v1), v2(v2)
    {
        glm::vec3 e1 = v1 - v0;
        glm::vec3 e2 = v2 - v0;
        glm::vec3 normal = glm::cross(e1, e2);
        n0 = n1 = n2 = glm::normalize(normal);
    }

    std::optional<HitInfo> Intersect(const Ray &ray, float t_min, float t_max) const override;

    glm::vec3 v0, v1, v2;   // 三角形的三个顶点
    glm::vec3 n0, n1, n2;   // 三角形的三个法向量
};
