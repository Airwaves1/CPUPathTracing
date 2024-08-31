#include <chrono>
#include <cstdlib>
#include <iostream>


#include "camera/camera.hpp"
#include "camera/film.hpp"
#include "glm/fwd.hpp"
#include "shape/model.hpp"
#include "shape/plane.hpp"
#include "shape/scene.hpp"
#include "shape/sphere.hpp"
#include "util/frame.hpp"
#include "util/progress.hpp"
#include "util/randomGenerator.hpp"
#include "util/rgb.hpp"


#ifdef PROJECT_ROOT_PATH
#define OUTPUT_PATH PROJECT_ROOT_PATH "/output.ppm"
#define RESOURCE_PATH PROJECT_ROOT_PATH "/resources/"
#define MODEL_PATH PROJECT_ROOT_PATH "/resources/models/"
#endif

int main() {
  std::cout << "--------------CPU Ray Tracing--------------" << std::endl;


  // 随机数生成器
  RandomGenerator gen;



  // 每个像素的采样点
  int spp = 4;

  // 创建一个1920x1080的胶片
  Film film{192 * 4, 108 * 4};

  // 简易进度条
  Progress progress{film.GetWidth() * film.GetHeight() * spp, 1};


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

  // 开始计时
  auto start = std::chrono::high_resolution_clock::now();

  pool.ParallelFor(film.GetWidth(), film.GetHeight(), [&](size_t x, size_t y) {
    for (int i = 0; i < spp; i++) {
      auto ray =
          camera.GenerateRay({x, y}, gen.Uniform(), gen.Uniform()); // 生成光线

      glm::vec3 beta = {1, 1, 1};
      glm::vec3 color = {0, 0, 0};

      while (true) {
        auto hit_info = scene.Intersect(ray, 0.001f, 1000.0f);
        if (hit_info.has_value()) {

          color += beta * (hit_info->material->emission); // 加上物体的自发光

          beta *= hit_info->material->albedo; // 更新beta

          // 生成下一条光线
          ray.origin = hit_info->hit_point;

          // 计算局部反射空间 Frame
          Frame frame{hit_info->normal};
          glm::vec3 light_direction_2;
          if (hit_info->material->is_specular) {
            glm::vec3 view_direction = frame.WorldToLocal(-ray.direction);
            light_direction_2 = {-view_direction.x, view_direction.y,
                                 -view_direction.z};
          } else {
            // 生成随机漫反射光线，使用半球采样，即只在半球内生成光线，如果生成的光线在物体的法线的下方，则取反；采样点必须在半球内
            do {
              light_direction_2 = {gen.Uniform(), gen.Uniform(), gen.Uniform()};
            } while (glm::length(light_direction_2) > 1.0f);

            light_direction_2 = light_direction_2.y > 0 ? light_direction_2
                                                        : -light_direction_2;

            ray.direction = frame.LocalToWorld(light_direction_2);
          }

        } else {
          break;
        }
      }

      film.AddSample(x, y, color);
    }

    progress.Update(spp);
  });

  pool.Wait();

  // 结束计时
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "ParallelFor execution time: " << elapsed.count() << " seconds"
            << std::endl;

  film.Save(OUTPUT_PATH);
  return 0;
}