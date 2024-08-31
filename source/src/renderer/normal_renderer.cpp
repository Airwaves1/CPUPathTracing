#include "renderer/normal_renderer.hpp"

glm::vec3 NormalRenderer::RenderPixel(const glm::ivec2 &pixel_coord) {
  
    auto ray = m_Camera.GenerateRay(pixel_coord);
    auto hit_info = m_Scene.Intersect(ray);
    if(hit_info.has_value()) {
        return hit_info->normal * 0.5f + 0.5f;
    } 
    return {0, 0, 0};
}