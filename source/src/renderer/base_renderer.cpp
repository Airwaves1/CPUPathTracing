#include "renderer/base_renderer.hpp"
#include "thread/thread_pool.hpp"
#include "util/progress.hpp"
#include <cstddef>

void BaseRenderer::RenderToImage(std::string filename, int spp) {
  size_t current_spp = 0;
  size_t increase = 1; // 一次并行for循环去渲染多少个spp

  // 获取相机的胶片
  auto &film = m_Camera.GetFilm();

  // 简易进度条
  Progress progress{film.GetWidth() * film.GetHeight() * spp};

  while (current_spp < spp) {

    // 并行for循环渲染图像
    thread_pool.ParallelFor(film.GetWidth(), film.GetHeight(),
                            [&](size_t x, size_t y) {
                              for (int i = 0; i < increase; i++) {
                                film.AddSample(x, y, RenderPixel({x, y}));
                              }
                              progress.Update(increase);
                            });

    thread_pool.Wait();

    // render increase
    current_spp += increase;
    increase = std::min<size_t>(current_spp, 32);

    film.Save(filename);
  }
}