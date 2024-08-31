#include <chrono>
#include <iostream>

#include "camera/camera.hpp"
#include "camera/film.hpp"
#include "glm/fwd.hpp"
#include "shape/model.hpp"
#include "shape/plane.hpp"
#include "shape/scene.hpp"
#include "shape/sphere.hpp"

#include "renderer/normal_renderer.hpp"
#include "util/rgb.hpp"


#ifdef PROJECT_ROOT_PATH
#define OUTPUT_PATH PROJECT_ROOT_PATH "/output.ppm"
#define RESOURCE_PATH PROJECT_ROOT_PATH "/resources/"
#define MODEL_PATH PROJECT_ROOT_PATH "/resources/models/"
#endif

int main() {
  std::cout << "--------------CPU Ray Tracing--------------" << std::endl;

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

  // 创建渲染器
  NormalRenderer renderer{camera, scene};

  // 开始计时
  auto start = std::chrono::high_resolution_clock::now();

  // 渲染图像
  renderer.RenderToImage(OUTPUT_PATH, 1);

  // 结束计时
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "ParallelFor execution time: " << elapsed.count() << " seconds"
            << std::endl;
  return 0;
}