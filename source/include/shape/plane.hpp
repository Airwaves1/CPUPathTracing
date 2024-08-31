#pragma once

#include "shape.hpp"

struct Plane : public Shape {
  Plane(const glm::vec3 &point, const glm::vec3 &normal = {0, 1, 0})
      : m_Point(point), m_Normal(normal) {}

  std::optional<HitInfo> Intersect(const Ray &ray, float t_min,
                                   float t_max) const override;

  glm::vec3 m_Point;
  glm::vec3 m_Normal;
};