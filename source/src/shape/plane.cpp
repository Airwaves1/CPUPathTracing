#include "shape/plane.hpp"

// 射线与平面求交
std::optional<HitInfo> Plane::Intersect(const Ray &ray, float t_min,
                                        float t_max) const {

  // 计算射线与平面的交点
  // t = (m_Point - ray.origin) * m_Normal / (ray.direction * m_Normal)
  // 如果分母为0，则射线与平面平行
  // 如果分子为0，则射线在平面上
  // 如果t < 0，则射线与平面背对
  float hit_t = glm::dot(m_Point - ray.origin, m_Normal) /
                glm::dot(ray.direction, m_Normal);
  if (hit_t > t_min && hit_t < t_max) {
    return HitInfo{hit_t, ray.Hit(hit_t), m_Normal};
  }

  return std::nullopt;
}