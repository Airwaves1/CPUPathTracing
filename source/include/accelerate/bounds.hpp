#pragma once

#include "camera/ray.hpp"
#include <glm/glm.hpp>
#include <limits>

struct Bounds {
  Bounds()
      : b_min(std::numeric_limits<float>::infinity()),
              b_max(-std::numeric_limits<float>::infinity()) {}
  Bounds(const glm::vec3 &min, const glm::vec3 &max) : b_min(min), b_max(max) {}

  // 扩展包围盒，使其包含点point
  void Expand(const glm::vec3 &point) {
    b_min = glm::min(b_min, point);
    b_max = glm::max(b_max, point);
  }

  // 判断光线是否与包围盒相交
  bool HasIntersection(const Ray &ray, float t_min, float t_max) const;

  glm::vec3 b_min;
  glm::vec3 b_max;
};