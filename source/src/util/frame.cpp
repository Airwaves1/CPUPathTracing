#include "util/frame.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include <cstdlib>

Frame::Frame(const glm::vec3 &normal) {
  y_axis = normal;

  // 选择 up 向量：如果 normal 向量与世界坐标系的 y 轴平行，选择 z
  // 轴作为备用向量，以避免叉乘结果为零。
  glm::vec3 up = abs(normal.y) < 0.99999f ? glm::vec3(0.0f, 1.0f, 0.0f)
                                          : glm::vec3(0.0f, 0.0f, 1.0f);

  x_axis = glm::normalize(glm::cross(up, y_axis));

  z_axis = glm::normalize(glm::cross(x_axis, y_axis));
}

glm::vec3 Frame::WorldToLocal(const glm::vec3 &world_direction) const {
  return glm::normalize(glm::vec3{glm::dot(world_direction, x_axis),
                                  glm::dot(world_direction, y_axis),
                                  glm::dot(world_direction, z_axis)});
}

glm::vec3 Frame::LocalToWorld(const glm::vec3 &local_direction) const {
  return glm::normalize(local_direction.x * x_axis +
                        local_direction.y * y_axis +
                        local_direction.z * z_axis);
}