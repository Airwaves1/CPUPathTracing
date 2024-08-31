#pragma once

#include "camera/camera.hpp"
#include "glm/fwd.hpp"
#include "shape/scene.hpp"
#include "util/randomGenerator.hpp"

#define DEFINE_RENDERER(Name)                                                  \
  class Name##Renderer : public BaseRenderer {                                 \
  public:                                                                      \
    Name##Renderer(Camera camera, Scene scene)                                 \
        : BaseRenderer(camera, scene) {}                                       \
                                                                               \
  protected:                                                                     \
    glm::vec3 RenderPixel(const glm::ivec2 &pixel_coord) override;             \
  };

class BaseRenderer {
public:
  BaseRenderer(Camera camera, Scene scene) : m_Camera(camera), m_Scene(scene) {}

  void RenderToImage(std::string filename, int spp);

protected:
  virtual glm::vec3 RenderPixel(const glm::ivec2 &pixel_coord) = 0;

protected:
  Camera m_Camera;
  Scene m_Scene;
  RandomGenerator m_RandomGenerator;
};