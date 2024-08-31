#pragma once

#include "material.hpp"
#include "camera/ray.hpp"
#include "shape/shape.hpp"
#include <limits>
#include <optional>

struct ShapeInstance {
  const Shape &shape;

  Material material;

  glm::mat4 object_to_world;
  glm::mat4 world_to_object;
};

struct Scene : public Shape {
public:
  void AddShape(const Shape &shape , const Material &material = {},
                const glm::vec3 &position = {0, 0, 0},
                const glm::vec3 &scale = {1, 1, 1},
                const glm::vec3 &rotation = {0, 0, 0});

  std::optional<HitInfo> Intersect(
      const Ray &ray, float t_min = 1e-6,
      float t_max = std::numeric_limits<float>::infinity()) const override;

private:
  std::vector<ShapeInstance> m_ShapeInstances;
};