#pragma once

#include "shape/material.hpp"
#include <glm/glm.hpp>

struct Ray {
  glm::vec3 origin;

  glm::vec3 direction;

  glm::vec3 Hit(float t) const { return origin + direction * t; }

  // 在后续 BVH 加速结构中，是在对象空间中进行求交的，所以需要将世界空间的射线转换到对象空间
  Ray WorldToObject(const glm::mat4 &object_to_world) const;
};

struct HitInfo {
  float t;

  glm::vec3 hit_point;

  glm::vec3 normal;

  const Material *material = nullptr;

};
