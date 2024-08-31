#include "shape/scene.hpp"
#include "glm/geometric.hpp"
#include "glm/matrix.hpp"
#include "glm/trigonometric.hpp"
#include "camera/ray.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <optional>

void Scene::AddShape(const Shape &shape, const Material &material,
                     const glm::vec3 &position, const glm::vec3 &scale,
                     const glm::vec3 &rotation) {

  glm::mat4 object_to_world =
      glm::translate(glm::mat4(1.0f), position) *
      glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), {1, 0, 0}) *
      glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), {0, 1, 0}) *
      glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), {0, 0, 1}) *
      glm::scale(glm::mat4(1.0f), scale);

  m_ShapeInstances.push_back(ShapeInstance{shape, material, object_to_world,
                                           glm::inverse(object_to_world)});
}

std::optional<HitInfo> Scene::Intersect(const Ray &ray, float t_min,
                                        float t_max) const {
  std::optional<HitInfo> closest_hit_info{};
  const ShapeInstance *closest_instance = nullptr;

  for (const auto &shape_instance : m_ShapeInstances) {
    auto object_space_ray = ray.WorldToObject(shape_instance.world_to_object);
    auto hit_info =
        shape_instance.shape.Intersect(object_space_ray, t_min, t_max);
    if (hit_info.has_value()) {
      closest_hit_info = hit_info;
      t_max = hit_info->t;
      closest_instance = &shape_instance;
    }
  }

  // 对于最近的交点，将其转换到世界空间
  // 法线需要转置矩阵的逆矩阵
  if (closest_instance) {

    closest_hit_info->hit_point = closest_instance->object_to_world *
                                  glm::vec4(closest_hit_info->hit_point, 1);
    closest_hit_info->normal = glm::normalize(
        glm::vec3(glm::transpose(closest_instance->object_to_world) *
                  glm::vec4(closest_hit_info->normal, 0)));

    closest_hit_info->material = &closest_instance->material;
  }

  return closest_hit_info;
}