#include <chrono>
#include <iostream>

// import std;

#include "camera.hpp"
#include "film.hpp"
#include "shape.hpp"
#include "sphere.hpp"
#include "thread_pool.hpp"
#include "model.hpp"


#ifdef PROJECT_ROOT_PATH
#define OUTPUT_PATH PROJECT_ROOT_PATH "/output.ppm"
#define RESOURCE_PATH PROJECT_ROOT_PATH "/resources/"
#define MODEL_PATH PROJECT_ROOT_PATH "/resources/models/"
#endif

int main()
{
  std::cout << "--------------CPU Ray Tracing--------------" << std::endl;

  // 简易进度条
  std::atomic<int> count = 0;

  // 创建线程池
  ThreadPool pool{};

  // 创建一个1920x1080的胶片
  Film film{1920, 1080};

  // 创建摄像机
  Camera camera{film, {-1, 0, 0}, {0, 0, 0}, 90};

  // 创建光源
  glm::vec3 lightPos{-1, 2, 1};

  // 创建球体
  Sphere sphere{{0, 0, 0}, 0.5f};

  // 加载模型
  Model model{std::string(MODEL_PATH) + "simple_dragon.obj"};

  Shape &shape = model;

  // 开始计时
  auto start = std::chrono::high_resolution_clock::now();

  pool.ParallelFor(film.GetWidth(), film.GetHeight(), [&](size_t x, size_t y)
                   {
        auto ray = camera.GenerateRay({x, y});
        auto hit_info = shape.intersect(ray);
        
        if (hit_info.has_value()) 
        {
          auto L = glm::normalize(lightPos - hit_info->hit_point);
          
          float cosine = glm::max(0.0f, glm::dot(hit_info->normal, L));

          film.SetPixel(x, y, {cosine, cosine, cosine}); 
          
        }

        count++;
        if (count % film.GetWidth() == 0)
        {
          float progress = static_cast<float>(count) / (film.GetWidth() * film.GetHeight());
          // 保留两位小数
          std::cout << "\rProgress: " << std::fixed << std::setprecision(2) << progress * 100 << "%";

          if (progress >= 1.0f)
          {
            std::cout << std::endl;
          }

        } });

  pool.Wait();

  // for (size_t x = 0; x < film.GetWidth(); x++)
  // {
  //     for (size_t y = 0; y < film.GetHeight(); y++)
  //     {
  //         Ray ray = camera.GenerateRay({x, y});

  //         auto result = sphere.Intersect(ray);
  //         if (result.has_value())
  //         {
  //             film.SetPixel(x, y, {0.5, 0.7, 0.2});
  //         }
  //         else
  //         {
  //             film.SetPixel(x, y, {0.2, 0.2, 0.2});
  //         }
  //     }
  // }

  // 结束计时
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "ParallelFor execution time: " << elapsed.count() << " seconds"
            << std::endl;

  film.Save(OUTPUT_PATH);
  return 0;
}