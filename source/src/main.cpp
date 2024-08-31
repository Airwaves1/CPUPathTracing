
#include "camera/camera.hpp"
#include "camera/film.hpp"
#include "glm/fwd.hpp"
#include "shape/model.hpp"
#include "shape/plane.hpp"
#include "shape/scene.hpp"
#include "shape/sphere.hpp"

#include "renderer/normal_renderer.hpp"
#include "renderer/simple_rt_renderer.hpp"
#include "util/profile.hpp"
#include "util/rgb.hpp"

#include "util/file_util.hpp"

int main() {
  // 创建一个1920x1080的胶片
  Film film{192 * 4, 108 * 4};

  // 创建摄像机
  Camera camera{film, {-3.6, 0.0, 0}, {0, 0, 0}, 50};

  // 创建平面
  Plane plane{{0, 0.0, 0}, {0, 1, 0}};

  // 创建球体
  Sphere sphere{{0, 0, 0}, 1.0f};

  // 加载模型
  Model model{std::string(MODEL_PATH) + "simple_dragon.obj"};

  // 创建场景
  Scene scene{};

  scene.AddShape(model, {RGB(202, 159, 117)}, {0, 0, 0}, {1, 3, 2}, {0, 0, 0});
  scene.AddShape(sphere, {{1, 1, 1}, false, RGB(255, 128, 128)}, {0, 0.0, 2.5});
  scene.AddShape(sphere, {{1, 1, 1}, false, RGB(128, 128, 255)},
                 {0, 0.0, -2.5});
  scene.AddShape(sphere, {{1, 1, 1}, true, RGB(128, 255, 128)}, {3.5, 0.5, -2});
  scene.AddShape(plane, {RGB{120, 204, 157}}, {0, -0.5, 0});

  // 渲染图像
  PROFILE("Render Job");

  SimpleRTRenderer rt_renderer{camera, scene};
  rt_renderer.RenderToImage(ROOT_DIR + "output_simple_rt.ppm", 4);

  film.Clear();
  NormalRenderer normal_renderer{camera, scene};
  normal_renderer.RenderToImage(ROOT_DIR + "output_normal.ppm", 1);

  return 0;
}