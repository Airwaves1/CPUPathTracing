#include "camera/film.hpp"
#include "thread/thread_pool.hpp"
#include "util/rgb.hpp"
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

Film::Film(size_t width, size_t height) : width(width), height(height) {
  pixels.resize(width * height);
}

void Film::Save(const std::filesystem::path &path) {
  std::ofstream file(path, std::ios::binary);

  file << "P6\n";
  file << width << " " << height << "\n";
  file << "255\n";

  std::vector<uint8_t> buffer(width * height * 3);

  thread_pool.ParallelFor(
      width, height,
      [&](size_t x, size_t y) {
        auto pixel = GetPixel(x, y);
        RGB rgb(pixel.color / static_cast<float>(pixel.sample_count));
        auto idx = (y * width + x) * 3;
        buffer[idx + 0] = rgb.r;
        buffer[idx + 1] = rgb.g;
        buffer[idx + 2] = rgb.b;
      },
      false);

  thread_pool.Wait();

  file.write(reinterpret_cast<char *>(buffer.data()), buffer.size());

  file.close();
}
