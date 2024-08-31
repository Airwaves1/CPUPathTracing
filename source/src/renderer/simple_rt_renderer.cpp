#include "renderer/simple_rt_renderer.hpp"
#include "util/frame.hpp"

glm::vec3 SimpleRTRenderer::RenderPixel(const glm::ivec2 &pixel_coord) {
  // 生成光线
  auto ray = m_Camera.GenerateRay(pixel_coord, m_RandomGenerator.Uniform(),
                                  m_RandomGenerator.Uniform());

  glm::vec3 beta = {1, 1, 1};
  glm::vec3 color = {0, 0, 0};

  while (true) {
    auto hit_info = m_Scene.Intersect(ray);
    if (hit_info.has_value()) {
      color += beta * hit_info->material->emission; // 加上物体自发光
      beta *= hit_info->material->albedo;           // 更新beta

      // 生成新的光线
      ray.origin = hit_info->hit_point;

      glm::vec3 new_light_dir;

      // 计算局部反射空间 Frame
      Frame frame(hit_info->normal);

      if (hit_info->material->is_specular) {
        // 镜面反射
        glm::vec3 view_direction = frame.WorldToLocal(-ray.direction);
        new_light_dir = {-view_direction.x, view_direction.y,
                         -view_direction.z};

      } else {
        // 生成随机漫反射光线，使用半球采样，即在半球内生成均匀分布的光线，如果生成的采样点在下半球，则取反；并且必须在半球内生成
        do {
          new_light_dir = {m_RandomGenerator.Uniform(),
                           m_RandomGenerator.Uniform(),
                           m_RandomGenerator.Uniform()};
        } while (glm::length(new_light_dir) > 1.0f);

        new_light_dir = new_light_dir.y > 0 ? new_light_dir : -new_light_dir;

        ray.direction = frame.LocalToWorld(new_light_dir);
      }
    } else {
      break;
    }
  }

  return color;
}