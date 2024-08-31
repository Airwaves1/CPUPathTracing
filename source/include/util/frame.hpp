#pragma once

#include <glm/glm.hpp>

/*
 * 局部反射空间 Frame
 */

class Frame {

public:
  // 传入世界空间下的法线，构建局部反射空间
  Frame(const glm::vec3 &normal);

  glm::vec3 WorldToLocal(const glm::vec3 &world_direction) const;
  glm::vec3 LocalToWorld(const glm::vec3 &local_direction) const;

private:
    glm::vec3 x_axis, y_axis, z_axis;
};