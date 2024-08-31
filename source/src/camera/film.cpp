#include "camera/film.hpp"
#include <cstdint>
#include <fstream>
#include <iostream>
#include "util/rgb.hpp"
#include "util/profile.hpp"

Film::Film(size_t width, size_t height) : width(width), height(height) {
  pixels.resize(width * height);
}

void Film::Save(const std::filesystem::path &path) {
  PROFILE("Save to " + path.string());

  std::ofstream file(path, std::ios::binary);

  file << "P6\n";
  file << width << " " << height << "\n";
  file << "255\n";

  for (size_t y = 0; y < height; y++) {
    for (size_t x = 0; x < width; x++) {

      auto pixel = GetPixel(x, y);
      RGB rgb(pixel.color / static_cast<float>(pixel.sample_count));

      file << static_cast<uint8_t>(rgb.r) << static_cast<uint8_t>(rgb.g)
           << static_cast<uint8_t>(rgb.b);
    }
  }

  file.close();

}
